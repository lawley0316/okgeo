#include "manager.h"

#include <QFile>
#include <QApplication>
#include <QTextStream>
#include <QLocale>
#include <QTranslator>
#include <QLibraryInfo>

#include "signals.h"
#include "language.h"
#include "src/bend/gateway.h"

Q_GLOBAL_STATIC(Manager, ins)

Manager::Manager(QObject *parent)
    : QObject{parent}
    , gtw(new Gateway(this))
    , sigs(new Signals(this))
    , lang(new Language(this)) {}

Manager* Manager::GetInstance() {
    return ins();
}

void Manager::Initialize() {
    // base
    QFile base_qss(":/static/base.qss");
    base_qss.open(QIODevice::ReadOnly);
    QString qss = base_qss.readAll();

    // UI
    QFile ui_qss(":/static/ui.qss");
    ui_qss.open(QIODevice::ReadOnly);
    qss += ui_qss.readAll();
    qApp->setStyleSheet(qss);

    // i18n
    lang->Initialize();
}
