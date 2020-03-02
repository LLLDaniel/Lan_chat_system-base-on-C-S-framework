#include "GetRoomInfoHelper.h"


GetRoomInfoHelper::GetRoomInfoHelper()
{

}

void GetRoomInfoHelper::handler(QTcpSocket *client, const Protocol &pack)
{
    UserHandler uh;
    QVector<Room> rooms = uh.UserGetRoomInfo();
    Protocol packRet(Protocol::getRoomInfos);
    for (QVector<Room>::iterator it = rooms.begin(); it != rooms.end(); it++) {
         qDebug() << "发送房间列表------------";
         packRet["roomId"] = it->getRoomId();
         qDebug() << "房间号" << it->getRoomId();
         packRet["roomName"] = it->getRoomName();
         qDebug() << "房主" << it->getRoomName();
         packRet["roomAcount"] = it->getRoomAcount_t().size();
         client->write(packRet.pack());
    }
}
