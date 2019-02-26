#ifndef ROOM_MANAGE_H
#define ROOM_MANAGE_H

#include "add_room.h"

#include <QMainWindow>
#include <QSqlRelationalTableModel>
#include <QSqlTableModel>

namespace Ui {
class room_Manage;
}

class room_Manage : public QMainWindow
{
    Q_OBJECT

public:
    explicit room_Manage(QWidget *parent = 0);
    ~room_Manage();

private slots:
    void on_pushButton_add_2_clicked();

    void on_pushButton_update_2_clicked();

    void on_pushButton_rollbackupdate_clicked();

    void on_pushButton_delete_2_clicked();

    void on_pushButton_showall_clicked();

    void on_pushButton_serch_2_clicked();

    void on_pushButton_out_2_clicked();

    void on_pushButton_showall_2_clicked();

    void on_pushButton_adduser_clicked();

private:
    Ui::room_Manage *ui;
    QSqlRelationalTableModel *roomModel;
    QSqlTableModel *noEnterRoomModel;
    add_room *addRoom;
};

#endif // ROOM_MANAGE_H
