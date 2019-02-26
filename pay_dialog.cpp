#include "pay_dialog.h"
#include "ui_pay_dialog.h"

pay_Dialog::pay_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pay_Dialog)
{
    ui->setupUi(this);
}

pay_Dialog::~pay_Dialog()
{
    delete ui;
}
