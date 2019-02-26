#include "user_mainwindow.h"
#include "ui_user_mainwindow.h"
#include "login_widget.h"
#include "connectionpool.h"

#include <QSplitter>
#include <QTimer>
#include <QPushButton>
#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>

QMainWindow *userQmd;

user_MainWindow::user_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::user_MainWindow)

{
    ui->setupUi(this);

    payDialog = new pay_Dialog(this);
    connect(ui->pushButton_3,&QPushButton::clicked,payDialog,&pay_Dialog::show);

    userQmd = new QMainWindow(this);

    ui->label_age->hide();
    ui->label_IDC->hide();
    ui->label_statu->hide();
    ui->label_WORK->hide();
    ui->label_pwd->hide();
    ui->lineEdit_age->hide();
    ui->lineEdit_work->hide();
    ui->lineEdit_idc->hide();
    ui->lineEdit_ststu->hide();
    ui->lineEdit_pwd->hide();

    //设置状态栏
    QLabel *per = new QLabel(this);
    per->setFrameStyle(QFrame::Box | QFrame::Sunken);
    per->setText("开发者：张富云   学校：山东建筑大学   学号：20140815020");
    per->setMinimumSize(400,30);
    ui->statusbar->addPermanentWidget(per);

    //获取系统当前时间并实时更新
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeUpdate()));
    timer->start(1000); //定时器1秒更新一次

    //切分窗口

    QSplitter *mainSpli = new QSplitter(Qt::Vertical,0);
    ui->frame_top->setLayout(ui->horizontalLayout_top);
    mainSpli->addWidget(ui->frame_top);
    mainSpli->setStretchFactor(0,0);

    QSplitter *leftSpli = new  QSplitter(Qt::Horizontal,mainSpli);
    ui->frame_left->setLayout(ui->verticalLayout_left);
    leftSpli->addWidget(ui->frame_left);
    leftSpli->setStretchFactor(0,0);

    QSplitter *centerSpli = new QSplitter(Qt::Horizontal,leftSpli);
    centerSpli->addWidget(userQmd);
    centerSpli->setStretchFactor(0,1);

    QSplitter *rightSpli = new QSplitter(Qt::Horizontal,centerSpli);
    ui->frame_center_2->setLayout(ui->verticalLayout_center_1);
    ui->frame_right->setLayout(ui->verticalLayout_2);
    rightSpli->addWidget(ui->frame_center_2);
    rightSpli->addWidget(ui->frame_right);
    rightSpli->setStretchFactor(0,0);
    rightSpli->setStretchFactor(1,0);

    setCentralWidget(mainSpli);

    xqInfo = new xqInfo_User(0);
    xqInfo->setParent(userQmd);
    userQmd->setCentralWidget(xqInfo);
    xqInfo->show();
    carInfo = new carInfo_User(0);
    complaintInfo = new complaintInfo_User(0);
    kftelInfo = new kfTel_Dialog(0);
    userfamInfo = new userFamilyInfo(0);
    changepwdInfo = new changeUserPWD(0);
    viewChargeItems = new view_ChargeItems(0);
    viewUserLogs = new view_UserLogs(0);

    connect(this,SIGNAL(senderUidToUserLogs(int)),viewUserLogs,SLOT(userLogs_receiveUidFromUser(int)));
    connect(this,SIGNAL(senderUidToChangePwd(int)),changepwdInfo,SLOT(receiveUidByUser(int)));
    connect(this,SIGNAL(senderUnameToCar(QString)),carInfo,SLOT(receiveUserInfo(QString)));
    connect(this,SIGNAL(senderUidToComplaint(int)),complaintInfo,SLOT(receveUidFromUser(int)));
    connect(this,SIGNAL(senderUidToUserFamily(int)),userfamInfo,SLOT(receveUidFromUser(int)));
}

user_MainWindow::~user_MainWindow()
{
    delete ui;
}

void user_MainWindow::timeUpdate()
{
    QDateTime locale(QDateTime::currentDateTime());
    QString localTime = locale.toString("yyyy年MM月dd日 hh:mm:ss");
    ui->lineEdit_time->setText(localTime);
    ui->lineEdit_time->setAlignment(Qt::AlignCenter);
}

