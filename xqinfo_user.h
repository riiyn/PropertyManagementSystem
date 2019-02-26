#ifndef XQINFO_USER_H
#define XQINFO_USER_H

#include <QMainWindow>

namespace Ui {
class xqInfo_User;
}

class xqInfo_User : public QMainWindow
{
    Q_OBJECT

public:
    explicit xqInfo_User(QWidget *parent = 0);
    ~xqInfo_User();

private:
    Ui::xqInfo_User *ui;
};

#endif // XQINFO_USER_H
