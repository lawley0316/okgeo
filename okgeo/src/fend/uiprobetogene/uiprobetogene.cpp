#include "uiprobetogene.h"
#include "ui_uiprobetogene.h"
#include "src/consts/api.h"
#include "src/bend/gateway.h"
#include "src/middle/signalmanager.h"
#include "src/middle/manager.h"
#include "src/fend/uicom/dialog.h"

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
    connect(ui->browseProbeFile, &QPushButton::clicked, this, &UiProbeToGene::onBrowseProbeFileClicked);

    // annotation file
    ui->annoFile->setEnabled(false);
    connect(ui->browseAnnoFile, &QPushButton::clicked, this, &UiProbeToGene::onBrowseAnnoFileClicked);

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
    connect(ui->browseOutfile, &QPushButton::clicked, this, &UiProbeToGene::onBrowseOutfileClicked);

    // convert button
    ui->convert->setEnabled(false);
    connect(ui->convert, &QPushButton::clicked, this, &UiProbeToGene::onConvertClicked);

    // signals
    connect(MANAGER->signalManager, &SignalManager::converted, this, &UiProbeToGene::onConverted);
}

UiProbeToGene::~UiProbeToGene()
{
    delete ui;
}

bool UiProbeToGene::isValid() const
{
    QString probeFile = ui->probeFile->text();
    if (probeFile.isEmpty()) return false;
    QString annoFile = ui->annoFile->text();
    if (annoFile.isEmpty()) return false;
    QString outfile = ui->outfile->text();
    return ! outfile.isEmpty();
}

QJsonObject UiProbeToGene::getParams() const
{
    QJsonObject params;
    params["probeFile"] = ui->probeFile->text();
    params["annoFile"] = ui->annoFile->text();
    params["column"] = ui->column->value();
    params["method"] = ui->method->currentText();
    params["outfile"] = ui->outfile->text();
    return params;
}

void UiProbeToGene::updateUi()
{
    if (isValid()) {
        ui->convert->setEnabled(true);
    }
}

void UiProbeToGene::disableUi()
{
    ui->browseProbeFile->setEnabled(false);
    ui->browseAnnoFile->setEnabled(false);
    ui->column->setEnabled(false);
    ui->method->setEnabled(false);
    ui->browseOutfile->setEnabled(false);
    ui->convert->setEnabled(false);
}

void UiProbeToGene::enableUi()
{
    ui->browseProbeFile->setEnabled(true);
    ui->browseAnnoFile->setEnabled(true);
    ui->column->setEnabled(true);
    ui->method->setEnabled(true);
    ui->browseOutfile->setEnabled(true);
    updateUi();
}

void UiProbeToGene::onBrowseProbeFileClicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Please Choose"), previousDir, tr("Series Matrix File (*.txt)"));
    if (path.isEmpty()) return;
    previousDir = QFileInfo(path).dir().path();
    ui->probeFile->setText(path);
    updateUi();
}

void UiProbeToGene::onBrowseAnnoFileClicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Please Choose"), previousDir, tr("Annotation File (*.txt)"));
    if (path.isEmpty()) return;
    previousDir = QFileInfo(path).dir().path();
    ui->annoFile->setText(path);
    updateUi();
}

void UiProbeToGene::onBrowseOutfileClicked()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Please Choose"), previousDir + "/expressions.txt", tr("Text file (*.txt)"));
    if (path.isEmpty()) return;
    previousDir = QFileInfo(path).dir().path();
    ui->outfile->setText(path);
    updateUi();
}

void UiProbeToGene::onConvertClicked()
{
    MANAGER->gateway->send(api::convert::CREATE, getParams());
    emit MANAGER->signalManager->loading();
}

void UiProbeToGene::onConverted()
{
    Dialog dialog(this);
    dialog.setTitle(tr("Successfully converted."));
    dialog.setContent(tr("Do you want to open the result file?"));
    dialog.addOkButton();
    dialog.addCancelButton();
    if (dialog.exec() == Dialog::Accepted) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(ui->outfile->text()));
    }
    enableUi();
}
