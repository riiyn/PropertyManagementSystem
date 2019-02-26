#include "admin_mainwindow.h"
#include "ui_admin_mainwindow.h"
#include "login_widget.h"
#include "connectionpool.h"
#include "usermanager.h"
#include "carmanage.h"
#include "admin_update_dialog.h"

#include <QSplitter>
#include <QTimer>
#include <QObject>
#include <QDebug>

QMainWindow *qmd;

admin_MainWindow::admin_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::admin_MainWindow)
{

    ui->setupUi(this);

    qmd = new QMainWindow(this);
//    qmd->setStyleSheet("border:2px solid rgb(121,121,121)");

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


    QSplitter *leftSpli = new QSplitter(Qt::Horizontal,mainSpli);

    ui->widget_left->setLayout(ui->verticalLayout_left_2);

    leftSpli->addWidget(ui->widget_left);
    leftSpli->setStretchFactor(0,0);

    QSplitter *rightSpli = new QSplitter(Qt::Horizontal,leftSpli);

    ui->frame_right->setLayout(ui->ver_right_2);

    rightSpli->addWidget(qmd);
    rightSpli->addWidget(ui->frame_right);
    rightSpli->setStretchFactor(0,1);
    rightSpli->setStretchFactor(1,0);

    setCentralWidget(mainSpli);

    usermage = new userManager(0);
    usermage->setParent(qmd);
    qmd->setCentralWidget(usermage);
    usermage->show();

    car = new carManage(0);
    house = new houseManage(0);
    matter = new matterMange(0);
    charge = new chargeManage(0);
    pay = new pay_Manage(0);
    complaint = new complaint_Manage(0);
    log = new log_Manage(0);
    xiaoqu = new xiaoqu_Manage(0);
    userFamily = new userFamily_Manage(0);
    room = new room_Manage(0);
    backupsql = new backupSQL(0);

}

admin_MainWindow::~admin_MainWindow()
{
    delete ui;
}

void admin_MainWindow::receveData(QPushButton *btn){
    //接收数据并显示
    adminData *admindata = (adminData *)(btn->userData(Qt::UserRole));

    ui->lineEdit_logname->setText(admindata->aname);
    ui->lineEdit_name->setText(admindata->arealname);
    ui->lineEdit_tel->setText(admindata->tel);

    ui->label_title->setText(tr("%1,欢迎使用大麦场！——山东建筑大学毕业设计").arg(admindata->arealname));
}

void admin_MainWindow::receveXqManagerData(QPushButton *btn){
    //接受小区负责人信息
    xqManagerData *xqmData = (xqManagerData *)(btn->userData(Qt::UserRole));

    ui->lineEdit_logname->setText(xqmData->username);
    ui->lineEdit_name->setText(xqmData->name);
    ui->lineEdit_tel->setText(xqmData->tel);
    ui->label_title->setText(tr("%1,欢迎使用大麦场！————山东建筑大学毕业设计").arg(xqmData->name));
}


void admin_MainWindow::timeUpdate()
{
    QDateTime locale(QDateTime::currentDateTime());
    QString localTime = locale.toString("yyyy年MM月dd日 hh:mm:ss");
    ui->lineEdit->setText(localTime);
    ui->lineEdit->setAlignment(Qt::AlignCenter);
}

void admin_MainWindow::on_pushButton_reback_clicked()
{
    emit reback_click();

    close();
}

/**用户管理
 * @brief admin_MainWindow::on_pushButton_13_clicked
 */
void admin_MainWindow::on_pushButton_13_clicked()
{
    if(car != NULL || backupsql != NULL || room != NULL || userFamily != NULL || xiaoqu != NULL || log != NULL || complaint != NULL || house != NULL || matter != NULL || charge != NULL || pay != NULL){
        qmd->takeCentralWidget();
    }

    if(usermage != NULL && !usermage->isHidden()){
        qmd->takeCentralWidget();
        usermage->hide();
    }
    if (usermage != NULL) {
        usermage->setParent(qmd);
        qmd->setCentralWidget(usermage);
        usermage->show();
    }

}

