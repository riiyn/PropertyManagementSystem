#include "login_widget.h"
#include "admin_mainwindow.h"
#include "user_mainwindow.h"
#include "connectionpool.h"
#include "admin_update_dialog.h"
#include "admin_signup_dialog.h"
#include "user_signup_dialog.h"
//#include "complaints_managebyuser.h"

#include <QApplication>
#include <QSqlError>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
    admin_Update_Dialog aUD;
    admin_MainWindow admw;
    user_MainWindow usmw;
    admin_SignUp_Dialog adminSD;
    user_SignUp_Dialog userSD;
//    complaints_ManageByUser comByUser;

//    comByUser.setObjectUser(&usmw);
    userSD.setUserSignUpObject(&w);
    adminSD.setAdminSignUpObject(&w);
    aUD.setAdminObject(&admw);
    w.setAdminObject(&admw);
    w.setUserObject(&usmw);
//    w.togetherFunc(&usmw);
    //登录到管理员
    QObject::connect(&w,SIGNAL(admin_loginButtonClicked()),&admw,SLOT(show()));
    //登录到用户
    QObject::connect(&w,SIGNAL(user_loginButtonClicked()),&usmw,SLOT(show()));
    //管理员返回到登录界面
    QObject::connect(&admw,SIGNAL(reback_click()),&w,SLOT(show()));

    //用户返回到登陆界面
    QObject::connect(&usmw,SIGNAL(reback()),&w,SLOT(show()));

    w.show();
    return a.exec();
    ConnectionPool::release();
}
