#ifndef MANAGER_H
#define MANAGER_H

#define MANAGER Manager::getInstance()

#include <QObject>

class Gateway;
class SignalManager;

class Manager : public QObject
{
    Q_OBJECT
public:
    explicit Manager(QObject *parent = nullptr);
    static Manager* getInstance();
    void init();

public:
    Gateway* gateway;
    SignalManager* signalManager;
};

#endif // MANAGER_H
