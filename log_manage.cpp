#include "log_manage.h"
#include "ui_log_manage.h"
#include "connectionpool.h"
#include "export_excel.h"

#include <QMessageBox>

log_Manage::log_Manage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::log_Manage)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    QSqlDatabase db = ConnectionPool::openConnection();
    logModel = new QSqlTableModel(this,db);
    logModel->setTable("log");
    logModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    logModel->setHeaderData(0,Qt::Horizontal,"登陆编号");
    logModel->setHeaderData(1,Qt::Horizontal,"登陆姓名");
    logModel->setHeaderData(2,Qt::Horizontal,"登陆次数");
    logModel->setHeaderData(3,Qt::Horizontal,"最后登陆时间");
    logModel->setHeaderData(4,Qt::Horizontal,"最后登陆IP");
    logModel->setHeaderData(5,Qt::Horizontal,"登陆角色");
    logModel->select();

    ui->tableView->setModel(logModel);
    ui->tableView->show();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//选中整行
    //设置行高列宽根据内容自动调整
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();

    ConnectionPool::closeConnection(db);

}

log_Manage::~log_Manage()
{
    delete ui;
}

void log_Manage::on_pushButton_serch_clicked()
{
    //查询
    QString name = ui->lineEdit_name->text();
    QString ip = ui->lineEdit_IP->text();
    QString play = ui->lineEdit_play->text();

    if(name != NULL){
        logModel->setFilter(QString("log_name = '%1'").arg(name));
    }else if (play != NULL) {
        logModel->setFilter(QString("log_play = '%1'").arg(play));
    } else if (ip != NULL) {
        logModel->setFilter(QString("last_ip = '%1'").arg(ip));
    } else {
        QMessageBox::warning(this,"操作提示",
                             "请输入查询依据！",
                             QMessageBox::Ok);
    }

    logModel->select();
    ui->lineEdit_name->clear();
    ui->lineEdit_IP->clear();
    ui->lineEdit_play->clear();
}

void log_Manage::on_pushButton_showall_clicked()
{
    //showall
    logModel->setTable("log");
    logModel->setHeaderData(0,Qt::Horizontal,"登陆编号");
    logModel->setHeaderData(1,Qt::Horizontal,"登陆姓名");
    logModel->setHeaderData(2,Qt::Horizontal,"登陆次数");
    logModel->setHeaderData(3,Qt::Horizontal,"最后登陆时间");
    logModel->setHeaderData(4,Qt::Horizontal,"最后登陆IP");
    logModel->setHeaderData(5,Qt::Horizontal,"登陆角色");
    logModel->select();
}

void log_Manage::on_pushButton_out_2_clicked()
{
    //excel
    export_Excel::tableToExcel(ui->tableView,"登陆日志");
}
