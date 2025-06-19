#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDialogButtonBox>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    void SetTitle(const QString& title);
    void SetContent(const QString& content);
    QPushButton* AddOkButton(const QString& text = "Ok");
    QPushButton* AddCancelButton(const QString& text = "Cancel");

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
