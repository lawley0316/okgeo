#include "rapidcsv.h"
#include "helpers.h"
#include "str.h"

#include <fstream>
#include <algorithm>
#include <iostream>

void ProbeExprHelper::parse(const std::string& path, Ids& probes, Ids& samples, Exprs& probeExprs)
{
    rapidcsv::Document doc(
        path,
        rapidcsv::LabelParams(0, 0),
        rapidcsv::SeparatorParams('\t'),
        rapidcsv::ConverterParams(true),
        rapidcsv::LineReaderParams(true, '!', true)
    );
    if ((doc.GetRowCount() == 0) || (doc.GetColumnCount() == 0)) {
        throw ProbeFileParseError("malformed probe file");
    }
    probes = doc.GetRowNames();
    samples = doc.GetColumnNames();
    for (std::size_t i=0; i<doc.GetRowCount(); ++i) {
        Expr expr = doc.GetRow<double>(i);
        if (expr.size() != samples.size()) {
            throw ProbeFileParseError("inconsistent number of columns across rows");
        }
        probeExprs.push_back(std::move(expr));
    }
}

void AnnoHelper::parse(const std::string& path, int column, Anno& anno)
{
    rapidcsv::Document doc(
        path,
        rapidcsv::LabelParams(0, 0),
        rapidcsv::SeparatorParams('\t'),
        rapidcsv::ConverterParams(),
        rapidcsv::LineReaderParams(true, '#', true)
    );
    if ((doc.GetRowCount() == 0) || (doc.GetColumnCount() == 0)) {
        throw AnnoFileParseError("malformed annotatinon file");
    }
    if (doc.GetColumnCount() < column - 1) {
        throw AnnoFileParseError("invalid gene symbol column");
    }
    const std::vector<std::string> probes = doc.GetRowNames();
    const std::vector<std::string> genes = doc.GetColumn<std::string>(column-2);
    for (std::size_t i=0; i<probes.size(); ++i) {
        if ((! genes[i].empty() && (genes[i] != "---"))) {
            anno.emplace(probes[i], genes[i]);
        }
    }
}

void GeneExprHelper::map(Anno& anno, Ids& probes, Exprs& probeExprs, std::unordered_map<std::string, ExprPtrs>& geneProbeExprPtrs)
{
    for (std::size_t i=0; i<probes.size(); ++i) {
        auto it = anno.find(probes[i]);
        if (it != anno.end()) {
            geneProbeExprPtrs[it->second].push_back(&probeExprs[i]);
        }
    }
}

void GeneExprHelper::merge(std::unordered_map<std::string, ExprPtrs>& geneProbeExprPtrs, const std::string& method, Ids& genes, Exprs& geneExprs)
{
    void (*func)(ExprPtrs&, Expr&) = nullptr;
    if (method == "mean") {
        func = mean;
    } else if (method == "median") {
        func = median;
    } else if (method == "max") {
        func = max;
    } else if (method == "min") {
        func = min;
    } else {
        throw std::runtime_error(method + ": invalid probe merge method");
    }
    for (const auto& pair : geneProbeExprPtrs) {
        genes.push_back(pair.first);
    }
    std::sort(genes.begin(), genes.end());
    for (const std::string& gene : genes) {
        if (geneProbeExprPtrs[gene].size() > 1) {
            Expr geneExpr;
            func(geneProbeExprPtrs[gene], geneExpr);
            geneExprs.push_back(std::move(geneExpr));
        } else {
            geneExprs.push_back(*(geneProbeExprPtrs[gene][0]));
        }
    }
}

void GeneExprHelper::write(Ids& genes, Ids& samples, Exprs& geneExprs, const std::string& outfile)
{
    std::ofstream out(outfile);
    if (! out) return;
    out << "Gene\t";
    for (std::size_t i=0; i<samples.size(); ++i) {
        if (i == samples.size() - 1) {
            out << samples[i] << '\n';
        } else {
            out << samples[i] << '\t';
        }
    }
    for (std::size_t i=0; i<genes.size(); ++i) {
        out << genes[i] << '\t';
        for (std::size_t j=0; j<geneExprs[i].size(); ++j) {
            if (j == geneExprs[i].size() - 1) {
                out << geneExprs[i][j] << '\n';
            } else {
                out << geneExprs[i][j] << '\t';
            }
        }
    }
}

void GeneExprHelper::mean(ExprPtrs& probeExprPtrs, Expr& geneExpr)
{
    std::size_t probes = probeExprPtrs.size();
    std::size_t samples = probeExprPtrs[0]->size();
    geneExpr.reserve(samples);
    for (std::size_t j=0; j<samples; ++j) {
        double sum = 0.0;
        bool hasnan = false;
        for (std::size_t i=0; i<probes; ++i) {
            double pev = (*probeExprPtrs[i])[j];
            if (std::isnan(pev)) {
                hasnan = true;
                break;
            } else {
                sum += pev;
            }
        }
        geneExpr.push_back(hasnan ? std::nan("") : sum / probes);
    }
}

