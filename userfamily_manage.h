#ifndef USERFAMILY_MANAGE_H
#define USERFAMILY_MANAGE_H

#include <QMainWindow>
#include <QSqlRelationalTableModel>

namespace Ui {
class userFamily_Manage;
}

class userFamily_Manage : public QMainWindow
{
    Q_OBJECT

public:
    explicit userFamily_Manage(QWidget *parent = 0);
    ~userFamily_Manage();

private slots:
    void on_pushButton_delete_clicked();

    void on_pushButton_out_clicked();

    void on_pushButton_serch_clicked();

    void on_pushButton_showall_clicked();

private:
    Ui::userFamily_Manage *ui;
    QSqlRelationalTableModel *userFamilyModel;
};

#endif // USERFAMILY_MANAGE_H
