#ifndef SIGNALMANAGER_H
#define SIGNALMANAGER_H

#include <QObject>

class SignalManager : public QObject
{
    Q_OBJECT
public:
    explicit SignalManager(QObject *parent = nullptr);

signals:
    void loading();
    void done();
    void converted();
    void errorOccurred(const QString& detail);
};

#endif // SIGNALMANAGER_H
