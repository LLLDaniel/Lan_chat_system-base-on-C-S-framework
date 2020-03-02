#ifndef MODIFYPASSWORDHANDLER_H
#define MODIFYPASSWORDHANDLER_H
#include "IHandler.h"
#include "Room.h"
#include "RoomHelper.h"
#include "UserHandler.h"
#include <QDebug>


class ModifyPasswordHandler:public IHandler
{
public:
    ModifyPasswordHandler();
    void handler(QTcpSocket* client, const Protocol& pack);
};

#endif // MODIFYPASSWORDHANDLER_H
