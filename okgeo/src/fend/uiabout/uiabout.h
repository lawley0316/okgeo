#ifndef UIABOUT_H
#define UIABOUT_H

#include <QDialog>

namespace Ui {
class UiAbout;
}

class UiAbout : public QDialog
{
    Q_OBJECT

public:
    explicit UiAbout(QWidget *parent = nullptr);
    ~UiAbout();

public slots:
    void OpenDocs();

protected:
    bool eventFilter(QObject* watced, QEvent* event) override;

private:
    Ui::UiAbout *ui;
};

#endif // UIABOUT_H
