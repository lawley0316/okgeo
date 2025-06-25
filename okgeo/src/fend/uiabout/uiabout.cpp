#include "uiabout.h"
#include "ui_uiabout.h"

#include <QDesktopServices>

UiAbout::UiAbout(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UiAbout) {
    ui->setupUi(this);
    ui->logo->setFixedSize(60, 60);
    ui->logo->setPixmap(QPixmap(":/static/logo.png"));
    ui->logo->setScaledContents(true);
    ui->website->installEventFilter(this);
}

UiAbout::~UiAbout() {
    delete ui;
}

void UiAbout::OpenDocs() {
    QDesktopServices::openUrl(QUrl("https://geo.oakes.cn"));
}

bool UiAbout::eventFilter(QObject* watched, QEvent* event) {
    if ((watched == ui->website) && (event->type() == QEvent::MouseButtonPress)) {
        OpenDocs();
    }
    return QDialog::eventFilter(watched, event);
}
