#include "loading.h"
#include "ui_loading.h"

Loading::Loading(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Loading) {
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

Loading::~Loading() {
    delete ui;
}

void Loading::RetranslateUi() {
    ui->retranslateUi(this);
}
