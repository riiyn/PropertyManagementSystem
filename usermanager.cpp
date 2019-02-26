#include "usermanager.h"
#include "ui_usermanager.h"
#include "connectionpool.h"
#include "export_excel.h"

#include <QDebug>
#include <QMessageBox>
#include <QSqlError>

userManager::userManager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::userManager)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    myPort = new QSerialPort(this);

    //查询用户数据
    QSqlDatabase db = ConnectionPool::openConnection();

    userModel= new QSqlTableModel(this,db);
    userModel->setTable("user");
    //设置编辑策略，由QSqlTableModel::EditStrategy枚举类型定义，一共有三个值。
    //用来说明当数据库中的值被编辑后，什么情况下提交修改。
    //QSqlTableModel::OnManualSubmit ---->所有改变在模型中缓存，直到调用submitAll()或revertAll()
    userModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    userModel->setHeaderData(0,Qt::Horizontal,"用户ID");
    userModel->setHeaderData(1,Qt::Horizontal,"登录名");
    userModel->setHeaderData(2,Qt::Horizontal,"登陆密码");
    userModel->setHeaderData(3,Qt::Horizontal,"用户姓名");
    userModel->setHeaderData(4,Qt::Horizontal,"性别");
    userModel->setHeaderData(5,Qt::Horizontal,"年龄");
    userModel->setHeaderData(6,Qt::Horizontal,"住址");
    userModel->setHeaderData(7,Qt::Horizontal,"楼号");
    userModel->setHeaderData(8,Qt::Horizontal,"单元号");
    userModel->setHeaderData(9,Qt::Horizontal,"房间号");
    userModel->setHeaderData(10,Qt::Horizontal,"身份证号");
    userModel->setHeaderData(11,Qt::Horizontal,"联系电话");
    userModel->setHeaderData(12,Qt::Horizontal,"工作地");
    userModel->setHeaderData(13,Qt::Horizontal,"用户卡号");
    userModel->setHeaderData(14,Qt::Horizontal,"用户状态");
    userModel->select();//选取整个表的所有行

    ui->tableView->setModel(userModel);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置行高列宽根据内容自动调整
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();

    //使其不可编辑
//    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ConnectionPool::closeConnection(db);

}

userManager::~userManager()
{
    delete ui;
}

void userManager::on_pushButton_update_clicked()
{
    //提交修改
    userModel->database().transaction(); //開啟事務操作

    if(userModel->submitAll()){
        userModel->database().commit(); //提交
        QMessageBox::information(this,"操作提示",
                                 "修改成功！");
    } else {
        userModel->database().rollback(); //回滾
        QMessageBox::warning(this,"操作提示",
                             tr("修改失败：")
                             .arg(userModel->lastError().text()),
                             QMessageBox::Ok);
    }

}

void userManager::on_pushButton_rollbackupdate_clicked()
{
    //撤销修改
    userModel->revertAll();
}

void userManager::on_pushButton_serch_clicked()
{
    //查询
    QString name = ui->lineEdit_name->text();
    int uid = ui->lineEdit_userID->text().toInt();
    QString statu = ui->lineEdit_userStatu->text();

    if(name != NULL){
        userModel->setFilter(QString("realname = '%1'").arg(name));
    } else if (uid != NULL) {
        userModel->setFilter(QString("uid = %1").arg(uid));
    } else if (statu != NULL) {
        userModel->setFilter(QString("statu = '%1'").arg(statu));
    } else {
        QMessageBox::warning(this,"操作提示",
                             "请输入查询依据！",
                             QMessageBox::Ok);
    }

    userModel->select();
    ui->lineEdit_name->clear();
    ui->lineEdit_userID->clear();
    ui->lineEdit_userStatu->clear();

}

