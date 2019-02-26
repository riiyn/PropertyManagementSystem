#ifndef LOG_MANAGE_H
#define LOG_MANAGE_H

#include <QMainWindow>
#include <QSqlTableModel>

namespace Ui {
class log_Manage;
}

class log_Manage : public QMainWindow
{
    Q_OBJECT

public:
    explicit log_Manage(QWidget *parent = 0);
    ~log_Manage();

private slots:
    void on_pushButton_serch_clicked();

    void on_pushButton_showall_clicked();

    void on_pushButton_out_2_clicked();

private:
    Ui::log_Manage *ui;
    QSqlTableModel *logModel;
};

#endif // LOG_MANAGE_H
