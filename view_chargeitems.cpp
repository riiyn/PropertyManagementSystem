#include "view_chargeitems.h"
#include "ui_view_chargeitems.h"
#include "connectionpool.h"
#include "export_excel.h"

#include <QMessageBox>

view_ChargeItems::view_ChargeItems(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::view_ChargeItems)
{
    ui->setupUi(this);

    QSqlDatabase db = ConnectionPool::openConnection();
    chargeitemsModel = new QSqlTableModel(this,db);
    chargeitemsModel->setTable("charge");
    chargeitemsModel->setHeaderData(0,Qt::Horizontal,"收费项目编号");
    chargeitemsModel->setHeaderData(1,Qt::Horizontal,"收费项目名称");
    chargeitemsModel->setHeaderData(2,Qt::Horizontal,"收费标准（元/月）");
    chargeitemsModel->select();

    ui->tableView->setModel(chargeitemsModel);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置表格不可编辑
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//选中整行
    //设置行高列宽根据内容自动调整
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();

    ConnectionPool::closeConnection(db);
}

view_ChargeItems::~view_ChargeItems()
{
    delete ui;
}

void view_ChargeItems::on_pushButton_serch_clicked()
{
    //查询
    QString name = ui->lineEdit_name->text();

    if(name != NULL){
        chargeitemsModel->setFilter(QString("cha_name = '%1'").arg(name));
    }else {
        QMessageBox::warning(this,"操作提示",
                             "请输入查询依据！",
                             QMessageBox::Ok);
    }

    chargeitemsModel->select();
    ui->lineEdit_name->clear();
}

void view_ChargeItems::on_pushButton_showall_clicked()
{
    //全部
    chargeitemsModel->setTable("charge");
    chargeitemsModel->setHeaderData(0,Qt::Horizontal,"收费项目编号");
    chargeitemsModel->setHeaderData(1,Qt::Horizontal,"收费项目名称");
    chargeitemsModel->setHeaderData(2,Qt::Horizontal,"收费标准（元/月）");
    chargeitemsModel->select();

}

void view_ChargeItems::on_pushButton_out_2_clicked()
{
    //excel
    export_Excel::tableToExcel(ui->tableView,"收费项目");
}
