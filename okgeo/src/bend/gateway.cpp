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
        try
        {
            Dispatch(api, params);
        }
        catch (const Error& err)
        {
            emit MANAGER->mSignals->ErrorOccurred(api, params, err.Message());
        }
        catch (...)
        {
            emit MANAGER->mSignals->ErrorOccurred(api, params, tr("unexpected error occurred"));
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
    QString probeFile = params["probeFile"].toString();
    QString annoFile = params["annoFile"].toString();
    int column = params["column"].toInt();
    QString method = params["method"].toString();
    QString outfile = params["outfile"].toString();

    // 1. parse probe file
    Ids probes;
    Ids samples;
    Exprs probeExprs;
    try {
        ProbeExprHelper::parse(probeFile.toStdString(), probes, samples, probeExprs);
    } catch (const OkGeoError& err) {
        throw Error::Get("111", err.what());
    }

    // 2. parse annotation file
    Anno anno;
    try {
        AnnoHelper::parse(annoFile.toStdString(), column, anno);
    } catch (const OkGeoError& err) {
        throw Error::Get("121", err.what());
    }

    // 3. map probe expression
    std::unordered_map<std::string, ExprPtrs> geneProbeExprs;
    GeneExprHelper::map(anno, probes, probeExprs, geneProbeExprs);
    if (geneProbeExprs.empty()) {
        throw Error::Get("131", "zero probes successfully annotated to genes");
    }

    // 4. merge
    Ids genes;
    Exprs geneExprs;
    GeneExprHelper::merge(geneProbeExprs, method.toStdString(), genes, geneExprs);

    // 5. save
    GeneExprHelper::write(genes, samples, geneExprs, outfile.toStdString());

    // 6. emit finished signal
    emit MANAGER->mSignals->ProbeToGeneConverted(outfile);
}

void Gateway::ParsePhenotype(const QJsonValue& params)
{
    QString seriesMatrixFile = params["seriesMatrixFile"].toString();
    QString outfile = params["outfile"].toString();
    Phenotype phenotype;

    // parse
    try
    {
        PhenotypeHelper::Parse(seriesMatrixFile.toStdString(), phenotype);
    }
    catch (const OkGeoError& err)
    {
        throw Error::Get("211", err.what());
    }

    // write
    try
    {
        PhenotypeHelper::Write(phenotype, outfile.toStdString());
    }
    catch (const OkGeoError& err)
    {
        throw Error::Get("221", err.what());
    }

    // emit finished signal
    emit MANAGER->mSignals->PhenotypeParsed(outfile);
}
