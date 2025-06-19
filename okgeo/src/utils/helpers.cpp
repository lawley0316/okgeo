#include "str.h"
#include "stats.h"
#include "helpers.h"
#include "rapidcsv.h"

#include <fstream>
#include <iostream>
#include <algorithm>

void ProbeExprHelper::Parse(const std::string& path, Ids& probes, Ids& samples, Exprs& probe_exprs)
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
    probe_exprs.reserve(doc.GetRowCount());
    for (std::size_t i=0; i<doc.GetRowCount(); ++i) {
        Expr expr = doc.GetRow<double>(i);
        if (expr.size() != samples.size()) {
            throw ProbeFileParseError("inconsistent number of columns across rows");
        }
        probe_exprs.push_back(std::move(expr));
    }
}

void AnnoHelper::Parse(const std::string& path, int column, Anno& anno) {
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

void GeneExprHelper::Map(Anno& anno, Ids& probes, Exprs& probe_exprs, std::unordered_map<std::string, ExprPtrs>& gene_probe_expr_ptrs) {
    for (std::size_t i=0; i<probes.size(); ++i) {
        auto it = anno.find(probes[i]);
        if (it != anno.end()) {
            gene_probe_expr_ptrs[it->second].push_back(&probe_exprs[i]);
        }
    }
}

void GeneExprHelper::Aggregate(std::unordered_map<std::string, ExprPtrs>& gene_probe_expr_ptrs, const std::string& method, Ids& genes, Exprs& gene_exprs)
{
    // aggregate function
    void (*func)(ExprPtrs&, Expr&) = nullptr;
    if (method == "mean") {
        func = Mean;
    } else if (method == "median") {
        func = Median;
    } else if (method == "max") {
        func = Max;
    } else if (method == "min") {
        func = Min;
    } else {
        throw std::runtime_error(method + ": invalid probe aggregate method");
    }

    // sort genes
    for (const auto& pair : gene_probe_expr_ptrs) {
        genes.push_back(pair.first);
    }
    std::sort(genes.begin(), genes.end());

    // aggregate
    for (const std::string& gene : genes) {
        if (gene_probe_expr_ptrs[gene].size() > 1) {
            Expr gene_expr;
            func(gene_probe_expr_ptrs[gene], gene_expr);
            gene_exprs.push_back(std::move(gene_expr));
        } else {
            gene_exprs.push_back(*(gene_probe_expr_ptrs[gene][0]));
        }
    }
}

void GeneExprHelper::Write(Ids& genes, Ids& samples, Exprs& gene_exprs, const std::string& outfile)
{
    std::ofstream out(outfile);
    if (! out) return;

    // write column names
    out << "Gene\t";
    for (std::size_t i=0; i<samples.size(); ++i) {
        if (i < samples.size() - 1) {
            out << samples[i] << '\t';
        } else {
            out << samples[i] << '\n';
        }
    }

    // write data
    for (std::size_t i=0; i<genes.size(); ++i) {
        out << genes[i] << '\t';
        for (std::size_t j=0; j<gene_exprs[i].size(); ++j) {
            if (j < gene_exprs[i].size() - 1) {
                out << gene_exprs[i][j] << '\t';
            } else {
                out << gene_exprs[i][j] << '\n';
            }
        }
    }
}

void GeneExprHelper::Mean(ExprPtrs& probe_expr_ptrs, Expr& gene_expr) {
    std::size_t probes = probe_expr_ptrs.size();
    std::size_t samples = probe_expr_ptrs[0]->size();
    gene_expr.reserve(samples);
    for (std::size_t j=0; j<samples; ++j) {
        double sum = 0.0;
        bool hasnan = false;
        for (std::size_t i=0; i<probes; ++i) {
            double pev = probe_expr_ptrs[i]->at(j);
            if (std::isnan(pev)) {
                hasnan = true;
                break;
            } else {
                sum += pev;
            }
        }
        gene_expr.push_back(hasnan ? std::nan("") : sum / probes);
    }
}

void GeneExprHelper::Median(ExprPtrs& probe_expr_ptrs, Expr& gene_expr) {
    std::size_t probes = probe_expr_ptrs.size();
    std::size_t samples = probe_expr_ptrs[0]->size();
    gene_expr.reserve(samples);
    std::vector<double> pevs;
    pevs.reserve(probes);
    for (std::size_t j=0; j<samples; ++j) {
        pevs.clear();
        bool hasnan = false;
        for (std::size_t i=0; i<probes; ++i) {
            double pev = probe_expr_ptrs[i]->at(j);
            if (std::isnan(pev)) {
                hasnan = true;
                break;
            } else {
                pevs.push_back(pev);
            }
        }
        gene_expr.push_back(hasnan ? std::nan("") : stats::MedianInplace(pevs.begin(), pevs.end()));
    }
}

void GeneExprHelper::Max(ExprPtrs& probe_expr_ptrs, Expr& gene_expr) {
    std::size_t probes = probe_expr_ptrs.size();
    std::size_t samples = probe_expr_ptrs[0]->size();
    gene_expr.reserve(samples);
    for (std::size_t j=0; j<samples; ++j) {
        bool hasnan = false;
        double gev = probe_expr_ptrs[0]->at(j);
        for (std::size_t i=0; i<probes; ++i) {
            double pev = probe_expr_ptrs[i]->at(j);
            if (std::isnan(pev)) {
                hasnan = true;
                break;
            } else {
                if (pev > gev) gev = pev;
            }
        }
        gene_expr.push_back(hasnan ? std::nan("") : gev);
    }
}

void GeneExprHelper::Min(ExprPtrs& probe_expr_ptrs, Expr& gene_expr) {
    std::size_t probes = probe_expr_ptrs.size();
    std::size_t samples = probe_expr_ptrs[0]->size();
    gene_expr.reserve(samples);
    for (std::size_t j=0; j<samples; ++j) {
        bool hasnan = false;
        double gev = probe_expr_ptrs[0]->at(j);
        for (std::size_t i=0; i<probes; ++i) {
            double pev = probe_expr_ptrs[i]->at(j);
            if (std::isnan(pev)) {
                hasnan = true;
                break;
            } else {
                if (pev < gev) gev = pev;
            }
        }
        gene_expr.push_back(hasnan ? std::nan("") : gev);
    }
}

void PhenotypeHelper::Parse(const std::string& file, Phenotype& phenotype) {
    std::ifstream in(file);
    if (! in) throw OsError(file + ": failed to open file");

    // read horizontail phenotype
    Phenotype h_phenotype;
    std::string line;
    while (std::getline(in, line)) {
        if (line.size() < 9) continue;
        if (str::StartsWith(line, "!series_matrix_table_begin")) break;
        if (str::StartsWith(line, "!Sample_")) {
            std::vector<std::string> column;
            str::Split(line.substr(8), '\t', column);
            h_phenotype.push_back(std::move(column));
        }
    }

    // check if series matrix file is malformed
    std::size_t cols = h_phenotype.size();
    if (cols == 0) throw SeriesMatrixFileParseError(file + ": malformed series matrix file");

    // check if number of rows across columns is consistent
    std::size_t rows = h_phenotype[0].size();
    for (std::size_t col=1; col<cols; ++col) {
        if (h_phenotype[col].size() != rows) {
            throw SeriesMatrixFileParseError(file + ": inconsistent number of rows across columns");
        }
    }

    // initialize phenotype dimensions
    phenotype.resize(rows);
    for (std::size_t row=0; row<rows; ++row) {
        phenotype[row].reserve(cols);
    }

    // push column names to phenotype
    std::string colname;
    std::unordered_map<std::string, int> counts;
    for (std::size_t col=0; col<cols; ++col) {
        colname = h_phenotype[col][0];
        auto it = counts.find(colname);
        if (it != counts.end()) {
            phenotype[0].push_back(colname + "." + std::to_string(it->second));
            it->second += 1;
        } else {
            counts[colname] = 1;
            phenotype[0].push_back(colname);
        }
    }

    // push real data to phenotype
    for (std::size_t row=1; row<rows; ++row) {
        for (std::size_t col=0; col<cols; ++col) {
            phenotype[row].push_back(std::move(h_phenotype[col][row]));
        }
    }
}

void PhenotypeHelper::Write(const Phenotype& phenotype, const std::string& file)
{
    std::ofstream out(file);
    if (! out) throw OsError(file + ": failed to open file");
    std::size_t rows = phenotype.size();
    std::size_t cols = phenotype[0].size();
    for (std::size_t row=0; row<rows; ++row) {
        for (std::size_t col=0; col<cols; ++col) {
            if (col < cols - 1) {
                out << phenotype[row][col] << '\t';
            } else {
                out << phenotype[row][col] << '\n';
            }
        }
    }
}
