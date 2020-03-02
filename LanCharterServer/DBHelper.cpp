#include "DBHelper.h"
#include <QDebug>

DBHelper* DBHelper::instance = nullptr;
QMutex DBHelper::mutex;


DBHelper::DBHelper()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("mysql");

}

DBHelper* DBHelper::getInstance()
{
    if( instance == nullptr)
    {
        mutex.lock();
        if(instance == nullptr)
        {
            instance = new DBHelper();
        }
        mutex.unlock();
    }
    return instance;
}

bool DBHelper::connectSqlBase()
{
    db.setDatabaseName("first");
    if(!db.open())
    {
        qDebug() << db.lastError().text();
        return false;
    }
    return true;
}

void DBHelper::closeSqlBase()
{
    db.close();
}
