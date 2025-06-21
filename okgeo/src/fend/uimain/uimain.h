#ifndef UIMAIN_H
#define UIMAIN_H

#include "src/fend/uicom/loading.h"

#include <QDialog>
#include <QLocale>

namespace Ui {
class UiMain;
}

class UiMain : public QDialog
{
    Q_OBJECT

public:
    explicit UiMain(QWidget *parent = nullptr);
    ~UiMain();

public slots:
    void ConvertProbeToGene();
    void ParsePhenotype();
    void OpenResultFile(const QString& path);
    void ShowError(int api, const QJsonValue& params, const QString& message);
    void ChangeLanguage();
    void RetranslateUi();
    void About();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    void UpdateLanguage();
    void UpdateLanguageIcon();

private:
    Ui::UiMain *ui;
    Loading* loading_;
    QLocale::Language language_;
};

#endif // UIMAIN_H
