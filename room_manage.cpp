#include "room_manage.h"
#include "ui_room_manage.h"
#include "connectionpool.h"
#include "export_excel.h"
#include "add_room.h"

#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

room_Manage::room_Manage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::room_Manage)
{
    ui->setupUi(this);
    addRoom = new add_room(0);
    connect(ui->pushButton_add_2,&QPushButton::clicked,addRoom,&add_room::show);


    ui->pushButton_adduser->hide();
    QSqlDatabase db = ConnectionPool::openConnection();

    roomModel = new QSqlRelationalTableModel(this,db);
    roomModel->setTable("room");
    roomModel->setRelation(2,QSqlRelation("house","hid","hname"));
    roomModel->setRelation(3,QSqlRelation("user","uid","realname"));

    roomModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    roomModel->setHeaderData(1,Qt::Horizontal,"房间号");
    roomModel->setHeaderData(2,Qt::Horizontal,"所属楼宇");
    roomModel->setHeaderData(3,Qt::Horizontal,"房主姓名");
    roomModel->setHeaderData(4,Qt::Horizontal,"入住时间");
    roomModel->setHeaderData(5,Qt::Horizontal,"房间类型");
    roomModel->setHeaderData(6,Qt::Horizontal,"房间面积（m2）");

    roomModel->select();
    ui->tableView->setModel(roomModel);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//选中整行
    //设置行高列宽根据内容自动调整
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();

    ConnectionPool::closeConnection(db);
}

room_Manage::~room_Manage()
{
    delete ui;
}

void room_Manage::on_pushButton_add_2_clicked()
{
    //添加
}

void room_Manage::on_pushButton_update_2_clicked()
{
    //提交修改
    roomModel->database().transaction();
    if(roomModel->submitAll()){
        roomModel->database().commit();
    } else {
        roomModel->database().rollback();
        QMessageBox::warning(this,"操作提示！",
                             tr("数据库错误：%1")
                             .arg(roomModel->lastError().text()));
    }
}


void room_Manage::on_pushButton_rollbackupdate_clicked()
{
    //撤销修改
    roomModel->revertAll();
    noEnterRoomModel->revertAll();
}

void room_Manage::on_pushButton_delete_2_clicked()
{
    //删除
    //获取选中行
    int currentRow = ui->tableView->currentIndex().row();
    if(currentRow >= 0){
        //删除
        roomModel->removeRow(currentRow);
        noEnterRoomModel->removeRow(currentRow);

        int ok = QMessageBox::warning(this,"删除当前行！",
                                      "你确定删除当前行吗？",
                                      QMessageBox::Yes,
                                      QMessageBox::No);
        if(ok == QMessageBox::No){
            roomModel->revertAll();
            noEnterRoomModel->revertAll();
        }else {
            roomModel->submitAll();
            noEnterRoomModel->submitAll();
        }
    } else {
        QMessageBox::warning(this,"操作提示！",
                             "请选择要删除的行！",
                             QMessageBox::Ok);
    }
}

void room_Manage::on_pushButton_showall_clicked()
{
    //显示全部
    ui->pushButton_adduser->hide();
    roomModel->setTable("room");
    roomModel->setRelation(2,QSqlRelation("house","hid","hname"));
    roomModel->setRelation(3,QSqlRelation("user","uid","realname"));

    roomModel->setHeaderData(1,Qt::Horizontal,"房间号");
    roomModel->setHeaderData(2,Qt::Horizontal,"所属楼宇");
    roomModel->setHeaderData(3,Qt::Horizontal,"房主姓名");
    roomModel->setHeaderData(4,Qt::Horizontal,"入住时间");
    roomModel->setHeaderData(5,Qt::Horizontal,"房间类型");
    roomModel->setHeaderData(6,Qt::Horizontal,"房间面积（m2）");

    roomModel->select();
    ui->tableView->setModel(roomModel);
    ui->tableView->setColumnHidden(0,true);
}

void room_Manage::on_pushButton_serch_2_clicked()
{
    //查询
    QString hname = ui->lineEdit_housname->text();
    QString rnum = ui->lineEdit_rnum->text();

    if(hname != NULL){
        roomModel->setFilter(tr("hname = '%1'").arg(hname));
    } else if (rnum != NULL) {
        roomModel->setFilter(tr("rnum = '%1'").arg(rnum));
    } else {
        QMessageBox::warning(this,"操作提示！",
                             "请输入查询依据！",
                             QMessageBox::Ok);
    }
    roomModel->select();
    ui->lineEdit_housname->clear();
    ui->lineEdit_rnum->clear();
}

void room_Manage::on_pushButton_out_2_clicked()
{
    //excel
    export_Excel::tableToExcel(ui->tableView,"车辆数据(carsData)");
}

void room_Manage::on_pushButton_showall_2_clicked()
{
    //未入住房间查询

    ui->pushButton_adduser->show();
    QSqlDatabase db = ConnectionPool::openConnection();
    noEnterRoomModel = new QSqlTableModel(this,db);
    noEnterRoomModel->setTable("room");
    noEnterRoomModel->setFilter("ISNULL(uid)");
    roomModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    noEnterRoomModel->setHeaderData(1,Qt::Horizontal,"房间号");
    noEnterRoomModel->setHeaderData(2,Qt::Horizontal,"所属楼宇");
    noEnterRoomModel->setHeaderData(3,Qt::Horizontal,"房主ID");
    noEnterRoomModel->setHeaderData(4,Qt::Horizontal,"入住时间");
    noEnterRoomModel->setHeaderData(5,Qt::Horizontal,"房间类型");
    noEnterRoomModel->setHeaderData(6,Qt::Horizontal,"房间面积（m2）");

    noEnterRoomModel->select();
    ui->tableView->setModel(noEnterRoomModel);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//选中整行
    //设置行高列宽根据内容自动调整
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();

    ConnectionPool::closeConnection(db);
}

void room_Manage::on_pushButton_adduser_clicked()
{
    //添加住户
    QSqlDatabase db = ConnectionPool::openConnection();
    int curRow = ui->tableView->currentIndex().row();
    QModelIndex indexTemp = noEnterRoomModel->index(curRow,3);
    QVariant dataTemp = noEnterRoomModel->data(indexTemp);
    int uid = dataTemp.toInt();

    QModelIndex indexTemp2 = noEnterRoomModel->index(curRow,4);
    QVariant dataTemp2 = noEnterRoomModel->data(indexTemp2);
    QString enterTime = dataTemp2.toString();

    QModelIndex indexTemp1 = noEnterRoomModel->index(curRow,0);
    QVariant dataTemp1 = noEnterRoomModel->data(indexTemp1);
    int rid = dataTemp1.toInt();
    if(uid != NULL && enterTime != NULL){
        QSqlQuery query(db);
        query.exec(QString("update room set uid = %1,enterTime = '%2' where rid = %3")
                   .arg(uid).arg(enterTime).arg(rid));
        QMessageBox::information(this,"提示",
                                 "住户添加成功！");
    }else {
        QMessageBox::warning(this,"错误",
                             "请输入住户ID或入住时间！");
    }
    ConnectionPool::closeConnection(db);
}
