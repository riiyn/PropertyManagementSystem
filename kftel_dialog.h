#ifndef KFTEL_DIALOG_H
#define KFTEL_DIALOG_H

#include <QDialog>

namespace Ui {
class kfTel_Dialog;
}

class kfTel_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit kfTel_Dialog(QWidget *parent = 0);
    ~kfTel_Dialog();

private:
    Ui::kfTel_Dialog *ui;
};

#endif // KFTEL_DIALOG_H