void GeneExprHelper::median(ExprPtrs& probeExprPtrs, Expr& geneExpr)
{
    std::size_t probes = probeExprPtrs.size();
    std::size_t samples = probeExprPtrs[0]->size();
    geneExpr.reserve(samples);
    for (std::size_t j=0; j<samples; ++j) {
        std::vector<double> pevs;
        pevs.reserve(probes);
        bool hasnan = false;
        for (std::size_t i=0; i<probes; ++i) {
            double pev = (*probeExprPtrs[i])[j];
            if (std::isnan(pev)) {
                hasnan = true;
                break;
            } else {
                pevs.push_back(pev);
            }
        }
        geneExpr.push_back(hasnan ? std::nan("") : stats::median(std::move(pevs)));
    }
}

void GeneExprHelper::max(ExprPtrs& probeExprPtrs, Expr& geneExpr)
{
    std::size_t probes = probeExprPtrs.size();
    std::size_t samples = probeExprPtrs[0]->size();
    geneExpr.reserve(samples);
    for (std::size_t j=0; j<samples; ++j) {
        bool hasnan = false;
        double gev = (*probeExprPtrs[0])[j];
        for (std::size_t i=0; i<probes; ++i) {
            double pev = (*probeExprPtrs[i])[j];
            if (std::isnan(pev)) {
                hasnan = true;
                break;
            } else {
                if (pev > gev) {
                    gev = pev;
                }
            }
        }
        geneExpr.push_back(hasnan ? std::nan("") : gev);
    }
}

void GeneExprHelper::min(ExprPtrs& probeExprPtrs, Expr& geneExpr)
{
    std::size_t probes = probeExprPtrs.size();
    std::size_t samples = probeExprPtrs[0]->size();
    geneExpr.reserve(samples);
    for (std::size_t j=0; j<samples; ++j) {
        bool hasnan = false;
        double gev = (*probeExprPtrs[0])[j];
        for (std::size_t i=0; i<probes; ++i) {
            double pev = (*probeExprPtrs[i])[j];
            if (std::isnan(pev)) {
                hasnan = true;
                break;
            } else {
                if (pev < gev) {
                    gev = pev;
                }
            }
        }
        geneExpr.push_back(hasnan ? std::nan("") : gev);
    }
}

void PhenotypeHelper::Parse(const std::string& file, Phenotype& phenotype)
{
    std::ifstream in(file);
    if (! in)
    {
        throw OsError(file + ": failed to open file");
    }
    Phenotype hPhenotype;  // horizontal phenotype data
    std::string line;
    while (std::getline(in, line))
    {
        if (line.size() < 9)
        {
            continue;
        }
        if (str::StartsWith(line, "!series_matrix_table_begin"))
        {
            break;
        }
        if (str::StartsWith(line, "!Sample_"))
        {
            std::vector<std::string> column;
            str::Split(line.substr(8), '\t', column);
            hPhenotype.push_back(std::move(column));
        }
    }

    // check if series matrix file is malformed
    std::size_t cols = hPhenotype.size();
    if (cols == 0)
    {
        throw SeriesMatrixFileParseError(file + ": malformed series matrix file");
    }

    // check if number of rows across columns is consistent
    std::size_t rows = hPhenotype[0].size();
    for (std::size_t col=1; col<cols; ++col)
    {
        if (hPhenotype[col].size() != rows)
        {
            throw SeriesMatrixFileParseError(file + ": inconsistent number of rows across columns");
        }
    }

    // initialize phenotype dimensions
    phenotype.resize(rows);
    for (std::size_t row=0; row<rows; ++row)
    {
        phenotype[row].reserve(cols);
    }

    // push column names to phenotype
    std::string colname;
    std::unordered_map<std::string, int> counts;
    for (std::size_t col=0; col<cols; ++col)
    {
        colname = hPhenotype[col][0];
        auto it = counts.find(colname);
        if (it != counts.end())
        {
            phenotype[0].push_back(colname + "." + std::to_string(it->second));
            it->second += 1;
        }
        else
        {
            counts[colname] = 1;
            phenotype[0].push_back(colname);
        }
    }

    // push real data to phenotype
    for (std::size_t row=1; row<rows; ++row)
    {
        for (std::size_t col=0; col<cols; ++col)
        {
            phenotype[row].push_back(std::move(hPhenotype[col][row]));
        }
    }
}

void PhenotypeHelper::Write(const Phenotype& phenotype, const std::string& file)
{
    std::ofstream out(file);
    if (! out)
    {
        throw OsError(file + ": failed to open file");
    }
    std::size_t rows = phenotype.size();
    std::size_t cols = phenotype[0].size();
    for (std::size_t row=0; row<rows; ++row)
    {
        for (std::size_t col=0; col<cols; ++col)
        {
            if (col < cols - 1)
            {
                out << phenotype[row][col] << '\t';
            }
            else
            {
                out << phenotype[row][col] << '\n';
            }
        }
    }
}
