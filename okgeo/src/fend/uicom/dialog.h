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
    void setTitle(const QString& title);
    void setContent(const QString& content);
    QPushButton* addOkButton(const QString& text = "Ok");
    QPushButton* addCancelButton(const QString& text = "Cancel");

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
