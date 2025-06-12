#include "gateway.h"
#include "src/consts/api.h"
#include "src/utils/error.h"
#include "src/utils/helpers.h"
#include "src/middle/signalmanager.h"
#include "src/middle/manager.h"

#include <QtConcurrent>

Gateway::Gateway(QObject *parent)
    : QObject{parent}
{}

void Gateway::send(int api, const QJsonValue& params)
{
    QtConcurrent::run([=]() {
        try {
            dispatch(api, params);
        } catch (const Error& err) {
            emit MANAGER->signalManager->errorOccurred(err.message());
        } catch (...) {
            emit MANAGER->signalManager->errorOccurred(tr("unexpected error occurred"));
        }
    });
}

void Gateway::dispatch(int api, const QJsonValue& params)
{
    switch (api) {
        case (api::convert::CREATE): {
            convert(params);
            break;
        }
        default:
            break;
    }
}

void Gateway::convert(const QJsonValue& params)
{
    std::string probeFile = params["probeFile"].toString().toStdString();
    std::string annoFile = params["annoFile"].toString().toStdString();
    int column = params["column"].toInt();
    std::string method = params["method"].toString().toStdString();
    std::string outfile = params["outfile"].toString().toStdString();

    // 1. parse probe file
    Ids probes;
    Ids samples;
    Exprs probeExprs;
    try {
        ProbeExprHelper::parse(probeFile, probes, samples, probeExprs);
    } catch (const OkGeoError& err) {
        throw errors::convert::probeFile::parseFailed(err.what());
    }

    // 2. parse annotation file
    Anno anno;
    try {
        AnnoHelper::parse(annoFile, column, anno);
    } catch (const OkGeoError& err) {
        throw errors::convert::annoFile::parseFailed(err.what());
    }

    // 3. map probe expression
    std::unordered_map<std::string, ExprPtrs> geneProbeExprs;
    GeneExprHelper::map(anno, probes, probeExprs, geneProbeExprs);
    if (geneProbeExprs.empty()) {
        throw errors::convert::map::mapFailed("zero probes successfully annotated to genes");
    }

    // 4. merge
    Ids genes;
    Exprs geneExprs;
    GeneExprHelper::merge(geneProbeExprs, method, genes, geneExprs);

    // 5. save
    GeneExprHelper::write(genes, samples, geneExprs, outfile);
    qDebug() << "foo";
    emit MANAGER->signalManager->converted();
}
