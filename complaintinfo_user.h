#ifndef COMPLAINTINFO_USER_H
#define COMPLAINTINFO_USER_H

#include "complaints_managebyuser.h"

#include <QMainWindow>

namespace Ui {
class complaintInfo_User;
}

class complaintInfo_User : public QMainWindow
{
    Q_OBJECT

public:
    explicit complaintInfo_User(QWidget *parent = 0);
    ~complaintInfo_User();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void receveUidFromUser(int uid);

signals:
    void senderUidToComplaintMbyU(int);

private:
    Ui::complaintInfo_User *ui;
    complaints_ManageByUser *complaintManageByUser;
};

#endif // COMPLAINTINFO_USER_H
