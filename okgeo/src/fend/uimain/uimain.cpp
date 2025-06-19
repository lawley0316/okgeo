#include "uimain.h"
#include "ui_uimain.h"

#include <QTimer>
#include <QKeyEvent>
#include <QDesktopServices>

#include "src/middle/manager.h"
#include "src/middle/signals.h"
#include "src/fend/uicom/dialog.h"
#include "src/fend/uiabout/uiabout.h"

UiMain::UiMain(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UiMain)
    , loading_(new Loading(this)) {
    ui->setupUi(this);
    setWindowTitle("OkGEO");

    // logo
    ui->logo->setFixedSize(QSize(40, 40));
    ui->logo->setPixmap(QPixmap(":/static/logo.png"));
    ui->logo->setScaledContents(true);

    // probe to gene
    ui->probe_to_gene->SetDefaultIcon(QIcon(":/static/map-dark.png"));
    ui->probe_to_gene->SetActiveIcon(QIcon(":/static/map-light.png"));
    ui->probe_to_gene->SetDefaultTextColor(QColor("#666666"));
    ui->probe_to_gene->SetActiveTextColor(QColor("#455DD0"));
    ui->probe_to_gene->setIconSize(QSize(25, 25));
    ui->probe_to_gene->setText(tr("Probe to Gene"));
    ui->probe_to_gene->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->probe_to_gene->setToolTip(tr(
        "Given a probe annotation file, the column containing gene symbols, "
        "and the method for merging probes, convert probe expression to gene expression."
    ));
    connect(ui->probe_to_gene, &QToolButton::clicked, this, &UiMain::ConvertProbeToGene);

    // phenotype
    ui->phenotype->SetDefaultIcon(QIcon(":/static/phenotype-dark.png"));
    ui->phenotype->SetActiveIcon(QIcon(":/static/phenotype-light.png"));
    ui->phenotype->SetDefaultTextColor(QColor("#666666"));
    ui->phenotype->SetActiveTextColor(QColor("#455DD0"));
    ui->phenotype->setIconSize(QSize(25, 25));
    ui->phenotype->setText(tr("Phenotype"));
    ui->phenotype->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->phenotype->setToolTip(tr("Parse the phenotype data from the Series Matrix file."));
    connect(ui->phenotype, &QToolButton::clicked, this, &UiMain::ParsePhenotype);

    // help
    ui->help->SetDefaultIcon(QIcon(":/static/help-dark.png"));
    ui->help->SetActiveIcon(QIcon(":/static/help-light.png"));
    ui->help->setIconSize(QSize(25, 25));
    ui->help->setToolTip(tr("Help"));
    connect(ui->help, &QToolButton::clicked, this, &UiMain::Help);

    // about
    ui->about->SetDefaultIcon(QIcon(":/static/about-dark.png"));
    ui->about->SetActiveIcon(QIcon(":/static/about-light.png"));
    ui->about->setIconSize(QSize(25, 25));
    ui->about->setToolTip(tr("About"));
    connect(ui->about, &QToolButton::clicked, this, &UiMain::About);

    ui->probe_to_gene->Activate();
    ui->pages->setCurrentIndex(0);

    // signals
    connect(MANAGER->sigs, &Signals::Loading, loading_, &QDialog::exec);
    connect(MANAGER->sigs, &Signals::ProbeToGeneConverted, this, &UiMain::OpenResultFile);
    connect(MANAGER->sigs, &Signals::PhenotypeParsed, this, &UiMain::OpenResultFile);
    connect(MANAGER->sigs, &Signals::ErrorOccurred, this, &UiMain::ShowError);
}

UiMain::~UiMain() {
    delete ui;
    delete loading_;
}

void UiMain::ConvertProbeToGene() {
    ui->probe_to_gene->Activate();
    ui->phenotype->Deactivate();
    ui->pages->setCurrentIndex(0);
}

void UiMain::ParsePhenotype() {
    ui->probe_to_gene->Deactivate();
    ui->phenotype->Activate();
    ui->pages->setCurrentIndex(1);
}

void UiMain::OpenResultFile(const QString& path) {
    loading_->accept();
    QTimer::singleShot(
        0, this, [=]() {
            Dialog dialog(this);
            dialog.SetTitle(tr("Finished."));
            dialog.SetContent(tr("Do you want to open the result file?"));
            dialog.AddOkButton();
            dialog.AddCancelButton();
            if (dialog.exec() == Dialog::Accepted) {
                QDesktopServices::openUrl(QUrl::fromLocalFile(path));
            }
        }
    );
}

void UiMain::ShowError(int api, const QJsonValue& params, const QString& message) {
    loading_->accept();
    QTimer::singleShot(
        0, this, [=]() {
            Dialog dialog(this);
            dialog.SetTitle(tr("Error occurred."));
            dialog.SetContent(message);
            dialog.AddOkButton();
            dialog.exec();
        }
    );
}

void UiMain::Help() {
    QDesktopServices::openUrl(QUrl("https://okgeo.lawley.cn"));
}

void UiMain::About() {
    UiAbout about;
    about.exec();
}

void UiMain::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) return;
    QDialog::keyPressEvent(event);
}
