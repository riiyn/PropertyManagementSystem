#ifndef USERFAMILYINFO_H
#define USERFAMILYINFO_H

#include <QMainWindow>
#include <QSqlTableModel>

namespace Ui {
class userFamilyInfo;
}

class userFamilyInfo : public QMainWindow
{
    Q_OBJECT

public:
    explicit userFamilyInfo(QWidget *parent = 0);
    ~userFamilyInfo();

private slots:
    void on_pushButton_add_2_clicked();

    void on_pushButton_update_2_clicked();

    void on_pushButton_rollbackupdate_clicked();

    void on_pushButton_delete_2_clicked();

    void on_pushButton_out_2_clicked();

    void on_pushButton_serch_clicked();

    void on_pushButton_showall_clicked();

    void receveUidFromUser(int UID);

private:
    Ui::userFamilyInfo *ui;
    QSqlTableModel *userfamModel;
};

#endif // USERFAMILYINFO_H
