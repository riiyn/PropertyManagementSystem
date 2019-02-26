#ifndef HOUSEMANAGE_H
#define HOUSEMANAGE_H

#include <QMainWindow>
#include <QSqlTableModel>

namespace Ui {
class houseManage;
}

class houseManage : public QMainWindow
{
    Q_OBJECT

public:
    explicit houseManage(QWidget *parent = 0);
    ~houseManage();

private slots:
    void on_pushButton_add_2_clicked();

    void on_pushButton_update_2_clicked();

    void on_pushButton_rollbackupdate_clicked();

    void on_pushButton_delete_2_clicked();

    void on_pushButton_showall_clicked();

    void on_pushButton_serch_2_clicked();

    void on_pushButton_out_2_clicked();

private:
    Ui::houseManage *ui;
    QSqlTableModel *houseModel;
};

#endif // HOUSEMANAGE_H
