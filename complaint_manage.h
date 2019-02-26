#ifndef COMPLAINT_MANAGE_H
#define COMPLAINT_MANAGE_H

#include <QMainWindow>
#include <QSqlRelationalTableModel>
#include <QSqlQueryModel>

namespace Ui {
class complaint_Manage;
}

class complaint_Manage : public QMainWindow
{
    Q_OBJECT

public:
    explicit complaint_Manage(QWidget *parent = 0);
    ~complaint_Manage();

private slots:
    void on_pushButton_search_clicked();

    void on_pushButton_all_clicked();

    void on_pushButton_out_clicked();

//    void on_TableViewColWidth_Changed(const QModelIndex &index,int first,int last);

private:
    Ui::complaint_Manage *ui;
    QSqlRelationalTableModel *complaintModel;
    QSqlQueryModel *queryModel;
};

#endif // COMPLAINT_MANAGE_H