void userManager::on_pushButton_showall_clicked()
{
    //显示全表
    //从新关联表，才能再次显示整个表
    userModel->setTable("user");
    userModel->select();
    userModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    userModel->setHeaderData(0,Qt::Horizontal,"用户ID");
    userModel->setHeaderData(1,Qt::Horizontal,"登录名");
    userModel->setHeaderData(2,Qt::Horizontal,"登陆密码");
    userModel->setHeaderData(3,Qt::Horizontal,"用户姓名");
    userModel->setHeaderData(4,Qt::Horizontal,"性别");
    userModel->setHeaderData(5,Qt::Horizontal,"年龄");
    userModel->setHeaderData(6,Qt::Horizontal,"住址");
    userModel->setHeaderData(7,Qt::Horizontal,"楼号");
    userModel->setHeaderData(8,Qt::Horizontal,"单元号");
    userModel->setHeaderData(9,Qt::Horizontal,"房间号");
    userModel->setHeaderData(10,Qt::Horizontal,"身份证号");
    userModel->setHeaderData(11,Qt::Horizontal,"联系电话");
    userModel->setHeaderData(12,Qt::Horizontal,"工作地");
    userModel->setHeaderData(13,Qt::Horizontal,"用户卡号");
    userModel->setHeaderData(14,Qt::Horizontal,"用户状态");
}

void userManager::on_pushButton_delete_clicked()
{
    //删除操作

    //获取选中的行
    int currentRow = ui->tableView->currentIndex().row();

    if(currentRow >= 0){
        //删除该行
        userModel->removeRow(currentRow);

        int ok = QMessageBox::warning(this,"删除当前行！",
                                      "你确定删除当前行吗？",
                                      QMessageBox::Yes,
                                      QMessageBox::No);
        if(ok == QMessageBox::No){
            userModel->revertAll();//如果不删除，则撤销
        } else {
            userModel->submitAll();//否则删除该行，数据库中对应数据删除
        }
    } else {
        QMessageBox::warning(this,"操作提示！",
                             "请选择要删除的行！",
                             QMessageBox::Ok);
    }

}

void userManager::on_pushButton_add_clicked()
{
    //添加数据
    int rowNum = userModel->rowCount();//获得表的行数
    userModel->insertRow(rowNum);//添加一行

//    userModel->submitAll();//可直接提交修改
}

void userManager::on_pushButton_IC_clicked()
{
    //读取IC卡
    //设置串口号
    myPort->setPortName("COM11");
    if(myPort->open(QIODevice::ReadWrite)){
        //波特率
        myPort->setBaudRate(QSerialPort::Baud9600);
        //数据位
        myPort->setDataBits(QSerialPort::Data8);
        //校验位
        myPort->setParity(QSerialPort::NoParity);
        //流控制
        myPort->setFlowControl(QSerialPort::NoFlowControl);
        //停止位
        myPort->setStopBits(QSerialPort::OneStop);

        //读取串口数据
        connect(myPort,SIGNAL(readyRead()),this,SLOT(readPortData()));
    }else {
        QMessageBox::warning(this,"错误",
                             "串口没有打开");
    }
    //第一次握手
    QString str1 = "23";
    QByteArray sendData1;
    StringToHex(str1,sendData1);;
    myPort->write(sendData1);
}

