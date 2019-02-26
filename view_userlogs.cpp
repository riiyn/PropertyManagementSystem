#include "view_userlogs.h"
#include "ui_view_userlogs.h"
#include "connectionpool.h"
#include "export_excel.h"

#include <QMessageBox>

int uidtouserLogs;
view_UserLogs::view_UserLogs(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::view_UserLogs)
{
    ui->setupUi(this);

    QSqlDatabase db = ConnectionPool::openConnection();
    userlogsModel = new QSqlTableModel(this,db);
    userlogsModel->setTable("userLogs");
    userlogsModel->setHeaderData(2,Qt::Horizontal,"登陆姓名");
    userlogsModel->setHeaderData(3,Qt::Horizontal,"登陆时间");
    userlogsModel->setHeaderData(4,Qt::Horizontal,"登陆IP");
    userlogsModel->setFilter(tr("uid = %1").arg(uidtouserLogs));
    userlogsModel->select();

    ui->tableView->setModel(userlogsModel);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnHidden(1,true);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置表格不可编辑
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//选中整行
    //设置行高列宽根据内容自动调整
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();

    ConnectionPool::closeConnection(db);
}

view_UserLogs::~view_UserLogs()
{
    delete ui;
}

void view_UserLogs::on_pushButton_serch_clicked()
{
    //查询
    QString name = ui->lineEdit_name->text();

    if(name != NULL){
        userlogsModel->setFilter(QString("logIP = '%1' and uid = %2")
                                 .arg(name).arg(uidtouserLogs));
    }else {
        QMessageBox::warning(this,"操作提示",
                             "请输入IP地址！",
                             QMessageBox::Ok);
    }

    userlogsModel->select();
    ui->lineEdit_name->clear();
}

void view_UserLogs::on_pushButton_showall_clicked()
{
    //showall
    userlogsModel->setTable("userLogs");
    userlogsModel->setHeaderData(2,Qt::Horizontal,"登陆姓名");
    userlogsModel->setHeaderData(3,Qt::Horizontal,"登陆时间");
    userlogsModel->setHeaderData(4,Qt::Horizontal,"登陆IP");
    userlogsModel->setFilter(tr("uid = %1").arg(uidtouserLogs));
    userlogsModel->select();
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnHidden(1,true);
}

void view_UserLogs::on_pushButton_out_2_clicked()
{
    //excel
    export_Excel::tableToExcel(ui->tableView,"登录日志");
}

void view_UserLogs::userLogs_receiveUidFromUser(int uidToUserLogs)
{
    uidtouserLogs = uidToUserLogs;
}
