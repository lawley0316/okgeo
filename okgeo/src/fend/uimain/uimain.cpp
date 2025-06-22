#include "uimain.h"
#include "ui_uimain.h"

#include <QTimer>
#include <QKeyEvent>
#include <QDesktopServices>

#include "src/middle/manager.h"
#include "src/middle/signals.h"
#include "src/middle/language.h"
#include "src/fend/uicom/dialog.h"
#include "src/fend/uiabout/uiabout.h"

UiMain::UiMain(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UiMain)
    , loading_(new Loading(this))
    , language_(QLocale::system().language()) {
    ui->setupUi(this);
    setWindowFlags(Qt::Window);

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
    ui->probe_to_gene->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(ui->probe_to_gene, &QToolButton::clicked, this, &UiMain::ConvertProbeToGene);

    // phenotype
    ui->phenotype->SetDefaultIcon(QIcon(":/static/phenotype-dark.png"));
    ui->phenotype->SetActiveIcon(QIcon(":/static/phenotype-light.png"));
    ui->phenotype->SetDefaultTextColor(QColor("#666666"));
    ui->phenotype->SetActiveTextColor(QColor("#455DD0"));
    ui->phenotype->setIconSize(QSize(25, 25));
    ui->phenotype->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(ui->phenotype, &QToolButton::clicked, this, &UiMain::ParsePhenotype);

    // language
    UpdateLanguageIcon();
    ui->language->setIconSize(QSize(25, 25));
    connect(ui->language, &QToolButton::clicked, this, &UiMain::ChangeLanguage);
    connect(MANAGER->lang, &Language::LanguageChanged, this, &UiMain::RetranslateUi);

    // about
    ui->about->SetDefaultIcon(QIcon(":/static/about-dark.png"));
    ui->about->SetActiveIcon(QIcon(":/static/about-light.png"));
    ui->about->setIconSize(QSize(25, 25));
    connect(ui->about, &QToolButton::clicked, this, &UiMain::About);

    ui->probe_to_gene->Activate();
    ui->pages->setCurrentIndex(0);

    // signals
    connect(MANAGER->sigs, &Signals::Loading, loading_, &QDialog::exec);
    connect(MANAGER->sigs, &Signals::ProbeToGeneConverted, this, &UiMain::OpenResultFile);
    connect(MANAGER->sigs, &Signals::PhenotypeParsed, this, &UiMain::OpenResultFile);
    connect(MANAGER->sigs, &Signals::ErrorOccurred, this, &UiMain::ShowError);
    RetranslateUi();
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
            dialog.SetTitle(tr("Finished"));
            dialog.SetContent(tr("Do you want to open the result file?"));
            dialog.AddOkButton(tr("OK"));
            dialog.AddCancelButton(tr("Cancel"));
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
            dialog.SetTitle(tr("Error occurred"));
            dialog.SetContent(message);
            dialog.AddOkButton(tr("OK"));
            dialog.exec();
        }
    );
}

void UiMain::ChangeLanguage() {
    UpdateLanguage();
    UpdateLanguageIcon();
    MANAGER->lang->SetLanguage(language_);
}

void UiMain::RetranslateUi() {
    ui->retranslateUi(this);
    loading_->RetranslateUi();
    ui->ui_probe_to_gene->RetranslateUi();
    ui->ui_phenotype->RetranslateUi();
    ui->probe_to_gene->setText(tr("Probe to Gene"));
    ui->probe_to_gene->setToolTip(tr(
        "Given a probe annotation file, the column containing gene symbols, "
        "and the method for merging probes, convert probe expression to gene expression."
    ));
    ui->phenotype->setText(tr("Phenotype"));
    ui->phenotype->setToolTip(tr("Parse the phenotype data from the Series Matrix file."));
    ui->about->setToolTip(tr("About"));
}

void UiMain::About() {
    UiAbout about;
    about.exec();
}

void UiMain::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) return;
    QDialog::keyPressEvent(event);
}

void UiMain::UpdateLanguage() {
    if (language_ == QLocale::Chinese) {
        language_ = QLocale::English;
    } else {
        language_ = QLocale::Chinese;
    }
}

void UiMain::UpdateLanguageIcon() {
    if (language_ == QLocale::Chinese) {
        ui->language->SetDefaultIcon(QIcon(":/static/en-dark.png"));
        ui->language->SetActiveIcon(QIcon(":/static/en-light.png"));
    } else {
        ui->language->SetDefaultIcon(QIcon(":/static/zh_CN-dark.png"));
        ui->language->SetActiveIcon(QIcon(":/static/zh_CN-light.png"));
    }
}
