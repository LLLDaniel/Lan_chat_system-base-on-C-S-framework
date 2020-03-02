#ifndef LOGOUTHANDLER_H
#define LOGOUTHANDLER_H
#include "IHandler.h"

class LogoutHandler:public IHandler
{
public:
    LogoutHandler();
    void handler(QTcpSocket* client, const Protocol& pack);
};

#endif // LOGOUTHANDLER_H
