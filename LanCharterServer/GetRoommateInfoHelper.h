#ifndef GETROOMMATEINFOHELPER_H
#define GETROOMMATEINFOHELPER_H
#include "IHandler.h"
#include "Room.h"
#include "RoomHelper.h"
#include "UserHandler.h"
#include <QDebug>


class GetRoommateInfoHelper: public IHandler
{
public:
    GetRoommateInfoHelper();
    void handler(QTcpSocket* client, const Protocol& pack);
};

#endif // GETROOMMATEINFOHELPER_H
