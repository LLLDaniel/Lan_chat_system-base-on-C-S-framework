#ifndef IHANDLER_H
#define IHANDLER_H
#include "protocol.h"
#include <QTcpServer>
#include <QTcpSocket>

class IHandler
{
public:
    IHandler();
    virtual void handler(QTcpSocket* client, const Protocol& pack) = 0;
};

#endif // IHANDLER_H
