#include "DestroyRoomHandler.h"

DestroyRoomHandler::DestroyRoomHandler()
{

}

void DestroyRoomHandler::handler(QTcpSocket *client, const Protocol &pack)
{
    int roomId = pack["roomId"].toInt();
    QString roomName = pack["roomName"].toString();
    qDebug() << " +begin to delte room " << roomId << " " << roomName;
    RoomHelper* instance = RoomHelper::getInstance();
    //在销毁之前通知该房间的所有用户
    noticeRoomUserDelete(roomId);
    bool ret = instance->deleteRoom(roomId, roomName);
    Protocol packRet;

    if( !ret )
    {
        qDebug() << "销毁房间失败";
        packRet.setType(Protocol::destroyliveroomfailed);
        packRet["errText"] = "销毁房间失败";
    }else {
        qDebug() << "销毁房间成功";
        packRet.setType(Protocol::destroyliveroomsucceed);
        packRet["errText"] = "销毁房间成功";
    }
    client->write(packRet.pack());
    qDebug() << "(*(*(";
}


void DestroyRoomHandler::noticeRoomUserDelete(int roomId)
{
    QVector<Room>& rooms = RoomHelper::getInstance()->getRooms();

    for (int i =0 ; i< rooms.size(); i++) {
        if( rooms[i].getRoomId() == roomId )
        {
            QVector<Acount_t>& acounts = rooms[i].getRoomAcount_t();
            Protocol packRet;
            packRet.setType(Protocol::roomoverdue);
            packRet["errText"] = "主播已退出房间，直播间过期，请及时退出.......";
            qDebug() << "notice every roomoverdue";
            for (int j = 0; j < acounts.size() ;j++) {
                acounts[j].socket->write(packRet.pack());

            }
        }
    }
}
