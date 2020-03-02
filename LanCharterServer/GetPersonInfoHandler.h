#ifndef GETPERSONINFOHANDLER_H
#define GETPERSONINFOHANDLER_H
#include "IHandler.h"
#include "Room.h"
#include "DBHelper.h"
#include "UserHandler.h"
#include <QVector>

class GetPersonInfoHandler: public IHandler
{
public:
    GetPersonInfoHandler();
    void handler(QTcpSocket* client, const Protocol& pack);
};

#endif // GETPERSONINFOHANDLER_H
