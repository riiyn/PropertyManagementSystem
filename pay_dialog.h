#ifndef PAY_DIALOG_H
#define PAY_DIALOG_H

#include <QDialog>

namespace Ui {
class pay_Dialog;
}

class pay_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit pay_Dialog(QWidget *parent = 0);
    ~pay_Dialog();

private:
    Ui::pay_Dialog *ui;
};

#endif // PAY_DIALOG_H
