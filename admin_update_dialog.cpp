#include "admin_update_dialog.h"
#include "ui_admin_update_dialog.h"
#include "connectionpool.h"

#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

admin_Update_Dialog::admin_Update_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::admin_Update_Dialog)
{
    ui->setupUi(this);
}

admin_Update_Dialog::~admin_Update_Dialog()
{
    delete ui;
}

void admin_Update_Dialog::receiveAdminUpdateInfo(QString name)
{
    show();
    QSqlDatabase db = ConnectionPool::openConnection();
    updateModel = new QSqlTableModel(this,db);
    updateModel->setTable("admin");
    updateModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    updateModel->setHeaderData(1,Qt::Horizontal,"管理员登录名");
    updateModel->setHeaderData(2,Qt::Horizontal,"登陆密码");
    updateModel->setHeaderData(3,Qt::Horizontal,"管理员姓名");
    updateModel->setHeaderData(4,Qt::Horizontal,"联系电话");
    updateModel->setHeaderData(5,Qt::Horizontal,"管理员状态（1存在，0已删除）");
    updateModel->select();

    ui->tableView->setModel(updateModel);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//选中整行
    //设置行高列宽根据内容自动调整
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();

    ConnectionPool::closeConnection(db);
}

void admin_Update_Dialog::setAdminObject(admin_MainWindow *adminObject){
    connect(adminObject,SIGNAL(sendr_adminUpdateInfo(QString)),this,SLOT(receiveAdminUpdateInfo(QString)));
}

void admin_Update_Dialog::on_pushButton_update_2_clicked()
{
    //提交修改
    updateModel->database().transaction();
    if(updateModel->submitAll()){
        updateModel->database().commit();
    } else {
        updateModel->database().rollback();
        QMessageBox::warning(this,"操作提示！",
                             tr("数据库错误：%1")
                             .arg(updateModel->lastError().text()));
    }

}

void admin_Update_Dialog::on_pushButton_rollbackupdate_clicked()
{
    //撤销修改
    updateModel->revertAll();
}
