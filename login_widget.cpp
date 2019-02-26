#include "login_widget.h"
#include "ui_login_widget.h"
#include "user_mainwindow.h"
#include "connectionpool.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include <QSignalTransition>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QDateTime>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->label_title_2->setText("**大学毕业设计\n\n大麦场物业管理系统欢迎您！");
    ui->textBrowser_deveInfo->setText("开发者信息\n\n开发者：**\n\n学  校：**大学\n\n学  号：**********");
    ui->textBrowser_deveInfo->setAlignment(Qt::AlignCenter);

    m_AdmwinDialog = new admin_MainWindow(0);
    m_AdmwinDialog->hide();
}

Widget::~Widget()
{
    delete m_AdmwinDialog;
    delete ui;
}

QString Widget::getLocalmachineIP(){
    /***获取IP***/
    QString ipAddress;
    QList<QHostAddress> ipAddressList = QNetworkInterface::allAddresses();
    for(int i = 0; i < ipAddressList.size(); ++i){
        if(ipAddressList.at(i) != QHostAddress::LocalHost &&
                ipAddressList.at(i).toIPv4Address()){
            ipAddress = ipAddressList.at(i).toString();
            break;
        }
    }
    if(ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    return ipAddress;
}

/**
 * 登陆操作
 * @brief Widget::on_pushButton_login_2_clicked
 */
//int count1 = 0;
//int count2 = 0;
void Widget::on_pushButton_login_2_clicked()
{
    QSqlDatabase db =  ConnectionPool::openConnection();
    QSqlQuery query(db);

    if(ui->radioButton_3->isChecked()){
        if(ui->lineEdit_name_2->text().isEmpty()){
            QMessageBox::warning(this,"提示",
                                 "请输入用户名！",
                                 QMessageBox::Ok);
        }else {
            //验证用户是否存在
            query.exec(tr("select * from admin where aname = '%1'")
                                  .arg(ui->lineEdit_name_2->text()));
            if(query.next() && query.value(5).toString().trimmed() == "1"){

                if(ui->lineEdit_pwd_2->text().isEmpty()){
                    QMessageBox::warning(this,"提示",
                                         "请输入密码！",
                                         QMessageBox::Ok);
                }else {
                    QString pwd = query.value(2).toString().trimmed();

                    //验证密码是否正确
                    if(ui->lineEdit_pwd_2->text() == pwd){

                        emit admin_loginButtonClicked();

//                        m_AdmwinDialog->show();
//                        m_AdmwinDialog->receveData(/*ui->pushButton_login_2*/);

                        adminData *admindata = new adminData();
                        admindata->aid = query.value(0).toInt();
                        admindata->aname = query.value(1).toString().trimmed();
                        admindata->apassword = query.value(2).toString().trimmed();
                        admindata->arealname = query.value(3).toString().trimmed();
                        admindata->tel = query.value(4).toString().trimmed();
                        admindata->status = query.value(5).toString().trimmed();

                        ui->pushButton_login_2->setUserData(Qt::UserRole,admindata);

                        emit sender_loginInfo(ui->pushButton_login_2);

                        //获取ip
                        QString lastIp = Widget::getLocalmachineIP();
                        //获取最后登录时间
                        QDateTime locale(QDateTime::currentDateTime());
                        QString localTime = locale.toString("yyyy-MM-dd hh:mm:ss");

                        //判断登陆者是否存在于log表，若是，则更新数据，若否，则插入数据
                        query.exec(tr("select * from log where log_name = '%1'")
                                   .arg(admindata->arealname));
                        if(query.next()){//存在
                            query.exec(tr("update log set log_counts = log_counts + 1,last_date = '%2',last_ip = '%3' where log_name = '%4'")
                                       .arg(localTime).arg(lastIp).arg(admindata->arealname));
                        } else {
                            //不存在
                            query.exec(tr("insert into log (last_ip,last_date,log_name,log_counts,log_play) values('%1','%2','%3',1,'%4')")
                                       .arg(lastIp).arg(localTime).arg(admindata->arealname).arg("管理员"));
                        }

                        //清空登录信息
                        ui->lineEdit_name_2->clear();
                        ui->lineEdit_pwd_2->clear();

                        //关闭登录界面
                        close();

                    } else {
                        QMessageBox::warning(this,"错误警告！",
                                             "密码错误！",
                                             QMessageBox::Ok);
                        ui->lineEdit_pwd_2->clear();
                        ui->lineEdit_pwd_2->setFocus();
                    }
                }
            } else {
                query.exec(tr("select * from xiaoqu where username = '%1'")
                           .arg(ui->lineEdit_name_2->text()));
                if(query.next()){
                    if(ui->lineEdit_pwd_2->text().isEmpty()){
                        QMessageBox::warning(this,"提示",
                                             "请输入密码！",
                                             QMessageBox::Ok);
                    } else {
                        QString pwd = query.value(6).toString().trimmed();
                        if(pwd == ui->lineEdit_pwd_2->text()){
                            emit admin_loginButtonClicked();
                            xqManagerData *xqManagerdata = new xqManagerData();
                            xqManagerdata->username = query.value(5).toString().trimmed();
                            xqManagerdata->name = query.value(2).toString().trimmed();
                            xqManagerdata->tel = query.value(4).toString().trimmed();

                            ui->pushButton_login_2->setUserData(Qt::UserRole,xqManagerdata);

                            emit sender_xqManagerInfo(ui->pushButton_login_2);

                            //获取ip
                            QString lastIp = Widget::getLocalmachineIP();
                            //获取最后登录时间
                            QDateTime locale(QDateTime::currentDateTime());
                            QString localTime = locale.toString("yyyy-MM-dd hh:mm:ss");
                            //更新小区负责人最后登陆时间
                            query.exec(tr("select *from xiaoqu where username = '%1'").arg(ui->lineEdit_name_2->text()));
                            if(query.next()){
                                query.exec(tr("update xiaoqu set op_time = '%1' where username = '%2'")
                                           .arg(localTime).arg(ui->lineEdit_name_2->text()));
                            }

                            //判断登陆者是否存在于log表，若是，则更新数据，若否，则插入数据
                            query.exec(tr("select * from log where log_name = '%1'")
                                       .arg(xqManagerdata->name));
                            if(query.next()){//存在
                                query.exec(tr("update log set log_counts = log_counts + 1,last_date = '%2',last_ip = '%3' where log_name = '%4'")
                                           .arg(localTime).arg(lastIp).arg(xqManagerdata->name));
                            } else {
                                //不存在
                                query.exec(tr("insert into log (last_ip,last_date,log_name,log_counts,log_play) values('%1','%2','%3',1,'%4')")
                                           .arg(lastIp).arg(localTime).arg(xqManagerdata->name).arg("小区负责人"));
                            }
                            //清空登录信息
                            ui->lineEdit_name_2->clear();
                            ui->lineEdit_pwd_2->clear();

                            //关闭登录界面
                            close();

                        }else {
                            QMessageBox::warning(this,"错误警告！",
                                                 "密码错误！",
                                                 QMessageBox::Ok);
                            ui->lineEdit_pwd_2->clear();
                            ui->lineEdit_pwd_2->setFocus();
                        }
                    }
                } else{
                    QMessageBox::warning(this,"错误警告！",
                                         "管理员不存在！",
                                         QMessageBox::Ok);
                    ui->lineEdit_name_2->clear();
                    ui->lineEdit_name_2->setFocus();
                }

            }
        }


    } else if (ui->radioButton_4->isChecked()) {
        if(ui->lineEdit_name_2->text().isEmpty()){
            QMessageBox::warning(this,"提示",
                                 "请输入用户名！",
                                 QMessageBox::Ok);
        }else {
            query.exec(QString("select *from user where username = '%1'")
                       .arg(ui->lineEdit_name_2->text()));
            if(query.next() && query.value(14).toString().trimmed() == "1"){
                if(ui->lineEdit_pwd_2->text().isEmpty()){
                    QMessageBox::warning(this,"提示",
                                         "请输入密码！",
                                         QMessageBox::Ok);
                }else {
                    QString upwd = query.value(2).toString().trimmed();

                    if(ui->lineEdit_pwd_2->text() == upwd){
                        emit user_loginButtonClicked();


                        usersData *usersdata = new usersData();
                        usersdata->uid = query.value(0).toInt();
                        usersdata->username = query.value(1).toString().trimmed();
                        usersdata->password = query.value(2).toString().trimmed();
                        usersdata->realname = query.value(3).toString().trimmed();
                        usersdata->sex = query.value(4).toString().trimmed();
                        usersdata->age = query.value(5).toString().trimmed();
                        usersdata->address = query.value(6).toString().trimmed();
                        usersdata->house_id = query.value(7).toString().trimmed();
                        usersdata->unit_id = query.value(8).toString().trimmed();
                        usersdata->room_id = query.value(9).toString().trimmed();
                        usersdata->userIDcard = query.value(10).toString().trimmed();
                        usersdata->tel = query.value(11).toString().trimmed();
                        usersdata->workplace = query.value(12).toString().trimmed();
                        usersdata->statu = query.value(14).toString().trimmed();

                        ui->pushButton_login_2->setUserData(Qt::UserRole,usersdata);

                        emit sender_userInfo(ui->pushButton_login_2);

                        //获取ip
                        QString lastIp = Widget::getLocalmachineIP();
                        //获取最后登录时间
                        QDateTime locale(QDateTime::currentDateTime());
                        QString localTime = locale.toString("yyyy-MM-dd hh:mm:ss");

                        //判断登陆者是否存在于log表，若是，则更新数据，若否，则插入数据
                        query.exec(tr("select * from log where log_name = '%1'")
                                   .arg(usersdata->realname));
                        if(query.next()){//存在
                            query.exec(tr("update log set log_counts = log_counts + 1,last_date = '%2',last_ip = '%3' where log_name = '%4'")
                                       .arg(localTime).arg(lastIp).arg(usersdata->realname));
                        } else {
                            //不存在
                            query.exec(tr("insert into log (last_ip,last_date,log_name,log_counts,log_play) values('%1','%2','%3',1,'%4')")
                                       .arg(lastIp).arg(localTime).arg(usersdata->realname).arg("用户"));
                        }

                        //将登录信息存入用户登录日志表userLogs
                        query.exec(tr("insert into userLogs values(0,%1,'%2','%3','%4')")
                                   .arg(usersdata->uid).arg(usersdata->realname).arg(localTime).arg(lastIp));


                        //清空登陆信息
                        ui->lineEdit_name_2->clear();
                        ui->lineEdit_pwd_2->clear();
                        close(); //关闭登录界面
                    }else {
                        QMessageBox::warning(this,"错误警告！",
                                             "密码错误！",
                                             QMessageBox::Ok);
                        ui->lineEdit_pwd_2->clear();
                        ui->lineEdit_pwd_2->setFocus();
                    }
                }

            }else {
                QMessageBox::warning(this,"错误警告！",
                                     "用户不存在！",
                                     QMessageBox::Ok);
                ui->lineEdit_name_2->clear();
                ui->lineEdit_name_2->setFocus();
            }
        }

    } else {
        QMessageBox::warning(this,"登陆警告！",
                                    "请选择登陆身份（管理员/业主）！",
                                    QMessageBox::Ok);
    }

    ConnectionPool::closeConnection(db);
}

void Widget::on_pushButton_quit_2_clicked()
{
    QApplication::quit();
}

void Widget::setAdminObject(admin_MainWindow *adminObject)
{
    connect(this, SIGNAL(sender_loginInfo(QPushButton*)), adminObject, SLOT(receveData(QPushButton*)));
    connect(this,SIGNAL(sender_xqManagerInfo(QPushButton*)),adminObject,SLOT(receveXqManagerData(QPushButton*)));
}

void Widget::setUserObject(user_MainWindow *userObject)
{
    connect(this,SIGNAL(sender_userInfo(QPushButton*)),userObject,SLOT(receveUsersData(QPushButton*)));

}

void Widget::on_pushButton_zhuce_2_clicked()
{
    //注册
    if(ui->radioButton_3->isChecked()){//管理员
        emit admin_signUp();
    }else if (ui->radioButton_4->isChecked()) {//业主
        emit user_signUp();
    }else {
        QMessageBox::warning(this,"操作提示！",
                                    "请选择注册身份（管理员/业主）！",
                                    QMessageBox::Ok);
    }

}
