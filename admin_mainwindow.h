#ifndef ADMIN_MAINWINDOW_H
#define ADMIN_MAINWINDOW_H

#include "usermanager.h"
#include "housemanage.h"
#include "carmanage.h"
#include "mattermange.h"
#include "chargemanage.h"
#include "pay_manage.h"
#include "complaint_manage.h"
#include "log_manage.h"
#include "xiaoqu_manage.h"
#include "userfamily_manage.h"
#include "room_manage.h"
#include "backupsql.h"

#include <QMainWindow>
#include <QPushButton>
#include <QGroupBox>

namespace Ui {
class admin_MainWindow;
}

class admin_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit admin_MainWindow(QWidget *parent = 0);
    ~admin_MainWindow();

//    void SetText(const QString & strText);


private slots:
    void on_pushButton_reback_clicked();


    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_22_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_clicked();

    void on_pushButton_24_clicked();

signals:
    void reback_click();
    void sendr_adminUpdateInfo(QString);
    void sender_xqManagerUpdateInfo(QString);

private:
    Ui::admin_MainWindow *ui;

public slots:
    void timeUpdate(void);
    void receveData(QPushButton *);
    void receveXqManagerData(QPushButton *);

private:
    userManager *usermage;
    carManage *car;
    houseManage *house;
    matterMange *matter;
    chargeManage *charge;
    pay_Manage *pay;
    complaint_Manage *complaint;
    log_Manage *log;
    xiaoqu_Manage *xiaoqu;
    userFamily_Manage *userFamily;
    room_Manage *room;
    backupSQL *backupsql;


};

#endif // ADMIN_MAINWINDOW_H
