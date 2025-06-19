#ifndef UIPHENOTYPE_H
#define UIPHENOTYPE_H

#include "src/fend/uicom/loading.h"

#include <QWidget>

namespace Ui {
class UiPhenotype;
}

class UiPhenotype : public QWidget
{
    Q_OBJECT

public:
    explicit UiPhenotype(QWidget *parent = nullptr);
    ~UiPhenotype();
    bool IsValid() const;
    QJsonObject GetParams() const;
    void UpdateUi();

public slots:
    void BrowseSeriesMatrixFile();
    void BrowseOutfile();
    void Parse();

private:
    Ui::UiPhenotype *ui;
    QString previous_dir_;
};

#endif // UIPHENOTYPE_H
