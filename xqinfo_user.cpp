#include "xqinfo_user.h"
#include "ui_xqinfo_user.h"
#include "connectionpool.h"

#include <QMessageBox>
#include <QSqlError>

xqInfo_User::xqInfo_User(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::xqInfo_User)
{
    ui->setupUi(this);

    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    query.exec("select *from xiaoqu");
    if(query.next()){
        ui->lineEdit_xqname->setText(query.value(1).toString().trimmed());
        ui->lineEdit_fzrname->setText(query.value(2).toString().trimmed());
        ui->lineEdit_fzrzw->setText(query.value(7).toString().trimmed());
        ui->lineEdit_sex->setText(query.value(3).toString().trimmed());
        ui->lineEdit_tel->setText(query.value(4).toString().trimmed());
        ui->textBrowser->setText(query.value(8).toString().trimmed());
    }else {
        QMessageBox::warning(this,"错误！",
                             tr("数据库错误：%1").arg(query.lastError().text()));
    }

    ConnectionPool::closeConnection(db);
}

xqInfo_User::~xqInfo_User()
{
    delete ui;
}
