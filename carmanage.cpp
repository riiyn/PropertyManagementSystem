#include "carmanage.h"
#include "ui_carmanage.h"
#include "connectionpool.h"
#include "export_excel.h"

#include <QMessageBox>
#include <QSqlError>
#include <QSqlRelationalDelegate>

carManage::carManage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::carManage)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    QSqlDatabase db = ConnectionPool::openConnection();

    carModel = new QSqlRelationalTableModel(this,db);
    carModel->setTable("car");
    carModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

//    carModel->setRelation(1,QSqlRelation("user","uid","realname"));;
    carModel->setHeaderData(1,Qt::Horizontal,"用户姓名");
    carModel->setHeaderData(2,Qt::Horizontal,"车辆名称");
    carModel->setHeaderData(3,Qt::Horizontal,"车辆牌号");
    carModel->setHeaderData(4,Qt::Horizontal,"停车位");

    carModel->select();
    ui->tableView->setModel(carModel);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//选中整行
    //设置行高列宽根据内容自动调整
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));

    ConnectionPool::closeConnection(db);



}

carManage::~carManage()
{
    delete ui;
}

void carManage::on_pushButton_add_2_clicked()
{
    //添加数据

    int rowCount = carModel->rowCount();
    carModel->insertRow(rowCount);
}

void carManage::on_pushButton_rollbackupdate_clicked()
{
    //撤销修改
    carModel->revertAll();
}

void carManage::on_pushButton_update_2_clicked()
{
    //提交修改
    carModel->database().transaction();
    if(carModel->submitAll()){
        carModel->database().commit();
    } else {
        carModel->database().rollback();
        QMessageBox::warning(this,"操作提示！",
                             tr("数据库错误：%1")
                             .arg(carModel->lastError().text()));
    }
}

void carManage::on_pushButton_serch_clicked()
{
    //查询
    QString name = ui->lineEdit_name->text();
    QString carnum = ui->lineEdit_carnum->text();

    if(name != NULL){
        carModel->setFilter(tr("realname = '%1'").arg(name));
    } else if (carnum != NULL) {
        carModel->setFilter(tr("cnum = '%1'").arg(carnum));
    } else {
        QMessageBox::warning(this,"操作提示！",
                             "请输入查询依据！",
                             QMessageBox::Ok);
    }
    carModel->select();
    ui->lineEdit_carnum->clear();
    ui->lineEdit_name->clear();
}

void carManage::on_pushButton_showall_clicked()
{
    //显示全部
    carModel->setTable("car");
    carModel->select();
    carModel->setHeaderData(1,Qt::Horizontal,"用户姓名");
    carModel->setHeaderData(2,Qt::Horizontal,"车辆名称");
    carModel->setHeaderData(3,Qt::Horizontal,"车辆牌号");
    carModel->setHeaderData(4,Qt::Horizontal,"停车位");
    ui->tableView->setColumnHidden(0,true);
}

void carManage::on_pushButton_delete_2_clicked()
{
    //获取选中行
    int currentRow = ui->tableView->currentIndex().row();
    if(currentRow >= 0){
        //删除
        carModel->removeRow(currentRow);

        int ok = QMessageBox::warning(this,"删除当前行！",
                                      "你确定删除当前行吗？",
                                      QMessageBox::Yes,
                                      QMessageBox::No);
        if(ok == QMessageBox::No){
            carModel->revertAll();
        }else {
            carModel->submitAll();
        }
    } else {
        QMessageBox::warning(this,"操作提示！",
                             "请选择要删除的行！",
                             QMessageBox::Ok);
    }

}

void carManage::on_pushButton_out_2_clicked()
{
    //导出excel
    export_Excel::tableToExcel(ui->tableView,"车辆数据(carsData)");
}
