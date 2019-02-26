#ifndef XIAOQU_MANAGE_H
#define XIAOQU_MANAGE_H

#include <QMainWindow>
#include <QSqlQuery>

namespace Ui {
class xiaoqu_Manage;
}

class xiaoqu_Manage : public QMainWindow
{
    Q_OBJECT

public:
    explicit xiaoqu_Manage(QWidget *parent = 0);
    ~xiaoqu_Manage();

private slots:
    void on_pushButton_update_clicked();


private:
    Ui::xiaoqu_Manage *ui;

};

#endif // XIAOQU_MANAGE_H
