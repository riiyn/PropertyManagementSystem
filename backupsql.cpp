#include "backupsql.h"
#include "ui_backupsql.h"

#include <QFileDialog>
#include <QProcess>
#include <QDesktopServices>
#include <QMessageBox>
#include <QSettings>

backupSQL::backupSQL(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::backupSQL)
{
    ui->setupUi(this);
}

backupSQL::~backupSQL()
{
    delete ui;
}

void backupSQL::on_pushButton_clicked()
{
    //备份数据库
    QString name = ui->lineEdit_count->text();
    QString pwd = ui->lineEdit_pwd->text();
    QSettings *configIniRead = new QSettings(":/config/databaseConfig.ini",QSettings::IniFormat);
    QString user_name = configIniRead->value("/databaseConfig/username").toString();
    QString password = configIniRead->value("/databaseConfig/password").toString();

    if(name == user_name){
        if(pwd == password){
            QString Cmd = QString("mysqldump.exe --add-drop-table -u%1 -p%2 test")
                    .arg(name,pwd);
            QString fileName = QFileDialog::getSaveFileName(this,
                                                            "备份数据库",
                                                            QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                            "sql 文件(*.Sql)");//选择保存路径
            if(fileName.isEmpty()){
                return;
            }else {
                QString path = QString("%1").arg(fileName);
                QProcess *pro = new QProcess;
                pro->setStandardOutputFile(path);
                pro->start(Cmd);
                QMessageBox::information(this,"提示",
                                         "数据库已成功备份！");
            }
        }else {
            QMessageBox::warning(this,"错误！",
                                 "数据库登录密码输入错误！请重新输入！");
        }
    }else {
        QMessageBox::warning(this,"错误！",
                             "数据库登录账户输入错误！请重新输入！");
    }
}
