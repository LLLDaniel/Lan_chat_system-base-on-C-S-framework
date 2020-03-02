#include "GetRoommateInfoHelper.h"

GetRoommateInfoHelper::GetRoommateInfoHelper()
{

}

void GetRoommateInfoHelper::handler(QTcpSocket *client, const Protocol &pack)
{
    int roomId = pack["roomId"].toInt();
    QString roomName = pack["roomName"].toString();
    qDebug() << "get the room" << roomId << " " << roomName;
    Room room;
    room.setRoomId(roomId);
    room.setRoomName(roomName);

    UserHandler uh;
    QVector<Acount_t> acounts = uh.UserGetRoommateInfo(room);
    qDebug() << "======" << acounts.size();
    Protocol packRet(Protocol::getRoomMateInfos);
    packRet["roomAcountNumber"] = acounts.size();
    for (QVector<Acount_t>::iterator it = acounts.begin();
         it != acounts.end(); it++)
    {
         qDebug() << "发送房间用户列表------------" << it->name;
         packRet["userName"] = it->name;
         packRet["userHeatNum"] = it->heatNum;
         client->write(packRet.pack());
    }
}
