#include "housemanage.h"
#include "ui_housemanage.h"
#include "connectionpool.h"
#include "export_excel.h"

#include <QMessageBox>
#include <QSqlError>

houseManage::houseManage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::houseManage)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    QSqlDatabase db = ConnectionPool::openConnection();
    houseModel = new QSqlTableModel(this,db);
    houseModel->setTable("house");

    houseModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    houseModel->setHeaderData(0,Qt::Horizontal,"楼宇编号");
    houseModel->setHeaderData(1,Qt::Horizontal,"楼宇名称");
    houseModel->setHeaderData(2,Qt::Horizontal,"开工时间");
    houseModel->setHeaderData(3,Qt::Horizontal,"竣工时间");
    houseModel->setHeaderData(4,Qt::Horizontal,"楼宇面积");
    houseModel->select();//选取整个表的所有行

    ui->tableView->setModel(houseModel);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置行高列宽根据内容自动调整
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();

    ConnectionPool::closeConnection(db);
}

houseManage::~houseManage()
{
    delete ui;
}

void houseManage::on_pushButton_add_2_clicked()
{
    //添加
    int rowNum = houseModel->rowCount();//获得表的行数
    houseModel->insertRow(rowNum);//添加一行
}

void houseManage::on_pushButton_update_2_clicked()
{
    //提交修改
    houseModel->database().transaction(); //開啟事務操作

    if(houseModel->submitAll()){
        houseModel->database().commit(); //提交
        QMessageBox::information(this,"操作提示",
                                 "修改成功！");
    } else {
        houseModel->database().rollback(); //回滾
        QMessageBox::warning(this,"操作提示",
                             tr("修改失败：")
                             .arg(houseModel->lastError().text()),
                             QMessageBox::Ok);
    }
}

void houseManage::on_pushButton_rollbackupdate_clicked()
{
    //撤销修改
    houseModel->revertAll();
}

void houseManage::on_pushButton_delete_2_clicked()
{
    //删除
    //获取选中的行
    int currentRow = ui->tableView->currentIndex().row();
//    qDebug() << currentRow;
    if(currentRow >= 0){
        //删除该行
        houseModel->removeRow(currentRow);

        int ok = QMessageBox::warning(this,"删除当前行！",
                                      "你确定删除当前行吗？",
                                      QMessageBox::Yes,
                                      QMessageBox::No);
        if(ok == QMessageBox::No){
            houseModel->revertAll();//如果不删除，则撤销
        } else {
            houseModel->submitAll();//否则删除该行，数据库中对应数据删除
        }
    } else {
        QMessageBox::warning(this,"操作提示！",
                             "请选择要删除的行！",
                             QMessageBox::Ok);
    }
}

void houseManage::on_pushButton_showall_clicked()
{
    //显示全部
    houseModel->setTable("house");
    houseModel->select();
    houseModel->setHeaderData(0,Qt::Horizontal,"楼宇编号");
    houseModel->setHeaderData(1,Qt::Horizontal,"楼宇名称");
    houseModel->setHeaderData(2,Qt::Horizontal,"开工时间");
    houseModel->setHeaderData(3,Qt::Horizontal,"竣工时间");
    houseModel->setHeaderData(4,Qt::Horizontal,"楼宇面积");
}

void houseManage::on_pushButton_serch_2_clicked()
{
    //查询
    QString name = ui->lineEdit_housname->text();

    if(name != NULL){
        houseModel->setFilter(QString("hname = '%1'").arg(name));
    } else {
        QMessageBox::warning(this,"操作提示",
                             "请输入查询依据！",
                             QMessageBox::Ok);
    }

    houseModel->select();
    ui->lineEdit_housname->clear();
}

void houseManage::on_pushButton_out_2_clicked()
{
    //excel
    export_Excel::tableToExcel(ui->tableView,"楼宇信息");
}