void user_MainWindow::receveUsersData(QPushButton *btn){
    usersData *usersdata = (usersData *)(btn->userData(Qt::UserRole));

    QString uid = QString::number(usersdata->uid);
    ui->lineEdit_id->setText(uid);
    ui->lineEdit_username->setText(usersdata->username);
    ui->lineEdit_name->setText(usersdata->realname);
    ui->lineEdit_address->setText(usersdata->address);
    ui->lineEdit_sex->setText(usersdata->sex);
    ui->lineEdit_unitid->setText(usersdata->unit_id);
    ui->lineEdit_housid->setText(usersdata->house_id);
    ui->lineEdit_roomid->setText(usersdata->room_id);
    ui->lineEdit_tel->setText(usersdata->tel);
    ui->lineEdit_age->setText(usersdata->age);
    ui->lineEdit_idc->setText(usersdata->userIDcard);
    ui->lineEdit_work->setText(usersdata->workplace);
    ui->lineEdit_ststu->setText(usersdata->statu);
    ui->lineEdit_pwd->setText(usersdata->password);

    ui->label_title->setText(tr("%1,欢迎使用大麦场！——山东建筑大学毕业设计").arg(usersdata->realname));
    emit senderUidToChangePwd(usersdata->uid);
    emit senderUnameToCar(usersdata->realname);
    emit senderUidToComplaint(usersdata->uid);
    emit senderUidToUserFamily(usersdata->uid);
    emit senderUidToUserLogs(usersdata->uid);

    //显示缴费信息
    QSqlDatabase db =  ConnectionPool::openConnection();
    QSqlQuery query(db);
    query.exec(tr("select sum(cha_standard) yj,sum(preal) sj,SUM(pbalance) qf,pmonth from pay where uid = %1")
               .arg(usersdata->uid));
    query.next();

    ui->textBrowser_10->setUpdatesEnabled(true);
    ui->textBrowser_10->setText(query.value(0).toString().trimmed());
    ui->textBrowser_10->update();
    ui->textBrowser_11->setText(query.value(1).toString().trimmed());
    ui->textBrowser_12->setText(query.value(2).toString().trimmed());
    ui->textBrowser_13->setText(query.value(3).toString().trimmed());

    ConnectionPool::closeConnection(db);
}

void user_MainWindow::on_pushButton_reback_clicked()
{
    emit reback();
    close();
}

/**小区信息
 * @brief user_MainWindow::on_pushButton_4_clicked
 */
void user_MainWindow::on_pushButton_4_clicked()
{
    if(carInfo != NULL || viewUserLogs != NULL || viewChargeItems != NULL 
		|| changepwdInfo != NULL || userfamInfo != NULL || complaintInfo != NULL || kftelInfo != NULL){
        userQmd->takeCentralWidget();
    }

    if(xqInfo != NULL && !xqInfo->isHidden()){
        userQmd->takeCentralWidget();
        xqInfo->hide();
    }
    if (xqInfo != NULL) {
        xqInfo->setParent(userQmd);
        userQmd->setCentralWidget(xqInfo);
        xqInfo->show();
    }
}
/**车辆信息
 * @brief user_MainWindow::on_pushButton_5_clicked
 */
void user_MainWindow::on_pushButton_5_clicked()
{
    if(xqInfo != NULL || viewUserLogs != NULL || viewChargeItems != NULL 
		|| changepwdInfo != NULL || userfamInfo != NULL || complaintInfo != NULL || kftelInfo != NULL){
        userQmd->takeCentralWidget();
    }

    if(carInfo != NULL && !carInfo->isHidden()){
        userQmd->takeCentralWidget();
        carInfo->hide();
    }
    if (carInfo != NULL) {
        carInfo->setParent(userQmd);
        userQmd->setCentralWidget(carInfo);
        carInfo->show();
    }
}

void user_MainWindow::on_pushButton_6_clicked()
{
    //投诉
    if(carInfo != NULL || viewUserLogs != NULL || viewChargeItems != NULL 
		|| changepwdInfo != NULL || userfamInfo != NULL || xqInfo != NULL || kftelInfo != NULL){
        userQmd->takeCentralWidget();
    }

    if(complaintInfo != NULL && !complaintInfo->isHidden()){
        userQmd->takeCentralWidget();
        complaintInfo->hide();
    }
    if (complaintInfo != NULL) {
        complaintInfo->setParent(userQmd);
        userQmd->setCentralWidget(complaintInfo);
        complaintInfo->show();
    }
}

void user_MainWindow::on_pushButton_7_clicked()
{
    //客服
    if(carInfo != NULL || viewUserLogs != NULL || viewChargeItems != NULL 
		|| changepwdInfo != NULL || xqInfo != NULL || userfamInfo != NULL || complaintInfo != NULL){
        userQmd->takeCentralWidget();
    }

    if(kftelInfo != NULL && !kftelInfo->isHidden()){
        userQmd->takeCentralWidget();
        kftelInfo->hide();
    }
    if (kftelInfo != NULL) {
        kftelInfo->setParent(userQmd);
        userQmd->setCentralWidget(kftelInfo);
        kftelInfo->show();
    }
}

void user_MainWindow::on_pushButton_10_clicked()
{
    //家属信息
    if(carInfo != NULL || viewUserLogs != NULL || viewChargeItems != NULL 
		|| changepwdInfo != NULL || kftelInfo != NULL || xqInfo != NULL || complaintInfo != NULL){
        userQmd->takeCentralWidget();
    }

    if(userfamInfo != NULL && !userfamInfo->isHidden()){
        userQmd->takeCentralWidget();
        userfamInfo->hide();
    }
    if (userfamInfo != NULL) {
        userfamInfo->setParent(userQmd);
        userQmd->setCentralWidget(userfamInfo);
        userfamInfo->show();
    }
}

