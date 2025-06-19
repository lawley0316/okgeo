#include "dialog.h"
#include "ui_dialog.h"

#include <QPushButton>
#include <QStyle>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog) {
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    // close button
    ui->close->setFixedSize(QSize(20, 20));
    ui->close->setIcon(QIcon(":/static/close-1.png"));
    ui->close->setIconSize(QSize(14, 14));
    connect(ui->close, &QToolButton::clicked, this, &Dialog::reject);
    ui->close->installEventFilter(this);
}

void Dialog::SetTitle(const QString& title) {
    ui->title->setText(title);
}

void Dialog::SetContent(const QString& content) {
    ui->content->setText(content);
}

QPushButton* Dialog::AddOkButton(const QString& text) {
    QPushButton* button = ui->buttonBox->addButton(text, QDialogButtonBox::YesRole);
    connect(button, &QPushButton::clicked, this, &Dialog::accept);
    button->setProperty("ok-class", "button");
    button->setProperty("ok-type", "primary");
    button->setProperty("ok-size", "small");
    button->style()->unpolish(button);
    button->style()->polish(button);
    return button;
}

QPushButton* Dialog::AddCancelButton(const QString& text) {
    QPushButton* button = ui->buttonBox->addButton(text, QDialogButtonBox::NoRole);
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

bool Dialog::eventFilter(QObject* watched, QEvent* event) {
    if (watched == ui->close) {
        if (event->type() == QEvent::Enter) {
            ui->close->setIcon(QIcon(":/static/close-3.png"));
        } else if (event->type() == QEvent::Leave) {
            ui->close->setIcon(QIcon(":/static/close-1.png"));
        }
    }
    return QDialog::eventFilter(watched, event);
}
