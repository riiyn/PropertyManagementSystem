#ifndef PAY_MANAGE_H
#define PAY_MANAGE_H

#include <QMainWindow>
#include <QSqlRelationalTableModel>

namespace Ui {
class pay_Manage;
}

class pay_Manage : public QMainWindow
{
    Q_OBJECT

public:
    explicit pay_Manage(QWidget *parent = 0);
    ~pay_Manage();

private slots:
    void on_pushButton_search_clicked();

    void on_pushButton_showall_clicked();

    void on_pushButton_out_clicked();

private:
    Ui::pay_Manage *ui;
    QSqlRelationalTableModel *payModel;
};

#endif // PAY_MANAGE_H
