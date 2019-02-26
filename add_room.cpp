#include "add_room.h"
#include "ui_add_room.h"
#include "connectionpool.h"
#include "export_excel.h"

#include <QMessageBox>
#include <QSqlError>

add_room::add_room(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::add_room)
{
    ui->setupUi(this);

    QSqlDatabase db = ConnectionPool::openConnection();
    addRoomModel = new QSqlTableModel(this,db);
    addRoomModel->setTable("room");
    addRoomModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    addRoomModel->setHeaderData(0,Qt::Horizontal,"房间编号");
    addRoomModel->setHeaderData(1,Qt::Horizontal,"房间号");
    addRoomModel->setHeaderData(2,Qt::Horizontal,"楼宇编号");
    addRoomModel->setHeaderData(3,Qt::Horizontal,"用户编号");
    addRoomModel->setHeaderData(4,Qt::Horizontal,"入住时间");
    addRoomModel->setHeaderData(5,Qt::Horizontal,"房间类型");
    addRoomModel->setHeaderData(6,Qt::Horizontal,"房间面积");

    addRoomModel->select();
    ui->tableView->setModel(addRoomModel);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//选中整行
    //设置行高列宽根据内容自动调整
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
    //使其不可编辑
//    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ConnectionPool::closeConnection(db);
}

add_room::~add_room()
{
    delete ui;
}

void add_room::on_pushButton_doadd_clicked()
{
    //执行添加
    //添加数据
    int rowNum = addRoomModel->rowCount();//获得表的行数
    addRoomModel->insertRow(rowNum);//添加一行

}

void add_room::on_pushButton_add_clicked()
{
    //确认添加
    addRoomModel->database().transaction(); //開啟事務操作

    if(addRoomModel->submitAll()){
        addRoomModel->database().commit(); //提交
        QMessageBox::information(this,"操作提示",
                                 "添加成功！");
//        addRoomModel->removeRows(0,addRoomModel->rowCount());

    } else {
        addRoomModel->database().rollback(); //回滾
        QMessageBox::warning(this,"操作提示",
                             "添加失败：请检查楼宇编号以及用户编号是否输入正确！",
                             QMessageBox::Ok);
    }
}
