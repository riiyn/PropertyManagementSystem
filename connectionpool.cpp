#include "connectionpool.h"
#include <QDebug>
#include <QSettings>
#include <QMessageBox>

QMutex ConnectionPool::mutex;
QWaitCondition ConnectionPool::waitConnection;
ConnectionPool* ConnectionPool::instance = NULL;

ConnectionPool::ConnectionPool()
{
    QSettings *configIniRead = new QSettings(":/config/databaseConfig.ini",QSettings::IniFormat);
    //将读取到的ini文件保存在QString中，先取值，然后通过toString()函数转换成QString类型
    QString hostname = configIniRead->value("/databaseConfig/hostName").toString();
    QString iPort = configIniRead->value("/databaseConfig/iPort").toString();
    QString databasetype = configIniRead->value("/databaseConfig/databaseType").toString();
    QString databasename = configIniRead->value("/databaseConfig/databaseName").toString();
    QString user_name = configIniRead->value("/databaseConfig/username").toString();
    QString pwd = configIniRead->value("/databaseConfig/password").toString();
    delete configIniRead;

    hostName = hostname;
    port = iPort;
    databaseName = databasename;
    username = user_name;
    password = pwd;
    databaseType = databasetype;
    testOnBorrow = true;
    testOnBorrowSql = "select 1";

    maxWaitTime = 1000;
    waitInterval = 200;
    maxConnectionCount = 50;
}

ConnectionPool::~ConnectionPool(){
    //销毁连接池的时候删除所有的连接
    foreach (QString connectionName, usedConnectionNames) {
        QSqlDatabase::removeDatabase(connectionName);
    }

    foreach (QString connectionName, unusedConnectionNames) {
        QSqlDatabase::removeDatabase(connectionName);
    }
}

ConnectionPool& ConnectionPool::getInstance(){
    if(NULL == instance){
        QMutexLocker locker(&mutex);

        if(NULL == instance){
            instance = new ConnectionPool();
        }
    }
    return *instance;
}

void ConnectionPool::release(){
    QMutexLocker locker(&mutex);
    delete instance;
    instance = NULL;
}

/**
 * @brief ConnectionPool::openConnection
 * @return
 *
 * openConnection() 函数相对比较复杂，也是 ConnectionPool 的核心
 * 1.如果没有可复用连接 pool.unusedConnectionNames.size() == 0 且已经创建的连接数达到最大，则等待，等待期间有连接被释放回连接池就复用这个连接，如果超时都没有可用连接，则返回一个无效的连接 QSqlDatabase()。
 * 2.如果没有可复用连接，但是已经创建的连接数没有达到最大，那么就创建一个新的连接，并把这个连接的名字添加到 usedConnectionNames。
 * 3.如果有可复用的连接，则复用它，把它的名字从 unusedConnectionNames 里删除并且添加到 usedConnectionNames。
 */
QSqlDatabase ConnectionPool::openConnection(){
    ConnectionPool& pool = ConnectionPool::getInstance();
    QString connectionName;

    QMutexLocker locker(&mutex);

    //已创建连接数
    int connectionCount = pool.unusedConnectionNames.size() + pool.usedConnectionNames.size();

    //如果连接已用完，等待waitInterval 毫秒看看是否有可用连接，最长等待 maxWaitTime 毫秒
    for(int i = 0; i < pool.maxWaitTime && pool.unusedConnectionNames.size() == 0 && connectionCount == pool.maxConnectionCount;
        i += pool.waitInterval){
        waitConnection.wait(&mutex, pool.waitInterval);

        //从新计算已创建连接数
        connectionCount = pool.unusedConnectionNames.size() + pool.usedConnectionNames.size();
    }

    if(pool.unusedConnectionNames.size() > 0){
        //大于0说明有已经回收的连接，复用它们
        connectionName = pool.unusedConnectionNames.dequeue();
    } else if (connectionCount < pool.maxConnectionCount) {
        //没有已经回收的连接，但是没有达到最大连接数，则创建新的连接
        connectionName = QString("Connection-%1").arg(connectionCount + 1);
    } else {
        //已经达到最大连接数
        qDebug() << "Cannot create more connections";
        return QSqlDatabase();
    }

    //创建连接
    QSqlDatabase db = pool.createConnection(connectionName);

    //有效的连接才放入usedConnectionNames
    if(db.isOpen()){
        pool.usedConnectionNames.enqueue(connectionName);

    }
    return db;

}

void ConnectionPool::closeConnection(QSqlDatabase connection){
    ConnectionPool& pool = ConnectionPool::getInstance();
    QString connectionName = connection.connectionName();

    //如果是我们创建的连接，从used里删除，防暑unused里
    if(pool.usedConnectionNames.contains(connectionName)){
        QMutexLocker locker(&mutex);
        pool.usedConnectionNames.removeOne(connectionName);
        pool.unusedConnectionNames.enqueue(connectionName);
        waitConnection.wakeOne();
    }
}

QSqlDatabase ConnectionPool::createConnection(const QString &connectionName){
    //连接已经创建了，复用它，而不是重新创建
    if(QSqlDatabase::contains(connectionName)){
        QSqlDatabase db1 = QSqlDatabase::database(connectionName);

        if(testOnBorrow){
            //返回连接前访问数据库，如果连接断开，重新建立连接
            qDebug() << "Test connection on borrow, execute:" << testOnBorrowSql << ", for" << connectionName;
            QSqlQuery query(testOnBorrowSql,db1);

            if(query.lastError().type() != QSqlError::NoError && !db1.open()){
                qDebug() << "Open datatabase error:" << db1.lastError().text();
                QMessageBox::warning(0,"错误！",
                                     "服务器错误：数据库连接失败！");
                return QSqlDatabase();
            }
        }
        return db1;
    }

    //创建一个新的连接
    QSqlDatabase db = QSqlDatabase::addDatabase(databaseType,connectionName);
    db.setHostName(hostName);
    db.setDatabaseName(databaseName);
    db.setUserName(username);
    db.setPassword(password);

    if(!db.open()){
        qDebug() << "Open datatabase error:" << db.lastError().text();
        QMessageBox::warning(0,"错误！",
                             "服务器错误：数据库连接失败！");
        return QSqlDatabase();
    }

    return db;
}













