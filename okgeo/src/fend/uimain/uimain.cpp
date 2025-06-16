#include "uimain.h"
#include "ui_uimain.h"
#include "src/fend/uicom/dialog.h"
#include "src/middle/manager.h"
#include "src/middle/signals.h"

#include <QTimer>
#include <QDesktopServices>

UiMain::UiMain(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UiMain)
    , mLoading(new Loading(this))
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
    connect(ui->probeToGene, &QToolButton::clicked, this, &UiMain::ConvertProbeToGene);

    // phenotype
    ui->phenotype->setDefaultIcon(QIcon(":/static/phenotype-dark.png"));
    ui->phenotype->setActiveIcon(QIcon(":/static/phenotype-light.png"));
    ui->phenotype->setDefaultTextColor(QColor("#666666"));
    ui->phenotype->setActiveTextColor(QColor("#455DD0"));
    ui->phenotype->setIconSize(QSize(25, 25));
    ui->phenotype->setText(tr("Phenotype"));
    ui->phenotype->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->phenotype->setToolTip(tr("Parse the phenotype data from the Series Matrix file."));
    connect(ui->phenotype, &QToolButton::clicked, this, &UiMain::ParsePhenotype);

    // about
    ui->about->setDefaultIcon(QIcon(":/static/about-dark.png"));
    ui->about->setActiveIcon(QIcon(":/static/about-light.png"));
    ui->about->setIconSize(QSize(25, 25));
    ui->about->setToolTip(tr("About"));

    ui->probeToGene->activate();
    ui->pages->setCurrentIndex(0);

    // signals
    connect(MANAGER->mSignals, &Signals::Loading, mLoading, &QDialog::exec);
    connect(MANAGER->mSignals, &Signals::ProbeToGeneConverted, this, &UiMain::OpenResultFile);
    connect(MANAGER->mSignals, &Signals::PhenotypeParsed, this, &UiMain::OpenResultFile);
    connect(MANAGER->mSignals, &Signals::ErrorOccurred, this, &UiMain::ShowError);
}

UiMain::~UiMain()
{
    delete ui;
    delete mLoading;
}

void UiMain::ConvertProbeToGene()
{
    ui->probeToGene->activate();
    ui->phenotype->deactivate();
    ui->pages->setCurrentIndex(0);
}

void UiMain::ParsePhenotype()
{
    ui->probeToGene->deactivate();
    ui->phenotype->activate();
    ui->pages->setCurrentIndex(1);
}

void UiMain::OpenResultFile(const QString& path)
{
    mLoading->accept();
    QTimer::singleShot(
        0, this,
        [=]()
        {
            Dialog dialog(this);
            dialog.setTitle(tr("Finished."));
            dialog.setContent(tr("Do you want to open the result file?"));
            dialog.addOkButton();
            dialog.addCancelButton();
            if (dialog.exec() == Dialog::Accepted)
            {
                QDesktopServices::openUrl(QUrl::fromLocalFile(path));
            }
        }
    );
}

void UiMain::ShowError(int api, const QJsonValue& params, const QString& message)
{
    mLoading->accept();
    QTimer::singleShot(
        0, this,
        [=]()
        {
            Dialog dialog(this);
            dialog.setTitle(tr("Error occurred."));
            dialog.setContent(message);
            dialog.addOkButton();
            dialog.exec();
        }
    );
}
