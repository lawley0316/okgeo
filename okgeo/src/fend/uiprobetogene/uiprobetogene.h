#ifndef UIPROBETOGENE_H
#define UIPROBETOGENE_H

#include <QString>
#include <QWidget>
#include <QJsonObject>

namespace Ui {
class UiProbeToGene;
}

class UiProbeToGene : public QWidget
{
    Q_OBJECT

public:
    explicit UiProbeToGene(QWidget *parent = nullptr);
    ~UiProbeToGene();
    bool isValid() const;
    QJsonObject getParams() const;
    void updateUi();
    void disableUi();
    void enableUi();

public slots:
    void onBrowseProbeFileClicked();
    void onBrowseAnnoFileClicked();
    void onBrowseOutfileClicked();
    void onConvertClicked();
    void onConverted();

private:
    Ui::UiProbeToGene *ui;
    QString previousDir;
};

#endif // UIPROBETOGENE_H
