#ifndef CHANGEUSERPWD_H
#define CHANGEUSERPWD_H

#include <QMainWindow>

namespace Ui {
class changeUserPWD;
}

class changeUserPWD : public QMainWindow
{
    Q_OBJECT

public:
    explicit changeUserPWD(QWidget *parent = 0);
    ~changeUserPWD();

private slots:
    void receiveUidByUser(int uids);

    void on_pushButton_clicked();

private:
    Ui::changeUserPWD *ui;
};

#endif // CHANGEUSERPWD_H