void userManager::readPortData(){
    temp = myPort->readAll();
    QDataStream out(&temp,QIODevice::ReadWrite);
    while (!out.atEnd()) {
        qint8 outChar = 0;
        out >> outChar;
        str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));

        if(str == "45"){
            //发送命令读取卡序列号
            QString command1 = "ab 00 ab";
            QByteArray sendData2;
            StringToHex(command1,sendData2);
            myPort->write(sendData2);
        }else if (str == "67") {
            //反向验证
            QString command2 = "89";
            QByteArray sendData3;
            StringToHex(command2,sendData3);
            myPort->write(sendData3);
        }else if (str == "01") {
            QMessageBox::warning(this,"错误！",
                                 "未检测到卡！");
            myPort->close();
        }else {
            if(str == "be"){
                QString ICNum = "C3ECC055";
                userModel->setTable("user");
                userModel->setFilter(tr("IC_Num = '%1'").arg(ICNum));
                userModel->select();
                userModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
                userModel->setHeaderData(0,Qt::Horizontal,"用户ID");
                userModel->setHeaderData(1,Qt::Horizontal,"登录名");
                userModel->setHeaderData(2,Qt::Horizontal,"登陆密码");
                userModel->setHeaderData(3,Qt::Horizontal,"用户姓名");
                userModel->setHeaderData(4,Qt::Horizontal,"性别");
                userModel->setHeaderData(5,Qt::Horizontal,"年龄");
                userModel->setHeaderData(6,Qt::Horizontal,"住址");
                userModel->setHeaderData(7,Qt::Horizontal,"楼号");
                userModel->setHeaderData(8,Qt::Horizontal,"单元号");
                userModel->setHeaderData(9,Qt::Horizontal,"房间号");
                userModel->setHeaderData(10,Qt::Horizontal,"身份证号");
                userModel->setHeaderData(11,Qt::Horizontal,"联系电话");
                userModel->setHeaderData(12,Qt::Horizontal,"工作地");
                userModel->setHeaderData(13,Qt::Horizontal,"用户卡号");
                userModel->setHeaderData(14,Qt::Horizontal,"用户状态");

                myPort->close();
            }else if (str == "df") {
                QString ICNum = "4708C155";
                userModel->setTable("user");
                userModel->setFilter(tr("IC_Num = '%1'").arg(ICNum));
                userModel->select();
                userModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
                userModel->setHeaderData(0,Qt::Horizontal,"用户ID");
                userModel->setHeaderData(1,Qt::Horizontal,"登录名");
                userModel->setHeaderData(2,Qt::Horizontal,"登陆密码");
                userModel->setHeaderData(3,Qt::Horizontal,"用户姓名");
                userModel->setHeaderData(4,Qt::Horizontal,"性别");
                userModel->setHeaderData(5,Qt::Horizontal,"年龄");
                userModel->setHeaderData(6,Qt::Horizontal,"住址");
                userModel->setHeaderData(7,Qt::Horizontal,"楼号");
                userModel->setHeaderData(8,Qt::Horizontal,"单元号");
                userModel->setHeaderData(9,Qt::Horizontal,"房间号");
                userModel->setHeaderData(10,Qt::Horizontal,"身份证号");
                userModel->setHeaderData(11,Qt::Horizontal,"联系电话");
                userModel->setHeaderData(12,Qt::Horizontal,"工作地");
                userModel->setHeaderData(13,Qt::Horizontal,"用户卡号");
                userModel->setHeaderData(14,Qt::Horizontal,"用户状态");

                myPort->close();
            }
        }
    }
}

void userManager::StringToHex(QString str, QByteArray &sendData){
    int hexData,lowHexData;
    int hexDataLen = 0;
    int len = str.length();
    sendData.resize(len / 2);
    char lstr,hstr;
    for(int i = 0; i < len;){
        hstr = str[i].toLatin1();
        if(hstr == ' '){
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexData = ConvertHexChar(hstr);
        lowHexData = ConvertHexChar(lstr);
        if((hexData == 16) || (lowHexData == 16))
            break;
        else
            hexData = hexData * 16 + lowHexData;
        i++;
        sendData[hexDataLen] = (char)hexData;
        hexDataLen++;
    }
    sendData.resize(hexDataLen);
}

char userManager::ConvertHexChar(char ch){
    if((ch >= '0') && (ch <= '9')){
        return ch - 0x30;
    }else if ((ch >= 'A') && (ch <= 'F')) {
        return ch - 'A' + 10;
    }else if ((ch >= 'a') && (ch <= 'f')) {
        return ch - 'a' + 10;
    }else {
        return(-1);
    }
}

void userManager::on_pushButton_out_clicked()
{
    //导出Excel
    export_Excel::tableToExcel(ui->tableView,"用户数据（usersData）");
}
