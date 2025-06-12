#ifndef GATEWAY_H
#define GATEWAY_H

#include <QObject>
#include <QJsonValue>

class Gateway : public QObject
{
    Q_OBJECT
public:
    explicit Gateway(QObject *parent = nullptr);
    void send(int api, const QJsonValue& params);

protected:
    void dispatch(int api, const QJsonValue& params);
    void convert(const QJsonValue& params);

signals:
};

#endif // GATEWAY_H
