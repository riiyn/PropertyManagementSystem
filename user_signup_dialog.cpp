#include "user_signup_dialog.h"
#include "ui_user_signup_dialog.h"
#include "connectionpool.h"

#include <QMessageBox>

user_SignUp_Dialog::user_SignUp_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::user_SignUp_Dialog)
{
    ui->setupUi(this);
}

user_SignUp_Dialog::~user_SignUp_Dialog()
{
    delete ui;
}

void user_SignUp_Dialog::setUserSignUpObject(Widget *widgetObject)
{
    connect(widgetObject,SIGNAL(user_signUp()),this,SLOT(receiveUserSignUp()));
}

void user_SignUp_Dialog::receiveUserSignUp()
{
    show();
}

void user_SignUp_Dialog::on_pushButton_signup_clicked()
{
    //注册
    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    QString userName = ui->lineEdit_username->text();
    QString pwd = ui->lineEdit_pwd->text();
    QString pwdConf = ui->lineEdit_pwdConfir->text();
    QString realName = ui->lineEdit_realName->text();
    int age = ui->lineEdit_age->text().toInt();
    QString address = ui->lineEdit_address->text();
    int houseID = ui->lineEdit_houseID->text().toInt();
    int unitID = ui->lineEdit_unitID->text().toInt();
    int roomID = ui->lineEdit_roomID->text().toInt();
    QString IDCard = ui->lineEdit_IDCard->text();
    QString tel = ui->lineEdit_tel->text();
    QString workPlace = ui->lineEdit_workPla->text();
    QString wyCard = "";

    if(userName != NULL && pwd != NULL && pwdConf != NULL && realName != NULL &&
            address != NULL && IDCard != NULL && tel != NULL && workPlace != NULL){
        query.exec(tr("select *from user where username = '%1'").arg(userName));
        if(query.next()){
            QMessageBox::warning(this,"错误！",
                                 "登录名已存在，请重新输入！");
        }else {
            if(pwd == pwdConf){
                if(pwd.length() >= 6){
                    if(age == NULL){
                        QMessageBox::warning(this,"错误！",
                                             "请检查“年龄”是否输入以及是否为数字！");
                    }else {
                        if(age < 150 && age > 0){
                            if(houseID == NULL){
                                QMessageBox::warning(this,"错误！",
                                                     "请检查“楼号”是否输入以及是否为数字！");
                            }else {
                                if(unitID == NULL){
                                    QMessageBox::warning(this,"错误！",
                                                         "请检查“单元号”是否输入以及是否为数字！");
                                }else {
                                    if(roomID == NULL){
                                        QMessageBox::warning(this,"错误！",
                                                             "请检查“房间号”是否输入以及是否为数字！");
                                    }else {
                                        if(IDCard.length() == 18){
                                            if(tel.length() == 7 || tel.length() == 11){
                                                if(ui->radioButton_boy->isChecked()){
                                                    query.exec(tr("insert into user values(0,'%1','%2','%3','男',%5,'%6',%7,%8,%9,'%10','%11','%12','%13','1')")
                                                               .arg(userName).arg(pwd).arg(realName)
                                                               .arg(age).arg(address).arg(houseID)
                                                               .arg(unitID).arg(roomID).arg(IDCard)
                                                               .arg(tel).arg(workPlace).arg(wyCard));
                                                    QMessageBox::information(this,"提示！",
                                                                             "注册成功！");
                                                    ui->lineEdit_address->clear();
                                                    ui->lineEdit_age->clear();
                                                    ui->lineEdit_houseID->clear();
                                                    ui->lineEdit_IDCard->clear();
                                                    ui->lineEdit_pwd->clear();
                                                    ui->lineEdit_pwdConfir->clear();
                                                    ui->lineEdit_realName->clear();
                                                    ui->lineEdit_roomID->clear();
                                                    ui->lineEdit_tel->clear();
                                                    ui->lineEdit_unitID->clear();
                                                    ui->lineEdit_username->clear();
                                                    ui->lineEdit_workPla->clear();
                                                    close();
                                                }else if (ui->radioButton_girl->isChecked()) {
                                                    query.exec(tr("insert into user values(0,'%1','%2','%3','女',%5,'%6',%7,%8,%9,'%10','%11','%12','%13','1')")
                                                               .arg(userName).arg(pwd).arg(realName)
                                                               .arg(age).arg(address).arg(houseID)
                                                               .arg(unitID).arg(roomID).arg(IDCard)
                                                               .arg(tel).arg(workPlace).arg(wyCard));
                                                    QMessageBox::information(this,"提示！",
                                                                             "注册成功！");
                                                    ui->lineEdit_address->clear();
                                                    ui->lineEdit_age->clear();
                                                    ui->lineEdit_houseID->clear();
                                                    ui->lineEdit_IDCard->clear();
                                                    ui->lineEdit_pwd->clear();
                                                    ui->lineEdit_pwdConfir->clear();
                                                    ui->lineEdit_realName->clear();
                                                    ui->lineEdit_roomID->clear();
                                                    ui->lineEdit_tel->clear();
                                                    ui->lineEdit_unitID->clear();
                                                    ui->lineEdit_username->clear();
                                                    ui->lineEdit_workPla->clear();
                                                    close();
                                                } else {
                                                    QMessageBox::warning(this,"错误！",
                                                                         "请选择性别！");
                                                }
                                            }else {
                                                QMessageBox::warning(this,"错误！",
                                                                     "请输入正确的7位电话号码或11位手机号码！");
                                            }
                                        }else {
                                            QMessageBox::warning(this,"错误！",
                                                                 "请输入18位正确的身份证号码！");
                                        }
                                    }
                                }
                            }
                        }else {
                            QMessageBox::warning(this,"警告！",
                                                 "请输入正确的年龄！");
                        }
                    }
                }else {
                    QMessageBox::warning(this,"错误！",
                                         "密码长度太短，请输入不小于6位的字母数字符号混合型密码！");
                }
            }else {
                QMessageBox::warning(this,"错误！",
                                     "两次输入秘密不一致，请重新输入！");
                ui->lineEdit_pwd->clear();
                ui->lineEdit_pwdConfir->clear();
            }
        }
    }else {
        QMessageBox::warning(this,"错误！",
                             "请输入所有可选项！");
    }

    ConnectionPool::closeConnection(db);
}
