#include "changeuserpwd.h"
#include "ui_changeuserpwd.h"
#include "connectionpool.h"

#include <QMessageBox>

int uidToChangePwd;
changeUserPWD::changeUserPWD(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::changeUserPWD)
{
    ui->setupUi(this);

    ui->lineEdit_yanzhengma->hide();
    ui->label_4->hide();
}

changeUserPWD::~changeUserPWD()
{
    delete ui;
}

void changeUserPWD::receiveUidByUser(int uids)
{
    uidToChangePwd = uids;
}

void changeUserPWD::on_pushButton_clicked()
{
    //修改密码
    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    QString oldPwd = ui->lineEdit_yuanmima->text();
    QString newPwd = ui->lineEdit_new->text();
    QString confirNewPwd = ui->lineEdit_confirnew->text();

    if(oldPwd != NULL){
        query.exec(tr("select password from user where uid = %1").arg(uidToChangePwd));
        query.next();
        if(oldPwd == query.value(0).toString().trimmed()){
            if(newPwd != NULL){
                if(newPwd.length() >= 6){
                    if(confirNewPwd != NULL){
                        if(newPwd == confirNewPwd){
                            query.exec(tr("update user set password = '%1' where uid = %2")
                                       .arg(newPwd).arg(uidToChangePwd));
                            QMessageBox::information(this,"提示！",
                                                     "密码修改成功！");
                            ui->lineEdit_confirnew->clear();
                            ui->lineEdit_new->clear();
                            ui->lineEdit_yanzhengma->clear();
                            ui->lineEdit_yuanmima->clear();
                        }else {
                            QMessageBox::warning(this,"错误！",
                                                 "两次输入密码不一致！请重新输入");
                            ui->lineEdit_confirnew->clear();
                        }
                    }else {
                        QMessageBox::warning(this,"错误！",
                                             "请输入确认密码！");
                    }
                }else {
                    QMessageBox::warning(this,"警告！",
                                         "密码长度太短！建议输入6位以上字母数字符号混合型密码！");
                }
            }else {
                QMessageBox::warning(this,"错误！",
                                     "请输入新密码！");
            }
        } else {
            QMessageBox::warning(this,"错误！",
                                 "旧密码错误！请重新输入！");
            ui->lineEdit_yuanmima->clear();
        }
    }else {
        QMessageBox::warning(this,"错误！",
                             "请输入原密码！");
    }

    ConnectionPool::closeConnection(db);
}
