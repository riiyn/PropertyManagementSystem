#ifndef ADMIN_SIGNUP_DIALOG_H
#define ADMIN_SIGNUP_DIALOG_H

#include "login_widget.h"

#include <QDialog>

namespace Ui {
class admin_SignUp_Dialog;
}

class admin_SignUp_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit admin_SignUp_Dialog(QWidget *parent = 0);
    ~admin_SignUp_Dialog();
    void setAdminSignUpObject(Widget *widgetObject);

private:
    Ui::admin_SignUp_Dialog *ui;

private slots:
    void receiveAdminSignUp();
    void on_pushButton_signup_clicked();
};

#endif // ADMIN_SIGNUP_DIALOG_H
