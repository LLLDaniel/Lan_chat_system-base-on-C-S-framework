#ifndef GIVEGIFTHANDLER_H
#define GIVEGIFTHANDLER_H
#include "IHandler.h"
#include "Room.h"
#include "RoomHelper.h"
#include "UserHandler.h"
#include <QDebug>
#include "protocol.h"

class GiveGiftHandler:public IHandler
{
public:
    GiveGiftHandler();
    void handler(QTcpSocket* client, const Protocol& pack);
    void sendGiftMessageToRoomMate(const Room&room, const User& user, const QString& giftName);
};

#endif // GIVEGIFTHANDLER_H
