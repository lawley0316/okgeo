#ifndef GATEWAY_H
#define GATEWAY_H

#include <QObject>
#include <QJsonValue>

class Gateway : public QObject
{
    Q_OBJECT
public:
    explicit Gateway(QObject *parent = nullptr);
    void Send(int api, const QJsonValue& params);

protected:
    void Dispatch(int api, const QJsonValue& params);
    void ConvertProbeToGene(const QJsonValue& params);
    void ParsePhenotype(const QJsonValue& params);

signals:
};

#endif // GATEWAY_H
