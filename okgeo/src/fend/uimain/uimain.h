#ifndef UIMAIN_H
#define UIMAIN_H

#include "src/fend/uicom/loading.h"

#include <QDialog>

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
    void onLoading();
    void onErrorOccurred(const QString& detail);
    void onProbeToGeneClicked();
    void onParsePhenoClicked();
    void onConverted();

private:
    Ui::UiMain *ui;
    Loading* loading;
};

#endif // UIMAIN_H
