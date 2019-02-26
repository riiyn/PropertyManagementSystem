#ifndef USER_SIGNUP_DIALOG_H
#define USER_SIGNUP_DIALOG_H

#include "login_widget.h"

#include <QDialog>

namespace Ui {
class user_SignUp_Dialog;
}

class user_SignUp_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit user_SignUp_Dialog(QWidget *parent = 0);
    ~user_SignUp_Dialog();
    void setUserSignUpObject(Widget *widgetObject);

private:
    Ui::user_SignUp_Dialog *ui;

private slots:
    void receiveUserSignUp();
    void on_pushButton_signup_clicked();
};

#endif // USER_SIGNUP_DIALOG_H
