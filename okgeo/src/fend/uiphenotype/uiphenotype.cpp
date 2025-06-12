#include "uiphenotype.h"
#include "ui_uiphenotype.h"

#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QJsonObject>

UiPhenotype::UiPhenotype(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UiPhenotype)
    , previousDir(QDir::homePath())
{
    ui->setupUi(this);
    ui->seriesMatrixFile->setEnabled(false);
    ui->outfile->setEnabled(false);
    ui->parse->setEnabled(false);
    connect(ui->browseSeriesMatrixFile, &QPushButton::clicked, this, &UiPhenotype::onBrowseSeriesMatrixFileClicked);
    connect(ui->browseOutfile, &QPushButton::clicked, this, &UiPhenotype::onBrowseOutfileClicked);
    connect(ui->parse, &QPushButton::clicked, this, &UiPhenotype::onParseClicked);
}

UiPhenotype::~UiPhenotype()
{
    delete ui;
}

bool UiPhenotype::isValid() const
{
    return ! (ui->seriesMatrixFile->text().isEmpty() || ui->outfile->text().isEmpty());
}

QJsonObject UiPhenotype::getParams() const
{
    QJsonObject params;
    params["seriesMatrixFile"] = ui->seriesMatrixFile->text();
    params["outfile"] = ui->outfile->text();
    return params;
}

void UiPhenotype::updateUi()
{
    ui->parse->setEnabled(isValid());
}

void UiPhenotype::onBrowseSeriesMatrixFileClicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Please Choose"), previousDir, tr("Series Matrix File (*.txt)"));
    if (path.isEmpty()) return;
    previousDir = QFileInfo(path).dir().path();
    ui->seriesMatrixFile->setText(path);
    updateUi();
}

void UiPhenotype::onBrowseOutfileClicked()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Please Choose"), previousDir + "/phenotype.txt", tr("Text File (*.txt)"));
    if (path.isEmpty()) return;
    previousDir = QFileInfo(path).dir().path();
    ui->outfile->setText(path);
    updateUi();
}

void UiPhenotype::onParseClicked()
{
    qDebug() << "parse";
}
