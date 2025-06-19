#ifndef SIGNALS_H
#define SIGNALS_H

#include <QObject>

class Signals : public QObject
{
    Q_OBJECT
public:
    explicit Signals(QObject *parent = nullptr);

signals:
    void Loading();
    void ProbeToGeneConverted(const QString& path);
    void PhenotypeParsed(const QString& path);
    void ErrorOccurred(int api, const QJsonValue& params, const QString& message);
};

#endif // SIGNALS_H
