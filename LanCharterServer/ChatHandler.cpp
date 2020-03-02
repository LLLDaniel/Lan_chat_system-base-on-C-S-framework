#include "ChatHandler.h"

ChatHandler::ChatHandler()
{

}

void ChatHandler::handler(QTcpSocket *client, const Protocol &pack)
{

    QString text = pack["chatText"].toString();
    QString userName = pack["userName"].toString();

    int roomId = pack["roomId"].toInt();
    qDebug() << text << " +++ <"<< roomId << " ___" << userName;

    QVector<Room>& rooms = RoomHelper::getInstance()->getRooms();

    for (int i =0 ; i< rooms.size(); i++) {
        if( rooms[i].getRoomId() == roomId )
        {
            QVector<Acount_t>& acounts = rooms[i].getRoomAcount_t();
            Protocol packRet;
            packRet.setType(Protocol::chat);
            packRet["chatText"] = text;
            packRet["userName"] = userName;
            qDebug() << "helo";
            for (int j = 0; j < acounts.size() ;j++) {
                acounts[j].socket->write(packRet.pack());
                if( acounts[j].name == userName )
                {
                    acounts[j].heatNum++;
                    qDebug()<< acounts[j].heatNum;
                }
            }
            //给主播发送信息
            QTcpSocket* roomMasterSocket = rooms[i].getSocketRoom();
            qDebug() << "send to roomMaster";
            roomMasterSocket->write(packRet.pack());
        }
    }
}
