#include "uiabout.h"
#include "ui_uiabout.h"

UiAbout::UiAbout(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UiAbout)
{
    ui->setupUi(this);
    setWindowTitle(tr("About"));
    ui->logo->setFixedSize(60, 60);
    ui->logo->setPixmap(QPixmap(":/static/logo.png"));
    ui->logo->setScaledContents(true);
}

UiAbout::~UiAbout()
{
    delete ui;
}
