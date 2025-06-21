#include "gateway.h"
#include "src/consts/api.h"
#include "src/utils/error.h"
#include "src/utils/helpers.h"
#include "src/middle/signals.h"
#include "src/middle/manager.h"

#include <QtConcurrent>

Gateway::Gateway(QObject *parent)
    : QObject{parent}
{}

void Gateway::Send(int api, const QJsonValue& params)
{
    QtConcurrent::run([=]() {
        try {
            Dispatch(api, params);
        } catch (const Error& err) {
            emit MANAGER->sigs->ErrorOccurred(api, params, err.Message());
        } catch (...) {
            emit MANAGER->sigs->ErrorOccurred(api, params, tr("unexpected error occurred"));
        }
    });
}

void Gateway::Dispatch(int api, const QJsonValue& params)
{
    switch (api) {
        case (API::PROBE_TO_GENE::CONVERT): {
            ConvertProbeToGene(params);
            break;
        }
        case (API::PHENOTYPE::PARSE): {
            ParsePhenotype(params);
            break;
        }
        default:
            break;
    }
}

void Gateway::ConvertProbeToGene(const QJsonValue& params)
{
    QString probe_file = params["probe_file"].toString();
    QString anno_file = params["anno_file"].toString();
    int column = params["column"].toInt();
    QString method = params["method"].toString();
    QString outfile = params["outfile"].toString();

    // 1. parse probe file
    Ids probes;
    Ids samples;
    Exprs probe_exprs;
    try {
        ProbeExprHelper::Parse(probe_file.toStdString(), probes, samples, probe_exprs);
    } catch (const OkGeoError& err) {
        throw Error::Get("111", err.what());
    }

    // 2. parse annotation file
    Anno anno;
    try {
        AnnoHelper::Parse(anno_file.toStdString(), column, anno);
    } catch (const OkGeoError& err) {
        throw Error::Get("121", err.what());
    }

    // 3. map probe expression
    std::unordered_map<std::string, ExprPtrs> gene_probe_exprs;
    GeneExprHelper::Map(anno, probes, probe_exprs, gene_probe_exprs);
    if (gene_probe_exprs.empty()) {
        throw Error::Get("131", tr("zero probes successfully annotated to genes"));
    }

    // 4. aggregate
    Ids genes;
    Exprs gene_exprs;
    GeneExprHelper::Aggregate(gene_probe_exprs, method.toStdString(), genes, gene_exprs);

    // 5. save
    GeneExprHelper::Write(genes, samples, gene_exprs, outfile.toStdString());

    // 6. emit finished signal
    emit MANAGER->sigs->ProbeToGeneConverted(outfile);
}

void Gateway::ParsePhenotype(const QJsonValue& params)
{
    QString series_matrix_file = params["series_matrix_file"].toString();
    QString outfile = params["outfile"].toString();
    Phenotype phenotype;

    // parse
    try {
        PhenotypeHelper::Parse(series_matrix_file.toStdString(), phenotype);
    } catch (const OkGeoError& err) {
        throw Error::Get("211", err.what());
    }

    // write
    try {
        PhenotypeHelper::Write(phenotype, outfile.toStdString());
    } catch (const OkGeoError& err) {
        throw Error::Get("221", err.what());
    }

    // emit finished signal
    emit MANAGER->sigs->PhenotypeParsed(outfile);
}
