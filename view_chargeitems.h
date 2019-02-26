#ifndef VIEW_CHARGEITEMS_H
#define VIEW_CHARGEITEMS_H

#include <QMainWindow>
#include <QSqlTableModel>

namespace Ui {
class view_ChargeItems;
}

class view_ChargeItems : public QMainWindow
{
    Q_OBJECT

public:
    explicit view_ChargeItems(QWidget *parent = 0);
    ~view_ChargeItems();

private slots:
    void on_pushButton_serch_clicked();

    void on_pushButton_showall_clicked();

    void on_pushButton_out_2_clicked();

private:
    Ui::view_ChargeItems *ui;
    QSqlTableModel *chargeitemsModel;
};

#endif // VIEW_CHARGEITEMS_H
