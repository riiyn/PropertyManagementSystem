#include "complaint_manage.h"
#include "ui_complaint_manage.h"
#include "export_excel.h"
#include "connectionpool.h"

#include <QMessageBox>

complaint_Manage::complaint_Manage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::complaint_Manage)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

//    //手动调整列宽时根据内容变化行高
//    connect(ui->tableView->horizontalHeader(),SIGNAL(sectionResized(int,int,int)),
//            ui->tableView,SLOT(resizeRowsToContents()));
//    //初始化及插入新行时主动刷新行高
//    connect(ui->tableView->model(),SIGNAL(rowsInserted(QModelIndex,int,int)),
//            this,SLOT(on_TableViewColWidth_Changed(QModelIndex,int,int)));

    QSqlDatabase db = ConnectionPool::openConnection();
    queryModel = new QSqlQueryModel(this);
    complaintModel = new QSqlRelationalTableModel(this,db);
    complaintModel->setTable("complaints");
    complaintModel->setRelation(1,QSqlRelation("user","uid","uid"));
    complaintModel->setRelation(2,QSqlRelation("user","realname","realname"));
    complaintModel->setHeaderData(0,Qt::Horizontal,"投诉编号");
    complaintModel->setHeaderData(1,Qt::Horizontal,"用户ID");
    complaintModel->setHeaderData(2,Qt::Horizontal,"用户姓名");
    complaintModel->setHeaderData(3,Qt::Horizontal,"投诉内容");
    complaintModel->setHeaderData(4,Qt::Horizontal,"投诉日期");
    complaintModel->setHeaderData(5,Qt::Horizontal,"投诉状态");
    complaintModel->select();

    ui->tableView->setModel(complaintModel);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//选中整行
    //设置行高列宽根据内容自动调整
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();

    ConnectionPool::closeConnection(db);

}

complaint_Manage::~complaint_Manage()
{
    delete ui;
}

void complaint_Manage::on_pushButton_search_clicked()
{
    //chaxun
    QString name = ui->lineEdit_name->text();
    if(name != NULL){
        QSqlDatabase db = ConnectionPool::openConnection();
        queryModel->setQuery(tr("select *from complaints where realname = '%1'").arg(name),db);
        queryModel->setHeaderData(0,Qt::Horizontal,"投诉编号");
        queryModel->setHeaderData(1,Qt::Horizontal,"用户ID");
        queryModel->setHeaderData(2,Qt::Horizontal,"用户姓名");
        queryModel->setHeaderData(3,Qt::Horizontal,"投诉内容");
        queryModel->setHeaderData(4,Qt::Horizontal,"投诉日期");
        queryModel->setHeaderData(5,Qt::Horizontal,"投诉状态");

        ui->tableView->setModel(queryModel);
        ui->lineEdit_name->clear();

    } else {
        QMessageBox::warning(this,"操作提示",
                             "请输入查询依据！",
                             QMessageBox::Ok);
    }

}

void complaint_Manage::on_pushButton_all_clicked()
{
    //showall
    complaintModel->setTable("complaints");
    complaintModel->setRelation(1,QSqlRelation("user","uid","uid"));
    complaintModel->setRelation(2,QSqlRelation("user","realname","realname"));
    complaintModel->setHeaderData(0,Qt::Horizontal,"投诉编号");
    complaintModel->setHeaderData(1,Qt::Horizontal,"用户ID");
    complaintModel->setHeaderData(2,Qt::Horizontal,"用户姓名");
    complaintModel->setHeaderData(3,Qt::Horizontal,"投诉内容");
    complaintModel->setHeaderData(4,Qt::Horizontal,"投诉日期");
    complaintModel->setHeaderData(5,Qt::Horizontal,"投诉状态");
    complaintModel->select();
}

void complaint_Manage::on_pushButton_out_clicked()
{
    //导出
    export_Excel::tableToExcel(ui->tableView,"投诉管理信息");
}

//自动换行
//void complaint_Manage::on_TableViewColWidth_Changed(const QModelIndex &index, int first, int last)
//{
//    ui->tableView->resizeRowToContents(first);
//}