/**车辆管理
 * @brief admin_MainWindow::on_pushButton_14_clicked
 */
void admin_MainWindow::on_pushButton_14_clicked()
{
    if(usermage != NULL || backupsql != NULL || room != NULL || userFamily != NULL || xiaoqu != NULL || log != NULL || complaint != NULL || house != NULL || matter != NULL || charge != NULL || pay != NULL){
        qmd->takeCentralWidget();
    }

    if(car != NULL && !car->isHidden()){
        qmd->takeCentralWidget();
        car->hide();
    }
    if(car != NULL) {
        car->setParent(qmd);
        qmd->setCentralWidget(car);
        car->show();
    }

}

/**楼宇管理
 * @brief admin_MainWindow::on_pushButton_15_clicked
 */
void admin_MainWindow::on_pushButton_15_clicked()
{
    if(usermage != NULL || backupsql != NULL || room != NULL || userFamily != NULL || xiaoqu != NULL || log != NULL || complaint != NULL || car != NULL || matter != NULL || charge != NULL || pay != NULL){
        qmd->takeCentralWidget();
    }

    if(house != NULL && !house->isHidden()){
        qmd->takeCentralWidget();
        house->hide();
    }
    if(house != NULL) {
        house->setParent(qmd);
        qmd->setCentralWidget(house);
        house->show();
    }
}

/**设备管理
 * @brief admin_MainWindow::on_pushButton_16_clicked
 */
void admin_MainWindow::on_pushButton_16_clicked()
{
    if(usermage != NULL || backupsql != NULL || room != NULL || userFamily != NULL || xiaoqu != NULL || log != NULL || complaint != NULL || car != NULL || house != NULL || charge != NULL || pay != NULL){
        qmd->takeCentralWidget();
    }

    if(matter != NULL && !matter->isHidden()){
        qmd->takeCentralWidget();
        matter->hide();
    }
    if(matter != NULL) {
        matter->setParent(qmd);
        qmd->setCentralWidget(matter);
        matter->show();
    }
}

/**收费项目管理
 * @brief admin_MainWindow::on_pushButton_17_clicked
 */
void admin_MainWindow::on_pushButton_17_clicked()
{
    if(usermage != NULL || backupsql != NULL || room != NULL || userFamily != NULL || xiaoqu != NULL || log != NULL || complaint != NULL || car != NULL || house != NULL || matter != NULL || pay != NULL){
        qmd->takeCentralWidget();
    }

    if(charge != NULL && !charge->isHidden()){
        qmd->takeCentralWidget();
        charge->hide();
    }
    if(charge != NULL) {
        charge->setParent(qmd);
        qmd->setCentralWidget(charge);
        charge->show();
    }
}

/**支付管理
 * @brief admin_MainWindow::on_pushButton_18_clicked
 */
void admin_MainWindow::on_pushButton_18_clicked()
{
    if(usermage != NULL || backupsql != NULL || room != NULL || userFamily != NULL || xiaoqu != NULL || log != NULL || car != NULL || house != NULL || matter != NULL || charge != NULL || complaint != NULL){
        qmd->takeCentralWidget();
    }

    if(pay != NULL && !pay->isHidden()){
        qmd->takeCentralWidget();
        pay->hide();
    }
    if(pay != NULL) {
        pay->setParent(qmd);
        qmd->setCentralWidget(pay);
        pay->show();
    }
}

/**投诉管理
 * @brief admin_MainWindow::on_pushButton_19_clicked
 */
void admin_MainWindow::on_pushButton_19_clicked()
{
    if(usermage != NULL || backupsql != NULL || room != NULL || userFamily != NULL || xiaoqu != NULL || log != NULL || car != NULL || house != NULL || matter != NULL || charge != NULL || pay != NULL){
        qmd->takeCentralWidget();
    }

    if(complaint != NULL && !complaint->isHidden()){
        qmd->takeCentralWidget();
        complaint->hide();
    }
    if(complaint != NULL) {
        complaint->setParent(qmd);
        qmd->setCentralWidget(complaint);
        complaint->show();
    }
}

