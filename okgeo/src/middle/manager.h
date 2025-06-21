#ifndef MANAGER_H
#define MANAGER_H

#define MANAGER Manager::GetInstance()

#include <QObject>

class Gateway;
class Signals;
class Language;

class Manager : public QObject
{
    Q_OBJECT
public:
    explicit Manager(QObject *parent = nullptr);
    static Manager* GetInstance();
    void Initialize();

public:
    Gateway* gtw;
    Signals* sigs;
    Language* lang;
};

#endif // MANAGER_H
