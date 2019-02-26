#ifndef ADD_ROOM_H
#define ADD_ROOM_H

#include <QMainWindow>
#include <QSqlTableModel>

namespace Ui {
class add_room;
}

class add_room : public QMainWindow
{
    Q_OBJECT

public:
    explicit add_room(QWidget *parent = 0);
    ~add_room();

private slots:
    void on_pushButton_doadd_clicked();

    void on_pushButton_add_clicked();

private:
    Ui::add_room *ui;
    QSqlTableModel *addRoomModel;

};

#endif // ADD_ROOM_H
