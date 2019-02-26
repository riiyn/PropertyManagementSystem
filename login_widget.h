#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QObject>
#include <QPushButton>

#include "user_mainwindow.h"
#include "admin_mainwindow.h"

namespace Ui {
class Widget;

}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
    QObject *sender() const;

private slots:
    void on_pushButton_login_2_clicked();

    void on_pushButton_quit_2_clicked();

    void on_pushButton_zhuce_2_clicked();

private:
    Ui::Widget *ui;
    QString getLocalmachineIP();

signals:
    void admin_loginButtonClicked(); //管理员登录按钮点击信号
    void user_loginButtonClicked(); //用户登陆按钮点击信号
    void sender_loginInfo(QPushButton *); //发送登陆信息
    void sender_userInfo(QPushButton *);
    void sender_xqManagerInfo(QPushButton *);

    void admin_signUp();//管理员注册
    void user_signUp();//用户注册
//    void together();//线程同步

public:
    void setAdminObject(admin_MainWindow * adminObject);
    void setUserObject(user_MainWindow * userObject);

private:
    admin_MainWindow* m_AdmwinDialog;

};

struct adminData : QObjectUserData{
    int aid;
    QString aname;
    QString apassword;
    QString arealname;
    QString tel;
    QString status;
};
Q_DECLARE_METATYPE(adminData)

struct usersData : QObjectUserData{
    int uid;
    QString username;
    QString password;
    QString realname;
    QString sex;
    QString age;
    QString address;
    QString house_id;
    QString unit_id;
    QString room_id;
    QString userIDcard;
    QString tel;
    QString workplace;
    QString statu;
};
Q_DECLARE_METATYPE(usersData)

struct xqManagerData : QObjectUserData{
    int xqid;
    QString xqname;
    QString name;
    QString sex;
    QString tel;
    QString username;
    QString pwd;
    QString acl;
    QString xqtext;
    QString op_time;
};
Q_DECLARE_METATYPE(xqManagerData)

#endif // WIDGET_H
