#include "uiprobetogene.h"
#include "ui_uiprobetogene.h"
#include "src/consts/api.h"
#include "src/bend/gateway.h"
#include "src/middle/signals.h"
#include "src/middle/manager.h"

#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QDesktopServices>

UiProbeToGene::UiProbeToGene(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UiProbeToGene)
    , previousDir(QDir::homePath())
{
    ui->setupUi(this);

    // probe file
    ui->probeFile->setEnabled(false);
    connect(ui->browseProbeFile, &QPushButton::clicked, this, &UiProbeToGene::BrowseProbeFile);

    // annotation file
    ui->annoFile->setEnabled(false);
    connect(ui->browseAnnoFile, &QPushButton::clicked, this, &UiProbeToGene::BrowseAnnoFile);

    // gene column
    ui->column->setMinimum(2);
    ui->column->setMaximum(100);

    // probe merege method
    ui->method->addItem(tr("mean"), "mean");
    ui->method->addItem(tr("median"), "median");
    ui->method->addItem(tr("max"), "max");
    ui->method->addItem(tr("min"), "min");

    // outfile
    ui->outfile->setEnabled(false);
    connect(ui->browseOutfile, &QPushButton::clicked, this, &UiProbeToGene::BrowseOutfile);

    // convert button
    ui->convert->setEnabled(false);
    connect(ui->convert, &QPushButton::clicked, this, &UiProbeToGene::Convert);
}

UiProbeToGene::~UiProbeToGene()
{
    delete ui;
}

bool UiProbeToGene::IsValid() const
{
    QString probeFile = ui->probeFile->text();
    if (probeFile.isEmpty()) return false;
    QString annoFile = ui->annoFile->text();
    if (annoFile.isEmpty()) return false;
    QString outfile = ui->outfile->text();
    return ! outfile.isEmpty();
}

QJsonObject UiProbeToGene::GetParams() const
{
    QJsonObject params;
    params["probeFile"] = ui->probeFile->text();
    params["annoFile"] = ui->annoFile->text();
    params["column"] = ui->column->value();
    params["method"] = ui->method->currentText();
    params["outfile"] = ui->outfile->text();
    return params;
}

void UiProbeToGene::UpdateUi()
{
    ui->convert->setEnabled(IsValid());
}

void UiProbeToGene::BrowseProbeFile()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Please Choose"), previousDir, tr("Series Matrix File (*.txt)"));
    if (path.isEmpty()) return;
    previousDir = QFileInfo(path).dir().path();
    ui->probeFile->setText(path);
    UpdateUi();
}

void UiProbeToGene::BrowseAnnoFile()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Please Choose"), previousDir, tr("Annotation File (*.txt)"));
    if (path.isEmpty()) return;
    previousDir = QFileInfo(path).dir().path();
    ui->annoFile->setText(path);
    UpdateUi();
}

void UiProbeToGene::BrowseOutfile()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Please Choose"), previousDir + "/expressions.txt", tr("Text file (*.txt)"));
    if (path.isEmpty()) return;
    previousDir = QFileInfo(path).dir().path();
    ui->outfile->setText(path);
    UpdateUi();
}

void UiProbeToGene::Convert()
{
    MANAGER->mGateway->Send(API::PROBE_TO_GENE::CONVERT, GetParams());
    emit MANAGER->mSignals->Loading();
}
