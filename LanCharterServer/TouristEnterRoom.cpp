#include "TouristEnterRoom.h"

TouristEnterRoom::TouristEnterRoom()
{

}

void TouristEnterRoom::handler(QTcpSocket *client, const Protocol &pack)
{
    int roomId = pack["roomId"].toInt();
    QString roomName = pack["roomName"].toString();
    qDebug() << roomId << " _+_+_+_" << roomName;
    QString userName = pack["userName"].toString();

    Room room;
    room.setRoomId(roomId);
    room.setRoomName(roomName);

    Acount_t user_t{userName, client, 0};

    //找到房间， 把acount_t 加进去
    UserHandler uh;
    groupInfo_t groupPackRet = uh.UserEnterRoom(room, user_t);

    Protocol packRet;
    if( !groupPackRet.ret )
    {
        qDebug() << "enter room failed";
        packRet.setType(Protocol::enterliveroomfailed);  
        packRet["errText"] = "进入房间失败,房间已失效";
    }else{
        qDebug() << "enter room succeed";
        packRet.setType(Protocol::enterliveroomsucceed);
        //将房间的组播信息发送给客户端
        packRet["roomId"] = roomId;
        packRet["roomName"] = roomName;
        packRet["groupPort"] = groupPackRet.groupPort;
        packRet["audioIp"] = groupPackRet.audioGroupAdress.toString();
        qDebug() << groupPackRet.audioGroupAdress.toString();
        packRet["videoIp"] = groupPackRet.videoGroupAdress.toString();
        qDebug() << groupPackRet.videoGroupAdress.toString();
        packRet["errText"] = "进入房间成功";
        //将进入的消息发送到房间的在线用户
        sendEnterMessageToRoomMate(roomId, roomName, userName);
    }
    client->write(packRet.pack());
}

//将进入的消息发送到房间的在线用户
void TouristEnterRoom::sendEnterMessageToRoomMate(int roomId, QString& roomName, QString& userName)
{
    RoomHelper* instance = RoomHelper::getInstance();
    QVector<Room>& rooms = instance->getRooms();
    Protocol packRet;
    packRet.setType(Protocol::anyoneenterliveroom);
    packRet["userName"] = userName;
    for (int i = 0; i < rooms.size() ; i++) {
        if( rooms[i].getRoomId() == roomId
                && rooms[i].getRoomName() == roomName )
        {
            rooms[i].getSocketRoom()->write(packRet.pack());
            QVector<Acount_t>& acounts = rooms[i].getRoomAcount_t();
            qDebug() << "send the anyone enter to mate";
            for (int j =0 ;j < acounts.size() ; j++) {
                acounts[j].socket->write(packRet.pack());
            }
        }
    }
}
