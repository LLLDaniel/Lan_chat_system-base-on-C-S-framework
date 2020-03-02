#ifndef NETWORKHELPER_H
#define NETWORKHELPER_H
#include <QTcpServer>
#include <QTcpSocket>
#include "SocketHandler.h"
#include <QVector>
#include <QMutex>
#include <QDebug>

class NetWorkHelper
{
public:

    QVector<SocketHandler*> getClients() const { return this->clients; }
    static NetWorkHelper* getInstance();

    void addClient(SocketHandler* client);

private:
    NetWorkHelper();
private:
    static NetWorkHelper* instance;
    static QMutex mutex;
    QVector<SocketHandler*> clients;
};

#endif // NETWORKHELPER_H
