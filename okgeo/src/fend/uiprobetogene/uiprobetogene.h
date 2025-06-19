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
    bool IsValid() const;
    QJsonObject GetParams() const;
    void UpdateUi();

public slots:
    void BrowseProbeFile();
    void BrowseAnnoFile();
    void BrowseOutfile();
    void Convert();

private:
    Ui::UiProbeToGene *ui;
    QString previous_dir_;
};

#endif // UIPROBETOGENE_H
