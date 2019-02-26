#include "pay_manage.h"
#include "ui_pay_manage.h"
#include "connectionpool.h"
#include "export_excel.h"

#include "QMessageBox"

pay_Manage::pay_Manage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::pay_Manage)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    QSqlDatabase db = ConnectionPool::openConnection();
    payModel = new QSqlRelationalTableModel(this,db);
    payModel->setTable("pay");
    payModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    payModel->setRelation(6,QSqlRelation("user","uid","realname"));
    payModel->setHeaderData(0,Qt::Horizontal,"支付编号");
    payModel->setHeaderData(1,Qt::Horizontal,"收费项目");
    payModel->setHeaderData(2,Qt::Horizontal,"收费标准（元）");
    payModel->setHeaderData(3,Qt::Horizontal,"实际支付（元）");
    payModel->setHeaderData(4,Qt::Horizontal,"欠费金额（元）");
    payModel->setHeaderData(5,Qt::Horizontal,"支付时间");
    payModel->setHeaderData(6,Qt::Horizontal,"用户姓名");
    payModel->setHeaderData(7,Qt::Horizontal,"支付状态");
    payModel->setHeaderData(8,Qt::Horizontal,"支付月份");
    payModel->select();

    ui->tableView->setModel(payModel);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//选中整行
    //设置行高列宽根据内容自动调整
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();

    ConnectionPool::closeConnection(db);

}

pay_Manage::~pay_Manage()
{
    delete ui;
}

void pay_Manage::on_pushButton_search_clicked()
{
    //查询
    QString name = ui->lineEdit_carnum->text();

    if(name != NULL){
        payModel->setFilter(QString("realname = '%1'").arg(name));
    }else {
        QMessageBox::warning(this,"操作提示",
                             "请输入查询依据！",
                             QMessageBox::Ok);
    }

    payModel->select();
    ui->lineEdit_carnum->clear();
}

void pay_Manage::on_pushButton_showall_clicked()
{
    //showall
    payModel->setTable("pay");
    payModel->setRelation(6,QSqlRelation("user","uid","realname"));
    payModel->setHeaderData(0,Qt::Horizontal,"支付编号");
    payModel->setHeaderData(1,Qt::Horizontal,"收费项目");
    payModel->setHeaderData(2,Qt::Horizontal,"收费标准（元）");
    payModel->setHeaderData(3,Qt::Horizontal,"实际支付（元）");
    payModel->setHeaderData(4,Qt::Horizontal,"欠费金额（元）");
    payModel->setHeaderData(5,Qt::Horizontal,"支付时间");
    payModel->setHeaderData(6,Qt::Horizontal,"用户姓名");
    payModel->setHeaderData(7,Qt::Horizontal,"支付状态");
    payModel->setHeaderData(8,Qt::Horizontal,"支付月份");
    payModel->select();

}

void pay_Manage::on_pushButton_out_clicked()
{
    //excel
    export_Excel::tableToExcel(ui->tableView,"用户缴费支付信息");
}
