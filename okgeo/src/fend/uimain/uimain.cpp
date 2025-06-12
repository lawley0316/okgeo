#include "uimain.h"
#include "ui_uimain.h"
#include "src/fend/uicom/dialog.h"
#include "src/middle/signalmanager.h"
#include "src/middle/manager.h"

UiMain::UiMain(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UiMain)
    , loading(new Loading(this))
{
    ui->setupUi(this);
    setWindowTitle("OkGEO");

    // logo
    ui->logo->setFixedSize(QSize(40, 40));
    ui->logo->setPixmap(QPixmap(":/static/logo.png"));
    ui->logo->setScaledContents(true);

    // probe to gene
    ui->probeToGene->setDefaultIcon(QIcon(":/static/map-dark.png"));
    ui->probeToGene->setActiveIcon(QIcon(":/static/map-light.png"));
    ui->probeToGene->setDefaultTextColor(QColor("#666666"));
    ui->probeToGene->setActiveTextColor(QColor("#455DD0"));
    ui->probeToGene->setIconSize(QSize(25, 25));
    ui->probeToGene->setText(tr("Probe to Gene"));
    ui->probeToGene->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->probeToGene->setToolTip(tr(
        "Given a probe annotation file, the column containing gene symbols, "
        "and the method for merging probes, convert probe expression to gene expression."
    ));
    connect(ui->probeToGene, &QToolButton::clicked, this, &UiMain::onProbeToGeneClicked);

    // parse
    ui->parsePheno->setDefaultIcon(QIcon(":/static/patient-dark.png"));
    ui->parsePheno->setActiveIcon(QIcon(":/static/patient-light.png"));
    ui->parsePheno->setDefaultTextColor(QColor("#666666"));
    ui->parsePheno->setActiveTextColor(QColor("#455DD0"));
    ui->parsePheno->setIconSize(QSize(25, 25));
    ui->parsePheno->setText(tr("Phenotype"));
    ui->parsePheno->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->parsePheno->setToolTip(tr("Parse the phenotype data from the Series Matrix file."));
    connect(ui->parsePheno, &QToolButton::clicked, this, &UiMain::onParsePhenoClicked);

    // about
    ui->about->setDefaultIcon(QIcon(":/static/about-dark.png"));
    ui->about->setActiveIcon(QIcon(":/static/about-light.png"));
    ui->about->setIconSize(QSize(25, 25));
    ui->about->setToolTip(tr("About"));

    ui->probeToGene->activate();
    ui->pages->setCurrentIndex(0);

    // signals
    connect(MANAGER->signalManager, &SignalManager::loading, this, &UiMain::onLoading);
    connect(MANAGER->signalManager, &SignalManager::converted, this, &UiMain::onConverted);
    connect(MANAGER->signalManager, &SignalManager::errorOccurred, this, &UiMain::onErrorOccurred);
}

UiMain::~UiMain()
{
    delete ui;
    delete loading;
}

void UiMain::onLoading()
{
    loading->exec();
}

void UiMain::onConverted()
{
    loading->close();
}

void UiMain::onErrorOccurred(const QString& detail)
{
    loading->close();
    Dialog dialog(this);
    dialog.setTitle(tr("Error occurred."));
    dialog.setContent(detail);
    dialog.addOkButton();
    dialog.exec();
}

void UiMain::onProbeToGeneClicked()
{
    ui->probeToGene->activate();
    ui->parsePheno->deactivate();
    ui->pages->setCurrentIndex(0);
}

void UiMain::onParsePhenoClicked()
{
    ui->probeToGene->deactivate();
    ui->parsePheno->activate();
    ui->pages->setCurrentIndex(1);
}
