#include "carinfo_user.h"
#include "ui_carinfo_user.h"
#include "connectionpool.h"
#include "export_excel.h"

#include <QMessageBox>
#include <QSqlError>
#include <QDebug>
QString realname;
carInfo_User::carInfo_User(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::carInfo_User)
{
    ui->setupUi(this);

    QSqlDatabase db = ConnectionPool::openConnection();
    carInfoMOdel = new QSqlTableModel(this,db);
    carInfoMOdel->setTable("car");
    carInfoMOdel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    carInfoMOdel->setHeaderData(1,Qt::Horizontal,"所属用户");
    carInfoMOdel->setHeaderData(2,Qt::Horizontal,"车辆名称");
    carInfoMOdel->setHeaderData(3,Qt::Horizontal,"车辆牌号");
    carInfoMOdel->setHeaderData(4,Qt::Horizontal,"停车位");
    carInfoMOdel->setFilter(tr("realname = '%1'").arg(realname));
    qDebug() << "****" << realname;
    carInfoMOdel->select();

    ui->tableView->setModel(carInfoMOdel);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//选中整行
    //设置行高列宽根据内容自动调整
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();

    ConnectionPool::closeConnection(db);
}

carInfo_User::~carInfo_User()
{
    delete ui;
}

void carInfo_User::on_pushButton_add_2_clicked()
{
    //添加
    int rowCount = carInfoMOdel->rowCount();
    carInfoMOdel->insertRow(rowCount);
}

void carInfo_User::on_pushButton_update_2_clicked()
{
    //提交修改
    carInfoMOdel->database().transaction();
    if(carInfoMOdel->submitAll()){
        carInfoMOdel->database().commit();
    } else {
        carInfoMOdel->database().rollback();
        QMessageBox::warning(this,"操作提示！",
                             tr("数据库错误：%1")
                             .arg(carInfoMOdel->lastError().text()));
    }
}

void carInfo_User::on_pushButton_rollbackupdate_clicked()
{
    //撤销修改
    carInfoMOdel->revertAll();
}

void carInfo_User::on_pushButton_delete_2_clicked()
{
    //删除
    //获取选中行
    int currentRow = ui->tableView->currentIndex().row();
    if(currentRow >= 0){
        //删除
        carInfoMOdel->removeRow(currentRow);

        int ok = QMessageBox::warning(this,"删除当前行！",
                                      "你确定删除当前行吗？",
                                      QMessageBox::Yes,
                                      QMessageBox::No);
        if(ok == QMessageBox::No){
            carInfoMOdel->revertAll();
        }else {
            carInfoMOdel->submitAll();
        }
    } else {
        QMessageBox::warning(this,"操作提示！",
                             "请选择要删除的行！",
                             QMessageBox::Ok);
    }
}

void carInfo_User::on_pushButton_out_2_clicked()
{
    //excel
    export_Excel::tableToExcel(ui->tableView,"车辆信息");
}

void carInfo_User::on_pushButton_serch_clicked()
{
    //查询
    QString name = ui->lineEdit_name->text();
    QString carnum = ui->lineEdit_carnum->text();

    if(name != NULL){
        carInfoMOdel->setFilter(tr("realname = '%1'").arg(name));
    } else if (carnum != NULL) {
        carInfoMOdel->setFilter(tr("cnum = '%1'").arg(carnum));
    } else {
        QMessageBox::warning(this,"操作提示！",
                             "请输入查询依据！",
                             QMessageBox::Ok);
    }
    carInfoMOdel->select();
    ui->lineEdit_carnum->clear();
    ui->lineEdit_name->clear();
}

void carInfo_User::on_pushButton_showall_clicked()
{
    //showall
    carInfoMOdel->setTable("car");
    carInfoMOdel->setHeaderData(1,Qt::Horizontal,"所属用户");
    carInfoMOdel->setHeaderData(2,Qt::Horizontal,"车辆名称");
    carInfoMOdel->setHeaderData(3,Qt::Horizontal,"车辆牌号");
    carInfoMOdel->setHeaderData(4,Qt::Horizontal,"停车位");
    carInfoMOdel->setFilter(tr("realname = '%1'").arg(realname));
    qDebug() << "+++++" << realname;
    carInfoMOdel->select();
    ui->tableView->setColumnHidden(0,true);

}

void carInfo_User::receiveUserInfo(QString name)
{
    realname = name;
}

