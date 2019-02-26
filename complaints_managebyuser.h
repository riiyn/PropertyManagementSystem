#ifndef COMPLAINTS_MANAGEBYUSER_H
#define COMPLAINTS_MANAGEBYUSER_H

//#include "user_mainwindow.h"

#include <QMainWindow>
#include <QSqlTableModel>

namespace Ui {
class complaints_ManageByUser;
}

class complaints_ManageByUser : public QMainWindow
{
    Q_OBJECT

public:
    explicit complaints_ManageByUser(QWidget *parent = 0);
    ~complaints_ManageByUser();

//    void setObjectUser(user_MainWindow *userWindow);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

    void receiveUidFromCinfo(int uidfrom);

private:
    Ui::complaints_ManageByUser *ui;
    QSqlTableModel *complaintModel;
};

#endif // COMPLAINTS_MANAGEBYUSER_H