void user_MainWindow::on_pushButton_11_clicked()
{
    //修改登录密码
    if(carInfo != NULL || viewUserLogs != NULL || viewChargeItems != NULL 
		|| userfamInfo != NULL || kftelInfo != NULL || xqInfo != NULL || complaintInfo != NULL){
        userQmd->takeCentralWidget();
    }

    if(changepwdInfo != NULL && !changepwdInfo->isHidden()){
        userQmd->takeCentralWidget();
        changepwdInfo->hide();
    }
    if (changepwdInfo != NULL) {
        changepwdInfo->setParent(userQmd);
        userQmd->setCentralWidget(changepwdInfo);
        changepwdInfo->show();
    }
}

void user_MainWindow::on_pushButton_13_clicked()
{
    //查看收费项目
    if(carInfo != NULL || viewUserLogs != NULL || changepwdInfo != NULL || 
		userfamInfo != NULL || kftelInfo != NULL || xqInfo != NULL || complaintInfo != NULL){
        userQmd->takeCentralWidget();
    }

    if(viewChargeItems != NULL && !viewChargeItems->isHidden()){
        userQmd->takeCentralWidget();
        viewChargeItems->hide();
    }
    if (viewChargeItems != NULL) {
        viewChargeItems->setParent(userQmd);
        userQmd->setCentralWidget(viewChargeItems);
        viewChargeItems->show();
    }
}

void user_MainWindow::on_pushButton_12_clicked()
{
    //查看登录日志
    if(carInfo != NULL || viewChargeItems != NULL || changepwdInfo != NULL 
		|| userfamInfo != NULL || kftelInfo != NULL || xqInfo != NULL || complaintInfo != NULL){
        userQmd->takeCentralWidget();
    }

    if(viewUserLogs!= NULL && !viewUserLogs->isHidden()){
        userQmd->takeCentralWidget();
        viewUserLogs->hide();
    }
    if (viewUserLogs != NULL) {
        viewUserLogs->setParent(userQmd);
        userQmd->setCentralWidget(viewUserLogs);
        viewUserLogs->show();
    }
}

void user_MainWindow::on_pushButton_clicked()
{
    //详细信息
    ui->label_age->show();
    ui->label_IDC->show();
    ui->label_statu->show();
    ui->label_WORK->show();
    ui->lineEdit_age->show();
    ui->lineEdit_work->show();
    ui->lineEdit_idc->show();
    ui->lineEdit_ststu->show();
    ui->lineEdit_pwd->show();
    ui->label_pwd->show();
    ui->lineEdit_username->setFocusPolicy(Qt::StrongFocus);
//    ui->lineEdit_pwd->setFocusPolicy(Qt::StrongFocus);
    ui->lineEdit_name->setFocusPolicy(Qt::StrongFocus);
    ui->lineEdit_address->setFocusPolicy(Qt::StrongFocus);
    ui->lineEdit_age->setFocusPolicy(Qt::StrongFocus);
    ui->lineEdit_housid->setFocusPolicy(Qt::StrongFocus);
    ui->lineEdit_idc->setFocusPolicy(Qt::StrongFocus);
    ui->lineEdit_roomid->setFocusPolicy(Qt::StrongFocus);
    ui->lineEdit_sex->setFocusPolicy(Qt::StrongFocus);
    ui->lineEdit_work->setFocusPolicy(Qt::StrongFocus);
    ui->lineEdit_unitid->setFocusPolicy(Qt::StrongFocus);
    ui->lineEdit_tel->setFocusPolicy(Qt::StrongFocus);
}

void user_MainWindow::on_pushButton_2_clicked()
{
    //修改个人信息

    int uid = ui->lineEdit_id->text().toInt();
    QString username = ui->lineEdit_username->text();
    QString pwd = ui->lineEdit_pwd->text();
    QString name = ui->lineEdit_name->text();
    QString address = ui->lineEdit_address->text();
    int age = ui->lineEdit_age->text().toInt();
    int houseid = ui->lineEdit_housid->text().toInt();
    QString idc = ui->lineEdit_idc->text();
    int roomid = ui->lineEdit_roomid->text().toInt();
    QString sex = ui->lineEdit_sex->text();
    QString work = ui->lineEdit_work->text();
    int unitid = ui->lineEdit_unitid->text().toInt();
    QString tel = ui->lineEdit_tel->text();

    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);

    if(username != NULL && pwd != NULL && name != NULL && address != NULL &&
            age != NULL && houseid != NULL && idc != NULL && roomid != NULL && sex != NULL &&
            work != NULL && unitid != NULL && tel != NULL){
        query.exec(QString("update user set username = '%1',password = '%2',realname = '%3',sex = '%4',age = %5,address = '%6',house_id = %7,unit_id = %8,room_id = %9,userIDcard = '%10',tel = '%11',workplace = '%12' where uid = %13")
                   .arg(username).arg(pwd).arg(name).arg(sex).arg(age).arg(address).arg(houseid).arg(unitid).arg(roomid).arg(idc).arg(tel).arg(work).arg(uid));
        QMessageBox::information(this,"提示！",
                                 "修改成功！");
    } else {
        QMessageBox::warning(this,"警告！",
                             "请输入所有可选项！");
    }
    ConnectionPool::closeConnection(db);
}

void user_MainWindow::on_pushButton_3_clicked()
{
    //缴费
}




