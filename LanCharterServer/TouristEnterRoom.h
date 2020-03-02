#ifndef TOURISTENTERROOM_H
#define TOURISTENTERROOM_H
#include "IHandler.h"
#include "Room.h"
#include "User.h"
#include "UserHandler.h"

class TouristEnterRoom: public IHandler
{
public:
    TouristEnterRoom();
    void handler(QTcpSocket* client, const Protocol& pack);
    void sendEnterMessageToRoomMate(int roomId, QString& roomName, QString& userName);

};

#endif // TOURISTENTERROOM_H
