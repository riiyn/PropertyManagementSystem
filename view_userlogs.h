#ifndef VIEW_USERLOGS_H
#define VIEW_USERLOGS_H

#include <QMainWindow>
#include <QSqlTableModel>

namespace Ui {
class view_UserLogs;
}

class view_UserLogs : public QMainWindow
{
    Q_OBJECT

public:
    explicit view_UserLogs(QWidget *parent = 0);
    ~view_UserLogs();

private slots:
    void on_pushButton_serch_clicked();

    void on_pushButton_showall_clicked();

    void on_pushButton_out_2_clicked();

    void userLogs_receiveUidFromUser(int uidToUserLogs);

private:
    Ui::view_UserLogs *ui;
    QSqlTableModel *userlogsModel;
};

#endif // VIEW_USERLOGS_H
