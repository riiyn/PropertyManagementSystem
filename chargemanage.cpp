#include "chargemanage.h"
#include "ui_chargemanage.h"
#include "connectionpool.h"
#include "export_excel.h"

#include "QMessageBox"
#include<qsqlerror.h>

chargeManage::chargeManage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::chargeManage)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    QSqlDatabase db = ConnectionPool::openConnection();
    chargeModel = new QSqlTableModel(this,db);
    chargeModel->setTable("charge");
    chargeModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    chargeModel->setHeaderData(0,Qt::Horizontal,"收费项目编号");
    chargeModel->setHeaderData(1,Qt::Horizontal,"收费项目名称");
    chargeModel->setHeaderData(2,Qt::Horizontal,"收费标准");
    chargeModel->select();
    ui->tableView->setModel(chargeModel);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//选中整行
    //设置行高列宽根据内容自动调整
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();

    ConnectionPool::closeConnection(db);
}

chargeManage::~chargeManage()
{
    delete ui;
}

void chargeManage::on_pushButton_add_2_clicked()
{
    //添加
    int rowNum = chargeModel->rowCount();//获得表的行数
    chargeModel->insertRow(rowNum);//添加一行
}

void chargeManage::on_pushButton_update_2_clicked()
{
    //提交修改
    chargeModel->database().transaction(); //開啟事務操作

    if(chargeModel->submitAll()){
        chargeModel->database().commit(); //提交
        QMessageBox::information(this,"操作提示",
                                 "修改成功！");
    } else {
        chargeModel->database().rollback(); //回滾
        QMessageBox::warning(this,"操作提示",
                             tr("修改失败：")
                             .arg(chargeModel->lastError().text()),
                             QMessageBox::Ok);
    }
}

void chargeManage::on_pushButton_rollbackupdate_clicked()
{
    //撤销修改
    chargeModel->revertAll();
}

void chargeManage::on_pushButton_delete_2_clicked()
{
    //删除
    int currentRow = ui->tableView->currentIndex().row();

    if(currentRow >= 0){
        //删除该行
        chargeModel->removeRow(currentRow);

        int ok = QMessageBox::warning(this,"删除当前行！",
                                      "你确定删除当前行吗？",
                                      QMessageBox::Yes,
                                      QMessageBox::No);
        if(ok == QMessageBox::No){
            chargeModel->revertAll();//如果不删除，则撤销
        } else {
            chargeModel->submitAll();//否则删除该行，数据库中对应数据删除
        }
    } else {
        QMessageBox::warning(this,"操作提示！",
                             "请选择要删除的行！",
                             QMessageBox::Ok);
    }
}

void chargeManage::on_pushButton_out_2_clicked()
{
    //excel
    export_Excel::tableToExcel(ui->tableView,"收费项目信息");
}

void chargeManage::on_pushButton_serch_clicked()
{
    //查询
    QString name = ui->lineEdit_carnum->text();

    if(name != NULL){
        chargeModel->setFilter(QString("cha_name = '%1'").arg(name));
    }else {
        QMessageBox::warning(this,"操作提示",
                             "请输入查询依据！",
                             QMessageBox::Ok);
    }

    chargeModel->select();
    ui->lineEdit_carnum->clear();
}

void chargeManage::on_pushButton_showall_clicked()
{
    //显示全部
    chargeModel->setTable("charge");
    chargeModel->setHeaderData(0,Qt::Horizontal,"收费项目编号");
    chargeModel->setHeaderData(1,Qt::Horizontal,"收费项目名称");
    chargeModel->setHeaderData(2,Qt::Horizontal,"收费标准");
    chargeModel->select();

}
