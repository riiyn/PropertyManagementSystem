#ifndef USER_MAINWINDOW_H
#define USER_MAINWINDOW_H

#include "xqinfo_user.h"
#include "carinfo_user.h"
#include "complaintinfo_user.h"
#include "kftel_dialog.h"
#include "userfamilyinfo.h"
#include "changeuserpwd.h"
#include "view_chargeitems.h"
#include "view_userlogs.h"
#include "pay_dialog.h"

#include <QMainWindow>
#include <QPushButton>
#include <QDateTime>

namespace Ui {
class user_MainWindow;
}

class user_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit user_MainWindow(QWidget *parent = 0);
    ~user_MainWindow();

private:
    Ui::user_MainWindow *ui;

private:
    user_MainWindow *userWin;
    xqInfo_User *xqInfo;
    carInfo_User *carInfo;
    complaintInfo_User *complaintInfo;
    kfTel_Dialog *kftelInfo;
    userFamilyInfo *userfamInfo;
    changeUserPWD *changepwdInfo;
    view_ChargeItems *viewChargeItems;
    view_UserLogs *viewUserLogs;
    pay_Dialog *payDialog;

signals:
    void reback();

    void senderUnameToCar(QString);
    void senderUidToComplaint(int);
    void senderUidToUserFamily(int);
    void senderUidToChangePwd(int);
    void senderUidToUserLogs(int);

//    void senderUserName(QString);

private slots:
    void on_pushButton_reback_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_12_clicked();

public slots:
    void timeUpdate(void);
    void receveUsersData(QPushButton *);
};

struct pay : QObjectUserData{
    int pid;
    QString cha_name;
    float cha_standard;
    float preal;
    float pbalance;
    QDateTime cha_time;
    int uid;
    QString pstate;
    QString pmonth;
};
Q_DECLARE_METATYPE(pay)


#endif // USER_MAINWINDOW_H
