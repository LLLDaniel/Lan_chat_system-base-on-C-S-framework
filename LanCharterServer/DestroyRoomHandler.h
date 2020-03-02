#ifndef DESTROYROOMHANDLER_H
#define DESTROYROOMHANDLER_H
#include "IHandler.h"
#include "UserHandler.h"
#include "RoomHelper.h"
#include <QDebug>


class DestroyRoomHandler: public IHandler
{
public:
    DestroyRoomHandler();
    void handler(QTcpSocket* client, const Protocol& pack);
    void noticeRoomUserDelete(int roomId);
};

#endif // DESTROYROOMHANDLER_H
