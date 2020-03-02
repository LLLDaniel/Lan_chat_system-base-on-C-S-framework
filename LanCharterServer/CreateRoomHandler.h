#ifndef CREATEROOMHANDLER_H
#define CREATEROOMHANDLER_H
#include "IHandler.h"
#include "UserHandler.h"

class CreateRoomHandler: public IHandler
{
public:
    CreateRoomHandler();
    void handler(QTcpSocket* client, const Protocol& pack);
};

#endif // CREATEROOMHANDLER_H
