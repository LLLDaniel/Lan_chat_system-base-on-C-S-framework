#ifndef TOURISTEXITROOM_H
#define TOURISTEXITROOM_H
#include "IHandler.h"
#include "Room.h"
#include "User.h"
#include "UserHandler.h"


class TouristExitRoom: public IHandler
{
public:
    TouristExitRoom();
    void handler(QTcpSocket* client, const Protocol& pack);
    void sendExitMessageToRoomMate(int roomId, QString& roomName, QString& userName);
};

#endif // TOURISTEXITROOM_H
