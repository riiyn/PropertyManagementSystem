#ifndef CARINFO_USER_H
#define CARINFO_USER_H

#include <QMainWindow>
#include <QSqlTableModel>

namespace Ui {
class carInfo_User;
}

class carInfo_User : public QMainWindow
{
    Q_OBJECT

public:
    explicit carInfo_User(QWidget *parent = 0);
    ~carInfo_User();



private slots:
    void on_pushButton_add_2_clicked();

    void on_pushButton_update_2_clicked();

    void on_pushButton_rollbackupdate_clicked();

    void on_pushButton_delete_2_clicked();

    void on_pushButton_out_2_clicked();

    void on_pushButton_serch_clicked();

    void on_pushButton_showall_clicked();

    void receiveUserInfo(QString name);

private:
    Ui::carInfo_User *ui;
    QSqlTableModel *carInfoMOdel;
};

#endif // CARINFO_USER_H
