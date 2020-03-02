#ifndef GETROOMINFOHELPER_H
#define GETROOMINFOHELPER_H
#include "IHandler.h"
#include "Room.h"
#include "DBHelper.h"
#include "UserHandler.h"
#include <QVector>



class GetRoomInfoHelper: public IHandler
{
public:
    GetRoomInfoHelper();
    void handler(QTcpSocket* client, const Protocol& pack);
};

#endif // GETROOMINFOHELPER_H
