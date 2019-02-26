#include "complaintinfo_user.h"
#include "ui_complaintinfo_user.h"
#include "connectionpool.h"

#include <QDateTime>
#include <QMessageBox>
int uidFromUser;
complaintInfo_User::complaintInfo_User(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::complaintInfo_User)
{
    ui->setupUi(this);
    complaintManageByUser = new complaints_ManageByUser(0);
    connect(ui->pushButton_2,&QPushButton::clicked,complaintManageByUser,&complaints_ManageByUser::show);
    connect(this,SIGNAL(senderUidToComplaintMbyU(int)),complaintManageByUser,SLOT(receiveUidFromCinfo(int)));
}

complaintInfo_User::~complaintInfo_User()
{
    delete ui;
}

void complaintInfo_User::on_pushButton_clicked()
{
    //投诉
    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    int uid = ui->lineEdit_id->text().toInt();
    QString name = ui->lineEdit_name->text();
    QString text = ui->textEdit->toPlainText();

    QDateTime locale(QDateTime::currentDateTime());
    QString localTime = locale.toString("yyyy-MM-dd hh:mm:ss");

    if(uid != NULL && name != NULL && text != NULL){
        query.exec(tr("insert into complaints values(0,%1,'%2','%3','%4',1)")
                   .arg(uid).arg(name).arg(text).arg(localTime));
        QMessageBox::information(this,"提示！",
                                 "您已成功投诉，我们会尽快处理您的投诉！感谢支持！");
        ui->lineEdit_id->clear();
        ui->lineEdit_name->clear();
        ui->textEdit->clear();
    }else {
        QMessageBox::warning(this,"警告！",
                             "请输入所有可选项！");
    }

    ConnectionPool::closeConnection(db);
}

void complaintInfo_User::on_pushButton_2_clicked()
{
    //查看我的投诉
    emit senderUidToComplaintMbyU(uidFromUser);
}

void complaintInfo_User::receveUidFromUser(int uid)
{
    uidFromUser = uid;
}
