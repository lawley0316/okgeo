#include "dialog.h"
#include "ui_dialog.h"

#include <QPushButton>
#include <QStyle>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    layout()->setSizeConstraint(QLayout::SetFixedSize);
    ui->closeButton->setFixedSize(QSize(20, 20));
    ui->closeButton->setIcon(QIcon(":/static/close-1.png"));
    ui->closeButton->setIconSize(QSize(14, 14));
    connect(ui->closeButton, &QToolButton::clicked, this, &Dialog::reject);
    ui->closeButton->installEventFilter(this);
}

void Dialog::setTitle(const QString& title)
{
    ui->titleLabel->setText(title);
}

void Dialog::setContent(const QString& content)
{
    ui->contentLabel->setText(content);
}

QPushButton* Dialog::addOkButton(const QString& text)
{
    QPushButton* button = ui->buttonBox->addButton(text, QDialogButtonBox::YesRole);
    connect(button, &QPushButton::clicked, this, &Dialog::accept);
    button->setProperty("ok-class", "button");
    button->setProperty("ok-type", "primary");
    button->setProperty("ok-size", "small");
    button->style()->unpolish(button);
    button->style()->polish(button);
    return button;
}

QPushButton* Dialog::addCancelButton(const QString& text)
{
    QPushButton* button = ui->buttonBox->addButton(text, QDialogButtonBox::NoRole);
    connect(button, &QPushButton::clicked, this, &Dialog::reject);
    button->setProperty("ok-class", "button");
    button->setProperty("ok-type", "secondary");
    button->setProperty("ok-size", "small");
    button->style()->unpolish(button);
    button->style()->polish(button);
    return button;
}

Dialog::~Dialog()
{
    delete ui;
}

bool Dialog::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == ui->closeButton) {
        if (event->type() == QEvent::Enter) {
            ui->closeButton->setIcon(QIcon(":/static/close-3.png"));
        } else if (event->type() == QEvent::Leave) {
            ui->closeButton->setIcon(QIcon(":/static/close-1.png"));
        }
    }
    return QDialog::eventFilter(watched, event);
}
