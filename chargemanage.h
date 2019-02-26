#ifndef CHARGEMANAGE_H
#define CHARGEMANAGE_H

#include <QMainWindow>
#include <QSqlTableModel>

namespace Ui {
class chargeManage;
}

class chargeManage : public QMainWindow
{
    Q_OBJECT

public:
    explicit chargeManage(QWidget *parent = 0);
    ~chargeManage();

private slots:
    void on_pushButton_add_2_clicked();

    void on_pushButton_update_2_clicked();

    void on_pushButton_rollbackupdate_clicked();

    void on_pushButton_delete_2_clicked();

    void on_pushButton_out_2_clicked();

    void on_pushButton_serch_clicked();

    void on_pushButton_showall_clicked();

private:
    Ui::chargeManage *ui;
    QSqlTableModel *chargeModel;
};

#endif // CHARGEMANAGE_H
