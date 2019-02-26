#include "admin_signup_dialog.h"
#include "ui_admin_signup_dialog.h"
#include "connectionpool.h"

#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

admin_SignUp_Dialog::admin_SignUp_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::admin_SignUp_Dialog)
{
    ui->setupUi(this);
}

admin_SignUp_Dialog::~admin_SignUp_Dialog()
{
    delete ui;
}

void admin_SignUp_Dialog::setAdminSignUpObject(Widget *widgetObject)
{
    connect(widgetObject,SIGNAL(admin_signUp()),this,SLOT(receiveAdminSignUp()));
}

void admin_SignUp_Dialog::receiveAdminSignUp()
{
    show();
}


void admin_SignUp_Dialog::on_pushButton_signup_clicked()
{
    //注册
    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    QString logName = ui->lineEdit_logName->text();
    QString pwd = ui->lineEdit_pwd->text();
    QString pwdConfirm = ui->lineEdit_pwdConfirm->text();
    QString realName = ui->lineEdit_realname->text();
    QString tel = ui->lineEdit_tel->text();

    if(logName != NULL && pwd != NULL && pwdConfirm != NULL &&
            realName != NULL && tel != NULL){
        query.exec(tr("select * from admin where aname = '%1'").arg(logName));
        if(query.next()){
            QMessageBox::warning(this,"提示！",
                                 "登录名已存在，请重新输入！");
        }else {
            if(pwd == pwdConfirm){
                if(pwd.length() >= 6){
                    if(tel.length() == 7 || tel.length() == 11){
                        query.exec(QString("insert into admin values(0,'%1','%2','%3','%4',1)")
                                   .arg(logName).arg(pwd).arg(realName).arg(tel));
                        QMessageBox::information(this,"提示！",
                                                 "注册成功！");
                        ui->lineEdit_logName->clear();
                        ui->lineEdit_pwd->clear();
                        ui->lineEdit_pwdConfirm->clear();
                        ui->lineEdit_realname->clear();
                        ui->lineEdit_tel->clear();
                        close();
                    }else {
                        QMessageBox::warning(this,"警告！",
                                             "请输入正确的手机或电话号码！");
                    }
                }else {
                    QMessageBox::warning(this,"警告！",
                                         "密码长度太短，请输入6位以上字母数字符号混合型密码！");
                }
            }else {
                QMessageBox::warning(this,"错误！",
                                     "两次输入密码不一致！！",
                                     QMessageBox::Ok);
                ui->lineEdit_pwd->clear();
                ui->lineEdit_pwdConfirm->clear();
            }
        }
    }else {
        QMessageBox::warning(this,"错误！",
                             "请输入所有可选项！",
                             QMessageBox::Ok);
    }

    ConnectionPool::closeConnection(db);

}