/**登陆日志
 * @brief admin_MainWindow::on_pushButton_20_clicked
 */
void admin_MainWindow::on_pushButton_20_clicked()
{
    if(usermage != NULL || backupsql != NULL || room != NULL || userFamily != NULL || xiaoqu != NULL || complaint != NULL || car != NULL || house != NULL || matter != NULL || charge != NULL || pay != NULL){
        qmd->takeCentralWidget();
    }

    if(log != NULL && !log->isHidden()){
        qmd->takeCentralWidget();
        log->hide();
    }
    if(log != NULL) {
        log->setParent(qmd);
        qmd->setCentralWidget(log);
        log->show();
    }
}

/**小区信息
 * @brief admin_MainWindow::on_pushButton_21_clicked
 */
void admin_MainWindow::on_pushButton_21_clicked()
{
    if(usermage != NULL || backupsql != NULL || room != NULL || userFamily != NULL || log!= NULL || complaint != NULL || car != NULL || house != NULL || matter != NULL || charge != NULL || pay != NULL){
        qmd->takeCentralWidget();
    }

    if(xiaoqu != NULL && !xiaoqu->isHidden()){
        qmd->takeCentralWidget();
        xiaoqu->hide();
    }
    if(xiaoqu != NULL) {
        xiaoqu->setParent(qmd);
        qmd->setCentralWidget(xiaoqu);
        xiaoqu->show();
    }
}

/**用户家属管理
 * @brief admin_MainWindow::on_pushButton_22_clicked
 */
void admin_MainWindow::on_pushButton_22_clicked()
{
    if(usermage != NULL || backupsql != NULL || room != NULL || xiaoqu != NULL || log!= NULL || complaint != NULL || car != NULL || house != NULL || matter != NULL || charge != NULL || pay != NULL){
        qmd->takeCentralWidget();
    }

    if(userFamily != NULL && !userFamily->isHidden()){
        qmd->takeCentralWidget();
        userFamily->hide();
    }
    if(userFamily != NULL) {
        userFamily->setParent(qmd);
        qmd->setCentralWidget(userFamily);
        userFamily->show();
    }
}

void admin_MainWindow::on_pushButton_23_clicked()
{
    //房间管理
    if(usermage != NULL || backupsql != NULL || userFamily != NULL || xiaoqu != NULL || log!= NULL || complaint != NULL || car != NULL || house != NULL || matter != NULL || charge != NULL || pay != NULL){
        qmd->takeCentralWidget();
    }

    if(room != NULL && !room->isHidden()){
        qmd->takeCentralWidget();
        room->hide();
    }
    if(room != NULL) {
        room->setParent(qmd);
        qmd->setCentralWidget(room);
        room->show();
    }
}

void admin_MainWindow::on_pushButton_24_clicked()
{
    //备份数据库
    if(usermage != NULL || room != NULL || userFamily != NULL || xiaoqu != NULL || log!= NULL || complaint != NULL || car != NULL || house != NULL || matter != NULL || charge != NULL || pay != NULL){
        qmd->takeCentralWidget();
    }

    if(backupsql != NULL && !backupsql->isHidden()){
        qmd->takeCentralWidget();
        backupsql->hide();
    }
    if(backupsql != NULL) {
        backupsql->setParent(qmd);
        qmd->setCentralWidget(backupsql);
        backupsql->show();
    }

}

void admin_MainWindow::on_pushButton_clicked()
{
    //修改
    QString name = ui->lineEdit_name->text();
    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    query.exec(tr("select log_play from log where log_name = '%1'").arg(name));
    query.next();
    QString logPlay = query.value(0).toString().trimmed();
    if(logPlay == "管理员"){
        emit sendr_adminUpdateInfo(ui->lineEdit_name->text());
    } else {
        ui->pushButton->setFocusPolicy(Qt::NoFocus);
    }
}


