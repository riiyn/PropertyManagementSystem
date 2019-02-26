#ifndef BACKUPSQL_H
#define BACKUPSQL_H

#include <QMainWindow>

namespace Ui {
class backupSQL;
}

class backupSQL : public QMainWindow
{
    Q_OBJECT

public:
    explicit backupSQL(QWidget *parent = 0);
    ~backupSQL();

private slots:
    void on_pushButton_clicked();

private:
    Ui::backupSQL *ui;
};

#endif // BACKUPSQL_H
