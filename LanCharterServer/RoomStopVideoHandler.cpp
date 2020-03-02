#include "RoomStopVideoHandler.h"

RoomStopVideoHandler::RoomStopVideoHandler()
{

}

void RoomStopVideoHandler::handler(QTcpSocket *client, const Protocol &pack)
{
    int roomId = pack["roomId"].toInt();
    QString roomName = pack["roomName"].toString();
    qDebug() << "get the room" << roomId << " " << roomName;

    QVector<Room>& rooms = RoomHelper::getInstance()->getRooms();

    for (int i =0 ; i< rooms.size(); i++) {
        if( rooms[i].getRoomId() == roomId )
        {
            QVector<Acount_t>& acounts = rooms[i].getRoomAcount_t();
            Protocol packRet;
            packRet.setType(Protocol::videosharestop);
            packRet["errText"] = "主播停止播送视频";
            qDebug() << "helo";
            for (int j = 0; j < acounts.size() ;j++) {
                acounts[j].socket->write(packRet.pack());
            }
        }
    }
}
