#ifndef CHATHANDLER_H
#define CHATHANDLER_H
#include "IHandler.h"
#include "Room.h"
#include "RoomHelper.h"
#include "QtDebug"


class ChatHandler:public IHandler
{
public:
    ChatHandler();
    void handler(QTcpSocket* client, const Protocol& pack);
};

#endif // CHATHANDLER_H
