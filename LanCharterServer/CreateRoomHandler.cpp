#include "CreateRoomHandler.h"

CreateRoomHandler::CreateRoomHandler()
{

}

void CreateRoomHandler::handler(QTcpSocket *client, const Protocol &pack)
{
    QString name = pack["userName"].toString();

    User user(name);

    UserHandler uh;
    bool ret = uh.UserCreateRoom(client,  user);
    Protocol packRet;

    if( !ret )
    {
        qDebug() << "创建房间失败";
        packRet.setType(Protocol::creatliveroomfailed);
        packRet["errText"] = "创建房间失败";
    }else {
        qDebug() << "创建房间成功";
        int roomId = 0;
        int groupPort = 0;
        QString audioIp;
        QString videoIp;
        RoomHelper* instance = RoomHelper::getInstance();
        for( auto it : instance->getRooms() )
        {
            if( it.getRoomName() == name )
            {
                roomId = it.getRoomId();
                groupPort = it.getGroupPort();
                qDebug() << groupPort;
                audioIp = it.getAudioGroupAdress().toString();
                qDebug() << audioIp;
                videoIp = it.getVideoGroupAdress().toString();
                qDebug() << videoIp << "90909";
            }
        }
        packRet.setType(Protocol::creatliveroomsucceed);
        packRet["roomId"] = roomId;
        packRet["roomName"] = name;
        packRet["groupPort"] = groupPort;
        packRet["audioIp"] = audioIp;
        packRet["videoIp"] = videoIp;
        packRet["errText"] = "创建房间成功";
    }
    client->write(packRet.pack());
}
