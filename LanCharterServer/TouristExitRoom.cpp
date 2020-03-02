#include "TouristExitRoom.h"

TouristExitRoom::TouristExitRoom()
{

}

void TouristExitRoom::handler(QTcpSocket *client, const Protocol &pack)
{
    int roomId = pack["roomId"].toInt();
    QString roomName = pack["roomName"].toString();
    qDebug() << roomId << " _+_+_+_" << roomName;
    QString userName = pack["userName"].toString();

    Room room;
    room.setRoomId(roomId);
    room.setRoomName(roomName);

    Acount_t user_t{userName, client};

    //找到房间， 把acount_t 删除
    UserHandler uh;
    bool ret = uh.UserExitRoom(room, user_t);

    Protocol packRet;
    if( !ret )
    {
        qDebug() << "exit room failed";
        packRet.setType(Protocol::exitliveroomfailed);
        packRet["errText"] = "离开房间失败";

    }else{
        qDebug() << "exit room succeed";
        packRet.setType(Protocol::exitliveroomsucceed);
        packRet["errText"] = "离开房间成功";
        //将离开的消息发送到房间的在线用户
        sendExitMessageToRoomMate(roomId, roomName, userName);
    }
    client->write(packRet.pack());
}

//将离开的消息发送到房间的在线用户
void TouristExitRoom::sendExitMessageToRoomMate(int roomId, QString& roomName, QString& userName)
{
    RoomHelper* instance = RoomHelper::getInstance();
    QVector<Room>& rooms = instance->getRooms();
    Protocol packRet;
    packRet.setType(Protocol::anyoneexitliveroom);
    packRet["userName"] = userName;
    for (int i = 0; i < rooms.size() ; i++) {
        if( rooms[i].getRoomId() == roomId
                && rooms[i].getRoomName() == roomName )
        {
            rooms[i].getSocketRoom()->write(packRet.pack());
            QVector<Acount_t>& acounts = rooms[i].getRoomAcount_t();
            qDebug() << "send the anyone live to mate";
            for (int j =0 ;j < acounts.size() ; j++) {
                acounts[j].socket->write(packRet.pack());
            }
        }
    }
}
