#include "complaints_managebyuser.h"
#include "ui_complaints_managebyuser.h"
#include "connectionpool.h"
#include "export_excel.h"

#include <QMessageBox>
#include <QSqlError>
#include <QDebug>
#include <QLabel>

int uidFrom;
complaints_ManageByUser::complaints_ManageByUser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::complaints_ManageByUser)
{
    ui->setupUi(this);

    //设置状态栏
    QLabel *per = new QLabel(this);
    per->setFrameStyle(QFrame::Box | QFrame::Sunken);
    per->setText("开发者：张富云   学校：山东建筑大学   学号：20140815020");
    per->setMinimumSize(400,30);
    ui->statusbar->addPermanentWidget(per);

    QSqlDatabase db = ConnectionPool::openConnection();
    complaintModel = new QSqlTableModel(this,db);
    complaintModel->setTable("complaints");
    complaintModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    complaintModel->setHeaderData(0,Qt::Horizontal,"投诉编号");
    complaintModel->setHeaderData(1,Qt::Horizontal,"投诉者ID");
    complaintModel->setHeaderData(2,Qt::Horizontal,"投诉者姓名");
    complaintModel->setHeaderData(3,Qt::Horizontal,"投诉内容");
    complaintModel->setHeaderData(4,Qt::Horizontal,"投诉时间");
    complaintModel->setHeaderData(5,Qt::Horizontal,"投诉状态（1未处理，0已处理）");
    complaintModel->setFilter(tr("uid = %1").arg(uidFrom));
    complaintModel->select();

    ui->tableView->setModel(complaintModel);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//选中整行
    //设置行高列宽根据内容自动调整
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();


    ConnectionPool::closeConnection(db);

}

complaints_ManageByUser::~complaints_ManageByUser()
{
    delete ui;
}

void complaints_ManageByUser::on_pushButton_clicked()
{
    //删除
    //获取选中的行
    int currentRow = ui->tableView->currentIndex().row();
    if(currentRow >= 0){
        //删除该行
        complaintModel->removeRow(currentRow);

        int ok = QMessageBox::warning(this,"删除当前行！",
                                      "你确定删除当前行吗？",
                                      QMessageBox::Yes,
                                      QMessageBox::No);
        if(ok == QMessageBox::No){
            complaintModel->revertAll();//如果不删除，则撤销
        } else {
            complaintModel->submitAll();//否则删除该行，数据库中对应数据删除
        }
    } else {
        QMessageBox::warning(this,"操作提示！",
                             "请选择要删除的行！",
                             QMessageBox::Ok);
    }
}

void complaints_ManageByUser::on_pushButton_3_clicked()
{
    //显示全部
    complaintModel->setTable("complaints");
    complaintModel->setHeaderData(0,Qt::Horizontal,"投诉编号");
    complaintModel->setHeaderData(1,Qt::Horizontal,"投诉者ID");
    complaintModel->setHeaderData(2,Qt::Horizontal,"投诉者姓名");
    complaintModel->setHeaderData(3,Qt::Horizontal,"投诉内容");
    complaintModel->setHeaderData(4,Qt::Horizontal,"投诉时间");
    complaintModel->setHeaderData(5,Qt::Horizontal,"投诉状态（1未处理，0已处理）");
    complaintModel->setFilter(tr("uid = %1").arg(uidFrom));
    complaintModel->select();

}

void complaints_ManageByUser::on_pushButton_4_clicked()
{
    //导出excel
    export_Excel::tableToExcel(ui->tableView,"我的投诉信息");

}

void complaints_ManageByUser::on_pushButton_2_clicked()
{
    //已处理查询
    complaintModel->setTable("complaints");
    complaintModel->setHeaderData(0,Qt::Horizontal,"投诉编号");
    complaintModel->setHeaderData(1,Qt::Horizontal,"投诉者ID");
    complaintModel->setHeaderData(2,Qt::Horizontal,"投诉者姓名");
    complaintModel->setHeaderData(3,Qt::Horizontal,"投诉内容");
    complaintModel->setHeaderData(4,Qt::Horizontal,"投诉时间");
    complaintModel->setHeaderData(5,Qt::Horizontal,"投诉状态（1未处理，0已处理）");
//    complaintModel->setFilter("comp_state = 0");
    complaintModel->setFilter(tr("uid = %1 and comp_state = 0").arg(uidFrom));
    complaintModel->select();

}

void complaints_ManageByUser::on_pushButton_6_clicked()
{
    //未处理查询
    complaintModel->setTable("complaints");
    complaintModel->setHeaderData(0,Qt::Horizontal,"投诉编号");
    complaintModel->setHeaderData(1,Qt::Horizontal,"投诉者ID");
    complaintModel->setHeaderData(2,Qt::Horizontal,"投诉者姓名");
    complaintModel->setHeaderData(3,Qt::Horizontal,"投诉内容");
    complaintModel->setHeaderData(4,Qt::Horizontal,"投诉时间");
    complaintModel->setHeaderData(5,Qt::Horizontal,"投诉状态（1未处理，0已处理）");
//    complaintModel->setFilter("comp_state = 1");
    complaintModel->setFilter(tr("uid = %1 and comp_state = 1").arg(uidFrom));
    complaintModel->select();
}

void complaints_ManageByUser::on_pushButton_5_clicked()
{
    //处理投诉
    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    int currentRow = ui->tableView->currentIndex().row();//选中行
    if(currentRow >= 0){
        QAbstractItemModel *model = ui->tableView->model();
        QModelIndex indexTemp = model->index(currentRow,0);
        QVariant dataTemp = model->data(indexTemp);

        int ok = QMessageBox::warning(this,"投诉处理！",
                                      "你确定处理此投诉吗？",
                                      QMessageBox::Yes,
                                      QMessageBox::No);
        if(ok == QMessageBox::No){
            complaintModel->revertAll();
        } else {
            query.exec(tr("update complaints set comp_state = 0 where comp_id = %1").arg(dataTemp.toInt()));
        }


    }else {
        QMessageBox::warning(this,"操作提示！",
                             "请选择要处理的投诉！",
                             QMessageBox::Ok);
    }

    ConnectionPool::closeConnection(db);
}

void complaints_ManageByUser::receiveUidFromCinfo(int uidfrom)
{
    uidFrom = uidfrom;
}

