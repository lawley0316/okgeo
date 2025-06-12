#include "manager.h"
#include "signalmanager.h""
#include "src/bend/gateway.h"

#include <QFile>
#include <QApplication>
#include <QTextStream>

Q_GLOBAL_STATIC(Manager, ins)

Manager::Manager(QObject *parent)
    : QObject{parent}
    , gateway(new Gateway(this))
    , signalManager(new SignalManager(this))
{}

Manager* Manager::getInstance()
{
    return ins();
}

void Manager::init()
{
    // base
    QFile baseQss(":/static/base.qss");
    baseQss.open(QIODevice::ReadOnly);
    QString qss = baseQss.readAll();

    // UI
    QFile uiQss(":/static/ui.qss");
    uiQss.open(QIODevice::ReadOnly);
    qss += uiQss.readAll();
    qApp->setStyleSheet(qss);
}
