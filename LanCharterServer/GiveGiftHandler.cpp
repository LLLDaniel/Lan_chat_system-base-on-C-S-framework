#include "GiveGiftHandler.h"

GiveGiftHandler::GiveGiftHandler()
{

}

void GiveGiftHandler::handler(QTcpSocket *client, const Protocol &pack)
{
    int roomId = pack["roomId"].toInt();
    QString roomName = pack["roomName"].toString();
    QString userName = pack["userName"].toString();

    int giftType = pack["giftType"].toInt();
    qDebug() << "gift number is " << giftType;

    Room room;
    room.setRoomId(roomId);
    room.setRoomName(roomName);

    User user(userName);
    user.setMoney(-1);

    int giftMoney = 0;
    QString giftName;
    switch (giftType) {
    case Protocol::giftType::flower:
        giftMoney = 1;
        giftName = "flower";
        break;
    case Protocol::giftType::diamond:
        giftMoney = 10;
        giftName = "diamond";
        break;
    case Protocol::giftType::runcar:
        giftMoney = 100;
        giftName = "runcar";
        break;
    default:
        break;
    }
    UserHandler uh;
    bool ret = uh.giveGift(room, user, giftMoney, giftName);

    Protocol packRet(Protocol::getRoomMateInfos);

    if( !ret )
    {
        qDebug() << "发送礼物失败";
        packRet.setType(Protocol::givinggiftfailed);
        if( (int)user.getMoney() != -1 && user.getMoney() < giftMoney )
        {
            packRet["errText"] = "账户余额不足，请充值！";
        }else{
            packRet["errText"] = "发送礼物失败";
        }

    }else {
        qDebug() << "发送礼物成功";
        packRet.setType(Protocol::givinggiftsucceed);
        packRet["errText"] = "发送礼物成功";
        //通知房间成员
        sendGiftMessageToRoomMate(room, user, giftName);

    }
    client->write(packRet.pack());
    qDebug() << "发送礼物完毕";

}

void GiveGiftHandler::sendGiftMessageToRoomMate(const Room& room, const User& user, const QString& giftName)
{
    QVector<Room>& rooms = RoomHelper::getInstance()->getRooms();

    for (int i =0 ; i< rooms.size(); i++) {
        if( rooms[i].getRoomId() == room.getRoomId() )
        {
            QVector<Acount_t>& acounts = rooms[i].getRoomAcount_t();
            Protocol packRet;
            packRet.setType(Protocol::anyonegivegifttoanchor);
            QString text = user.getName() + " 送了 " + giftName + " 给主播~~~~~~~~~,鼓掌！！！！";
            packRet["errText"] = text;
            qDebug() << "helo";
            for (int j = 0; j < acounts.size() ;j++) {
                acounts[j].socket->write(packRet.pack());
            }
            //给主播发送信息
            QTcpSocket* roomMasterSocket = rooms[i].getSocketRoom();
            qDebug() << "send to roomMaster";
            roomMasterSocket->write(packRet.pack());
        }
    }
}
