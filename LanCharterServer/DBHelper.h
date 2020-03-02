#ifndef DBHELPER_H
#define DBHELPER_H
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QMutex>


class DBHelper
{
public:
    static DBHelper* getInstance();
    bool connectSqlBase();
    void closeSqlBase();

private:
    DBHelper();
    DBHelper(const DBHelper&) = default;
    DBHelper& operator=(const DBHelper&) = default;

    static QMutex mutex;
    static DBHelper* instance;
    QSqlDatabase db;

};

#endif // DBHELPER_H
