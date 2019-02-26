#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSerialPort>

namespace Ui {
class userManager;
}

class userManager : public QMainWindow
{
    Q_OBJECT

public:
    explicit userManager(QWidget *parent = 0);
    ~userManager();


private slots:
    void on_pushButton_update_clicked();

    void on_pushButton_rollbackupdate_clicked();

    void on_pushButton_serch_clicked();

    void on_pushButton_showall_clicked();

    void on_pushButton_delete_clicked();

    void on_pushButton_add_clicked();

    void on_pushButton_IC_clicked();

    void on_pushButton_out_clicked();

    void readPortData();
    void StringToHex(QString str, QByteArray &sendData);
    char ConvertHexChar(char ch);

private:
    Ui::userManager *ui;
    QSqlTableModel *userModel;
    QSerialPort *myPort;
    QByteArray temp;
    QString str;


};

#endif // USERMANAGER_H
