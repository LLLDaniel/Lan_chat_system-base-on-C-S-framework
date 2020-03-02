#ifndef REGISTHANDLER_H
#define REGISTHANDLER_H
#include "IHandler.h"
#include <QDebug>

#include "DBHelper.h"
#include "User.h"
#include "UserHandler.h"


class RegistHandler:public IHandler
{
public:
    RegistHandler();
    void handler(QTcpSocket* client, const Protocol& pack);
};

#endif // REGISTHANDLER_H
