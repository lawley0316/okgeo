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
    , previous_dir_(QDir::homePath()) {
    ui->setupUi(this);

    // series matrix file
    ui->series_matrix_file->setEnabled(false);
    connect(ui->browse_series_matrix_file, &QPushButton::clicked, this, &UiPhenotype::BrowseSeriesMatrixFile);

    // outfile
    ui->outfile->setEnabled(false);
    connect(ui->browse_outfile, &QPushButton::clicked, this, &UiPhenotype::BrowseOutfile);

    // parse
    ui->parse->setEnabled(false);
    connect(ui->parse, &QPushButton::clicked, this, &UiPhenotype::Parse);
}

UiPhenotype::~UiPhenotype() {
    delete ui;
}

bool UiPhenotype::IsValid() const {
    return ! (ui->series_matrix_file->text().isEmpty() || ui->outfile->text().isEmpty());
}

QJsonObject UiPhenotype::GetParams() const {
    QJsonObject params;
    params["series_matrix_file"] = ui->series_matrix_file->text();
    params["outfile"] = ui->outfile->text();
    return params;
}

void UiPhenotype::UpdateUi() {
    ui->parse->setEnabled(IsValid());
}

void UiPhenotype::BrowseSeriesMatrixFile() {
    QString path = QFileDialog::getOpenFileName(this, tr("Please Choose"), previous_dir_, tr("Series Matrix File (*.txt)"));
    if (path.isEmpty()) return;
    previous_dir_ = QFileInfo(path).dir().path();
    ui->series_matrix_file->setText(path);
    UpdateUi();
}

void UiPhenotype::BrowseOutfile() {
    QString path = QFileDialog::getSaveFileName(this, tr("Please Choose"), previous_dir_ + "/phenotype.txt", tr("Text File (*.txt)"));
    if (path.isEmpty()) return;
    previous_dir_ = QFileInfo(path).dir().path();
    ui->outfile->setText(path);
    UpdateUi();
}

void UiPhenotype::Parse() {
    MANAGER->gtw->Send(API::PHENOTYPE::PARSE, GetParams());
    emit MANAGER->sigs->Loading();
}
