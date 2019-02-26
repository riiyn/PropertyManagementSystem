#include "userfamilyinfo.h"
#include "ui_userfamilyinfo.h"
#include "connectionpool.h"
#include "export_excel.h"

#include <QMessageBox>
#include <QSqlError>

int ufi;
userFamilyInfo::userFamilyInfo(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::userFamilyInfo)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    QSqlDatabase db = ConnectionPool::openConnection();
    userfamModel = new QSqlTableModel(this,db);
    userfamModel->setTable("user_family");
    userfamModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    userfamModel->setHeaderData(0,Qt::Horizontal,"家属编号");
    userfamModel->setHeaderData(1,Qt::Horizontal,"家属姓名");
    userfamModel->setHeaderData(2,Qt::Horizontal,"家属性别");
    userfamModel->setHeaderData(3,Qt::Horizontal,"家属联系方式");
    userfamModel->setHeaderData(4,Qt::Horizontal,"与业主关系");
    userfamModel->setHeaderData(5,Qt::Horizontal,"业主ID");
    userfamModel->setFilter(tr("uid = %1").arg(ufi));
    userfamModel->select();

    ui->tableView->setModel(userfamModel);
    ui->tableView->show();
//    ui->tableView->setColumnHidden(5,true);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置行高列宽根据内容自动调整
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();

    ConnectionPool::closeConnection(db);
}

userFamilyInfo::~userFamilyInfo()
{
    delete ui;
}

void userFamilyInfo::on_pushButton_add_2_clicked()
{
    //添加
    int rowNum = userfamModel->rowCount();//获得表的行数
    userfamModel->insertRow(rowNum);//添加一行
}

void userFamilyInfo::on_pushButton_update_2_clicked()
{
    //修改
    userfamModel->database().transaction(); //開啟事務操作

    if(userfamModel->submitAll()){
        userfamModel->database().commit(); //提交
        QMessageBox::information(this,"操作提示",
                                 "修改成功！");
    } else {
        userfamModel->database().rollback(); //回滾
        QMessageBox::warning(this,"操作提示",
                             tr("修改失败：")
                             .arg(userfamModel->lastError().text()),
                             QMessageBox::Ok);
    }
}

void userFamilyInfo::on_pushButton_rollbackupdate_clicked()
{
    //撤销修改
    userfamModel->revertAll();
}

void userFamilyInfo::on_pushButton_delete_2_clicked()
{
    //删除
    //获取选中的行
    int currentRow = ui->tableView->currentIndex().row();

    if(currentRow >= 0){
        //删除该行
        userfamModel->removeRow(currentRow);

        int ok = QMessageBox::warning(this,"删除当前行！",
                                      "你确定删除该家属吗？",
                                      QMessageBox::Yes,
                                      QMessageBox::No);
        if(ok == QMessageBox::No){
            userfamModel->revertAll();//如果不删除，则撤销
        } else {
            userfamModel->submitAll();//否则删除该行，数据库中对应数据删除
        }
    } else {
        QMessageBox::warning(this,"操作提示！",
                             "请选择要删除的行！",
                             QMessageBox::Ok);
    }
}

void userFamilyInfo::on_pushButton_out_2_clicked()
{
    //excel
    export_Excel::tableToExcel(ui->tableView,"我的家属信息");
}

void userFamilyInfo::on_pushButton_serch_clicked()
{
    //查询
    QString name = ui->lineEdit_name->text();

    if(name != NULL){
        userfamModel->setFilter(QString("ufname = '%1'").arg(name));
    }else {
        QMessageBox::warning(this,"操作提示",
                             "请输入查询依据！",
                             QMessageBox::Ok);
    }

    userfamModel->select();
    ui->lineEdit_name->clear();
}

void userFamilyInfo::on_pushButton_showall_clicked()
{
    //showall
    userfamModel->setTable("user_family");
    userfamModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    userfamModel->setHeaderData(0,Qt::Horizontal,"家属编号");
    userfamModel->setHeaderData(1,Qt::Horizontal,"家属姓名");
    userfamModel->setHeaderData(2,Qt::Horizontal,"家属性别");
    userfamModel->setHeaderData(3,Qt::Horizontal,"家属联系方式");
    userfamModel->setHeaderData(4,Qt::Horizontal,"与业主关系");
    userfamModel->setHeaderData(5,Qt::Horizontal,"业主ID");
    userfamModel->setFilter(tr("uid = %1").arg(ufi));
    userfamModel->select();

    ui->tableView->setModel(userfamModel);
//    ui->tableView->setColumnHidden(5,true);
}

void userFamilyInfo::receveUidFromUser(int UID)
{
    ufi = UID;
}
