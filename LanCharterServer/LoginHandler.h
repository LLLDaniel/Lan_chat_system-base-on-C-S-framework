#ifndef LOGINHANDLER_H
#define LOGINHANDLER_H
#include "IHandler.h"

class LoginHandler: public IHandler
{
public:
    LoginHandler();
    void handler(QTcpSocket* client, const Protocol& pack);
};

#endif // LOGINHANDLER_H
