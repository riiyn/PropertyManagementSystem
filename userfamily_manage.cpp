#include "userfamily_manage.h"
#include "ui_userfamily_manage.h"
#include "connectionpool.h"
#include "export_excel.h"

#include <QMessageBox>
#include <QSqlError>

userFamily_Manage::userFamily_Manage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::userFamily_Manage)
{
    ui->setupUi(this);

    QSqlDatabase db = ConnectionPool::openConnection();

    userFamilyModel = new QSqlRelationalTableModel(this,db);
    userFamilyModel->setTable("user_family");
    userFamilyModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    userFamilyModel->setRelation(5,QSqlRelation("user","uid","realname"));

    userFamilyModel->setHeaderData(0,Qt::Horizontal,"家属ID");
    userFamilyModel->setHeaderData(1,Qt::Horizontal,"家属姓名");
    userFamilyModel->setHeaderData(2,Qt::Horizontal,"家属性别");
    userFamilyModel->setHeaderData(3,Qt::Horizontal,"家属联系方式");
    userFamilyModel->setHeaderData(4,Qt::Horizontal,"与用户关系");
    userFamilyModel->setHeaderData(5,Qt::Horizontal,"用户姓名");

    userFamilyModel->select();

    ui->tableView->setModel(userFamilyModel);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置行高列宽根据内容自动调整
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();

    ConnectionPool::closeConnection(db);
}

userFamily_Manage::~userFamily_Manage()
{
    delete ui;
}

void userFamily_Manage::on_pushButton_delete_clicked()
{
    //删除操作

    //获取选中的行
    int currentRow = ui->tableView->currentIndex().row();

    if(currentRow >= 0){
        //删除该行
        userFamilyModel->removeRow(currentRow);

        int ok = QMessageBox::warning(this,"删除当前行！",
                                      "你确定删除当前行吗？",
                                      QMessageBox::Yes,
                                      QMessageBox::No);
        if(ok == QMessageBox::No){
            userFamilyModel->revertAll();//如果不删除，则撤销
        } else {
            userFamilyModel->submitAll();//否则删除该行，数据库中对应数据删除
        }
    } else {
        QMessageBox::warning(this,"操作提示！",
                             "请选择要删除的行！",
                             QMessageBox::Ok);
    }
}

void userFamily_Manage::on_pushButton_out_clicked()
{
    //excel
    export_Excel::tableToExcel(ui->tableView,"用户家属信息");
}

void userFamily_Manage::on_pushButton_serch_clicked()
{
    //查询
    QString username = ui->lineEdit_name->text();
    QString userfam = ui->lineEdit_userfam->text();

    if(username != NULL){
        userFamilyModel->setFilter(tr("realname = '%1'").arg(username));
    } else if (userfam != NULL) {
        userFamilyModel->setFilter(tr("ufname = '%1'").arg(userfam));
    } else {
        QMessageBox::warning(this,"操作提示",
                             "请输入查询依据！",
                             QMessageBox::Ok);
    }

    userFamilyModel->select();
    ui->lineEdit_name->clear();
    ui->lineEdit_userfam->clear();
}

void userFamily_Manage::on_pushButton_showall_clicked()
{
    //显示全部
    userFamilyModel->setTable("user_family");
    userFamilyModel->setRelation(5,QSqlRelation("user","uid","realname"));
    userFamilyModel->select();

    userFamilyModel->setHeaderData(0,Qt::Horizontal,"家属ID");
    userFamilyModel->setHeaderData(1,Qt::Horizontal,"家属姓名");
    userFamilyModel->setHeaderData(2,Qt::Horizontal,"家属性别");
    userFamilyModel->setHeaderData(3,Qt::Horizontal,"家属联系方式");
    userFamilyModel->setHeaderData(4,Qt::Horizontal,"与用户关系");
    userFamilyModel->setHeaderData(5,Qt::Horizontal,"用户姓名");

}
