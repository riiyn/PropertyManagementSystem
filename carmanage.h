#ifndef CARMANAGE_H
#define CARMANAGE_H

#include <QMainWindow>
#include <QSqlRelationalTableModel>

namespace Ui {
class carManage;
}

class carManage : public QMainWindow
{
    Q_OBJECT

public:
    explicit carManage(QWidget *parent = 0);
    ~carManage();

private slots:
    void on_pushButton_add_2_clicked();

    void on_pushButton_rollbackupdate_clicked();

    void on_pushButton_update_2_clicked();

    void on_pushButton_serch_clicked();

    void on_pushButton_showall_clicked();

    void on_pushButton_delete_2_clicked();

    void on_pushButton_out_2_clicked();

private:
    Ui::carManage *ui;
    QSqlRelationalTableModel *carModel;
};

#endif // CARMANAGE_H
