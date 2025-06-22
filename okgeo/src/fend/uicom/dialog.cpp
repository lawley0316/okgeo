#include "dialog.h"
#include "ui_dialog.h"

#include <QPushButton>
#include <QStyle>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog) {
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog);
}

void Dialog::SetTitle(const QString& title) {
    setWindowTitle(title);
}

void Dialog::SetContent(const QString& content) {
    ui->content->setText(content);
}

QPushButton* Dialog::AddOkButton(const QString& text) {
    QPushButton* button = ui->button_box->addButton(text, QDialogButtonBox::YesRole);
    connect(button, &QPushButton::clicked, this, &Dialog::accept);
    button->setProperty("ok-class", "button");
    button->setProperty("ok-type", "primary");
    button->setProperty("ok-size", "small");
    button->style()->unpolish(button);
    button->style()->polish(button);
    return button;
}

QPushButton* Dialog::AddCancelButton(const QString& text) {
    QPushButton* button = ui->button_box->addButton(text, QDialogButtonBox::NoRole);
    connect(button, &QPushButton::clicked, this, &Dialog::reject);
    button->setProperty("ok-class", "button");
    button->setProperty("ok-type", "secondary");
    button->setProperty("ok-size", "small");
    button->style()->unpolish(button);
    button->style()->polish(button);
    return button;
}

Dialog::~Dialog() {
    delete ui;
}
