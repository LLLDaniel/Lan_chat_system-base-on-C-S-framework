#ifndef ROOMSTOPVIDEOHANDLER_H
#define ROOMSTOPVIDEOHANDLER_H
#include "IHandler.h"
#include "Room.h"
#include "RoomHelper.h"
#include "QtDebug"

class RoomStopVideoHandler:public IHandler
{
public:
    RoomStopVideoHandler();
    void handler(QTcpSocket* client, const Protocol& pack);
};

#endif // ROOMSTOPVIDEOHANDLER_H
