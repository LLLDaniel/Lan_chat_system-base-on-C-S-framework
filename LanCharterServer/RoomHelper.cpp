#include "RoomHelper.h"

RoomHelper* RoomHelper::instance = nullptr;
QMutex RoomHelper::mutex;

RoomHelper::RoomHelper()
{
    qDebug() <<" room helper create";

}

RoomHelper* RoomHelper::getInstance()
{
    if( instance == nullptr )
    {
        mutex.lock();
        if( instance == nullptr )
        {

            instance = new RoomHelper();
        }
        mutex.unlock();
    }
    return instance;
}

QVector<Room>& RoomHelper::getRooms()
{
    return  this->rooms;
}

bool RoomHelper::addRoom(Room& room)   //这个地方不能加const,加了之后添加不进去
{
    for(int i =0;i<rooms.size(); i++)
    {
        if( rooms[i].getRoomName() == room.getRoomName() )
        {
            return false;
        }
    }
    room.setRoomId(Room::getRoomNumber()); //设置房间id
    room.setGroupPort(Room::getGroupPortNumber()); //设置房间组播的端口
    room.setAudioGroupAdress(QHostAddress("224.1.1.2")); //设置房间音频组播的地址
    room.setVideoGroupAdress(QHostAddress("224.1.1.1")); //设置房间视频组播的地址

    rooms.push_back(room);
    return true;
}

bool RoomHelper::deleteRoom(int roomId, const QString &roomName)
{
    int i = 0;
    qDebug() << "begin delete room " << roomId << "  " << roomName;
    qDebug() << "room's size before " << rooms.size();
    for( QVector<Room>:: iterator it=rooms.begin(); it!=rooms.end(); it++ )
    {
        if( it->getRoomId() == roomId && it->getRoomName() == roomName )
        {
            rooms.remove(i);
            qDebug() << " delete room succeed ";
            qDebug() << "room's size after " << rooms.size();
            return true;
        }
        i++;
    }
    return false;
}
