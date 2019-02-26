#include "kftel_dialog.h"
#include "ui_kftel_dialog.h"
#include "connectionpool.h"

#include <QMessageBox>
#include <QSqlError>

kfTel_Dialog::kfTel_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::kfTel_Dialog)
{
    ui->setupUi(this);

    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    query.exec("select *from xiaoqu");
    if(query.next()){
        ui->label_kftel->setText(query.value(9).toString().trimmed());
        ui->label_fze->setText(query.value(2).toString().trimmed());
        ui->label_fzrtel->setText(query.value(4).toString().trimmed());
    }else {
        QMessageBox::warning(this,"错误！",
                             tr("数据库错误：%1").arg(query.lastError().text()));
    }
    ConnectionPool::closeConnection(db);
}

kfTel_Dialog::~kfTel_Dialog()
{
    delete ui;
}
