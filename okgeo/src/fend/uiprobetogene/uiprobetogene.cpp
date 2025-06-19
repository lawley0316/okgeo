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
    , previous_dir_(QDir::homePath()) {
    ui->setupUi(this);

    // probe file
    ui->probe_file->setEnabled(false);
    connect(ui->browse_probe_file, &QPushButton::clicked, this, &UiProbeToGene::BrowseProbeFile);

    // annotation file
    ui->anno_file->setEnabled(false);
    connect(ui->browse_anno_file, &QPushButton::clicked, this, &UiProbeToGene::BrowseAnnoFile);

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
    connect(ui->browse_outfile, &QPushButton::clicked, this, &UiProbeToGene::BrowseOutfile);

    // convert button
    ui->convert->setEnabled(false);
    connect(ui->convert, &QPushButton::clicked, this, &UiProbeToGene::Convert);
}

UiProbeToGene::~UiProbeToGene() {
    delete ui;
}

bool UiProbeToGene::IsValid() const {
    QString probe_file = ui->probe_file->text();
    if (probe_file.isEmpty()) return false;
    QString anno_file = ui->anno_file->text();
    if (anno_file.isEmpty()) return false;
    QString outfile = ui->outfile->text();
    return ! outfile.isEmpty();
}

QJsonObject UiProbeToGene::GetParams() const {
    QJsonObject params;
    params["probe_file"] = ui->probe_file->text();
    params["anno_file"] = ui->anno_file->text();
    params["column"] = ui->column->value();
    params["method"] = ui->method->currentText();
    params["outfile"] = ui->outfile->text();
    return params;
}

void UiProbeToGene::UpdateUi() {
    ui->convert->setEnabled(IsValid());
}

void UiProbeToGene::BrowseProbeFile() {
    QString path = QFileDialog::getOpenFileName(this, tr("Please Choose"), previous_dir_, tr("Series Matrix File (*.txt)"));
    if (path.isEmpty()) return;
    previous_dir_ = QFileInfo(path).dir().path();
    ui->probe_file->setText(path);
    UpdateUi();
}

void UiProbeToGene::BrowseAnnoFile() {
    QString path = QFileDialog::getOpenFileName(this, tr("Please Choose"), previous_dir_, tr("Annotation File (*.txt)"));
    if (path.isEmpty()) return;
    previous_dir_ = QFileInfo(path).dir().path();
    ui->anno_file->setText(path);
    UpdateUi();
}

void UiProbeToGene::BrowseOutfile() {
    QString path = QFileDialog::getSaveFileName(this, tr("Please Choose"), previous_dir_ + "/expressions.txt", tr("Text file (*.txt)"));
    if (path.isEmpty()) return;
    previous_dir_ = QFileInfo(path).dir().path();
    ui->outfile->setText(path);
    UpdateUi();
}

void UiProbeToGene::Convert() {
    MANAGER->gtw->Send(API::PROBE_TO_GENE::CONVERT, GetParams());
    emit MANAGER->sigs->Loading();
}
