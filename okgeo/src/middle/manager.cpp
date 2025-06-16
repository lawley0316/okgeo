#include "manager.h"
#include "signals.h"
#include "src/bend/gateway.h"

#include <QFile>
#include <QApplication>
#include <QTextStream>

Q_GLOBAL_STATIC(Manager, ins)

Manager::Manager(QObject *parent)
    : QObject{parent}
    , mGateway(new Gateway(this))
    , mSignals(new Signals(this))
{}

Manager* Manager::GetInstance()
{
    return ins();
}

void Manager::Initialize()
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
