#ifndef UIPHENOTYPE_H
#define UIPHENOTYPE_H

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
    bool isValid() const;
    QJsonObject getParams() const;
    void updateUi();
    void onBrowseSeriesMatrixFileClicked();
    void onBrowseOutfileClicked();
    void onParseClicked();

private:
    Ui::UiPhenotype *ui;
    QString previousDir;
};

#endif // UIPHENOTYPE_H
