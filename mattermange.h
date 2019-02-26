#ifndef MATTERMANGE_H
#define MATTERMANGE_H

#include <QMainWindow>
#include <QSqlTableModel>

namespace Ui {
class matterMange;
}

class matterMange : public QMainWindow
{
    Q_OBJECT

public:
    explicit matterMange(QWidget *parent = 0);
    ~matterMange();

private slots:
    void on_pushButton_add_2_clicked();

    void on_pushButton_update_2_clicked();

    void on_pushButton_rollbackupdate_clicked();

    void on_pushButton_delete_2_clicked();

    void on_pushButton_out_2_clicked();

    void on_pushButton_serch_clicked();

    void on_pushButton_showall_clicked();

private:
    Ui::matterMange *ui;
    QSqlTableModel *matterModel;
};

#endif // MATTERMANGE_H
