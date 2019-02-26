#ifndef ADMIN_UPDATE_DIALOG_H
#define ADMIN_UPDATE_DIALOG_H

#include "admin_mainwindow.h"

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class admin_Update_Dialog;
}

class admin_Update_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit admin_Update_Dialog(QWidget *parent = 0);
    ~admin_Update_Dialog();
    void setAdminObject(admin_MainWindow * adminObject);

private:
    Ui::admin_Update_Dialog *ui;
    QSqlTableModel *updateModel;


private slots:
    void receiveAdminUpdateInfo(QString name);
    void on_pushButton_update_2_clicked();
    void on_pushButton_rollbackupdate_clicked();
};

#endif // ADMIN_UPDATE_DIALOG_H
