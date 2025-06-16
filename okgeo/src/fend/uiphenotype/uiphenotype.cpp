#include "uiphenotype.h"
#include "ui_uiphenotype.h"
#include "src/consts/api.h"
#include "src/middle/signals.h"
#include "src/bend/gateway.h"
#include "src/middle/manager.h"

#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QJsonObject>
#include <QTimer>
#include <QDesktopServices>

UiPhenotype::UiPhenotype(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UiPhenotype)
    , mPreviousDir(QDir::homePath())
{
    ui->setupUi(this);
    ui->seriesMatrixFile->setEnabled(false);
    ui->outfile->setEnabled(false);
    ui->parse->setEnabled(false);
    connect(ui->browseSeriesMatrixFile, &QPushButton::clicked, this, &UiPhenotype::BrowseSeriesMatrixFile);
    connect(ui->browseOutfile, &QPushButton::clicked, this, &UiPhenotype::BrowseOutfile);
    connect(ui->parse, &QPushButton::clicked, this, &UiPhenotype::Parse);
}

UiPhenotype::~UiPhenotype()
{
    delete ui;
}

bool UiPhenotype::IsValid() const
{
    return ! (ui->seriesMatrixFile->text().isEmpty() || ui->outfile->text().isEmpty());
}

QJsonObject UiPhenotype::GetParams() const
{
    QJsonObject params;
    params["seriesMatrixFile"] = ui->seriesMatrixFile->text();
    params["outfile"] = ui->outfile->text();
    return params;
}

void UiPhenotype::UpdateUi()
{
    ui->parse->setEnabled(IsValid());
}

void UiPhenotype::BrowseSeriesMatrixFile()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Please Choose"), mPreviousDir, tr("Series Matrix File (*.txt)"));
    if (path.isEmpty()) return;
    mPreviousDir = QFileInfo(path).dir().path();
    ui->seriesMatrixFile->setText(path);
    UpdateUi();
}

void UiPhenotype::BrowseOutfile()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Please Choose"), mPreviousDir + "/phenotype.txt", tr("Text File (*.txt)"));
    if (path.isEmpty()) return;
    mPreviousDir = QFileInfo(path).dir().path();
    ui->outfile->setText(path);
    UpdateUi();
}

void UiPhenotype::Parse()
{
    MANAGER->mGateway->Send(API::PHENOTYPE::PARSE, GetParams());
    emit MANAGER->mSignals->Loading();
}
