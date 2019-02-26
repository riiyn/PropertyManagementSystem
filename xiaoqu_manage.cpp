#include "xiaoqu_manage.h"
#include "ui_xiaoqu_manage.h"
#include "connectionpool.h"

#include <QMessageBox>
#include <QSqlError>

xiaoqu_Manage::xiaoqu_Manage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::xiaoqu_Manage)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);

    query.exec("select * from xiaoqu");
    query.next();
    ui->lineEdit_xqname->setText(query.value(1).toString().trimmed());
    ui->lineEdit_name->setText(query.value(2).toString().trimmed());
    ui->lineEdit_sex->setText(query.value(3).toString().trimmed());
    ui->lineEdit_tel->setText(query.value(4).toString().trimmed());
    ui->lineEdit_username->setText(query.value(5).toString().trimmed());
    ui->lineEdit_play->setText(query.value(7).toString().trimmed());
    ui->lineEdit_lasttime->setText(query.value(10).toString().trimmed());
    ui->textEdit->setText(query.value(8).toString().trimmed());
    ui->lineEdit_kftel->setText(query.value(9).toString().trimmed());

    ConnectionPool::closeConnection(db);
}

xiaoqu_Manage::~xiaoqu_Manage()
{
    delete ui;

}

void xiaoqu_Manage::on_pushButton_update_clicked()
{
    //提交修改
    QString kftel = ui->lineEdit_kftel->text();
    QString xqname = ui->lineEdit_xqname->text();
    QString name = ui->lineEdit_name->text();
    QString sex = ui->lineEdit_sex->text();
    QString tel = ui->lineEdit_tel->text();
    QString username = ui->lineEdit_username->text();
    QString acl = ui->lineEdit_play->text();
    QString xqtext = ui->textEdit->toPlainText();

    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    query.exec(tr("update xiaoqu set xqname = '%1',name = '%2',sex = '%3',tel = '%4',username = '%5',acl = '%6',xqtext = '%7',kftel = '%8'")
               .arg(xqname).arg(name).arg(sex).arg(tel).arg(username).arg(acl).arg(xqtext).arg(kftel));

    ConnectionPool::closeConnection(db);
}

