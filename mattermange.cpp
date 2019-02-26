#include "mattermange.h"
#include "ui_mattermange.h"
#include "connectionpool.h"
#include "export_excel.h"

#include <QMessageBox>
#include <QSqlError>

matterMange::matterMange(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::matterMange)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    QSqlDatabase db = ConnectionPool::openConnection();

    matterModel = new QSqlTableModel(this,db);
    matterModel->setTable("matter");
    matterModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    matterModel->setHeaderData(0,Qt::Horizontal,"设备编号");
    matterModel->setHeaderData(1,Qt::Horizontal,"设备名称");
    matterModel->setHeaderData(2,Qt::Horizontal,"设备数量");
    matterModel->setHeaderData(3,Qt::Horizontal,"设备价格");
    matterModel->setHeaderData(4,Qt::Horizontal,"入库时间");
    matterModel->setHeaderData(5,Qt::Horizontal,"出库时间");
    matterModel->setHeaderData(6,Qt::Horizontal,"设备状态");
    matterModel->select();

    ui->tableView->setModel(matterModel);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//选中整行
    //设置行高列宽根据内容自动调整
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();

    ConnectionPool::closeConnection(db);
}

matterMange::~matterMange()
{
    delete ui;
}

void matterMange::on_pushButton_add_2_clicked()
{
    //添加
    int rowNum = matterModel->rowCount();//获得表的行数
    matterModel->insertRow(rowNum);//添加一行
}

void matterMange::on_pushButton_update_2_clicked()
{
    //提交修改
    matterModel->database().transaction(); //開啟事務操作

    if(matterModel->submitAll()){
        matterModel->database().commit(); //提交
        QMessageBox::information(this,"操作提示",
                                 "修改成功！");
    } else {
        matterModel->database().rollback(); //回滾
        QMessageBox::warning(this,"操作提示",
                             tr("修改失败：")
                             .arg(matterModel->lastError().text()),
                             QMessageBox::Ok);
    }
}

void matterMange::on_pushButton_rollbackupdate_clicked()
{
    //撤销修改
    matterModel->revertAll();
}

void matterMange::on_pushButton_delete_2_clicked()
{
    //删除
    int currentRow = ui->tableView->currentIndex().row();

    if(currentRow >= 0){
        //删除该行
        matterModel->removeRow(currentRow);

        int ok = QMessageBox::warning(this,"删除当前行！",
                                      "你确定删除当前行吗？",
                                      QMessageBox::Yes,
                                      QMessageBox::No);
        if(ok == QMessageBox::No){
            matterModel->revertAll();//如果不删除，则撤销
        } else {
            matterModel->submitAll();//否则删除该行，数据库中对应数据删除
        }
    } else {
        QMessageBox::warning(this,"操作提示！",
                             "请选择要删除的行！",
                             QMessageBox::Ok);
    }
}

void matterMange::on_pushButton_out_2_clicked()
{
    //导出excel
    export_Excel::tableToExcel(ui->tableView,"设备管理信息");
}

void matterMange::on_pushButton_serch_clicked()
{
    //查询
    QString name = ui->lineEdit_carnum->text();

    if(name != NULL){
        matterModel->setFilter(QString("mname = '%1'").arg(name));
    }else {
        QMessageBox::warning(this,"操作提示",
                             "请输入查询依据！",
                             QMessageBox::Ok);
    }

    matterModel->select();
    ui->lineEdit_carnum->clear();
}

void matterMange::on_pushButton_showall_clicked()
{
    //显示全部
    matterModel->setTable("matter");
    matterModel->setHeaderData(0,Qt::Horizontal,"设备编号");
    matterModel->setHeaderData(1,Qt::Horizontal,"设备名称");
    matterModel->setHeaderData(2,Qt::Horizontal,"设备数量");
    matterModel->setHeaderData(3,Qt::Horizontal,"设备价格");
    matterModel->setHeaderData(4,Qt::Horizontal,"入库时间");
    matterModel->setHeaderData(5,Qt::Horizontal,"出库时间");
    matterModel->setHeaderData(6,Qt::Horizontal,"设备状态");
    matterModel->select();

}
