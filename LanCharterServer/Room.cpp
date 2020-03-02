#include "Room.h"

int Room::roomNumber = 1000;
int Room::groupPortNumber = 8888;   //广播组端口

Room::Room()
{
    qDebug() << roomId << "----";
}


Room::Room(const QString& roomName, QTcpSocket* socketRoom)
    :roomName(roomName), socketRoom(socketRoom)
{
//    this->roomId = Room::roomNumber++;
//    qDebug() << roomId << "++++";
}

Room::Room(const Room &m_room)
{
    this->roomId = m_room.roomId;
    this->roomName = m_room.roomName;
    this->socketRoom = m_room.socketRoom;
    this->groupPort = m_room.groupPort;   //广播组端口
    this->audioGroupAdress = m_room.audioGroupAdress;
    this->videoGroupAdress = m_room.videoGroupAdress;
    this->roomAcounts = m_room.roomAcounts;  //存放房间所有人的信息
}

void Room::deleteAccouts(const Acount_t acount)
{
    int i =0;
    for (i = 0; i < roomAcounts.size(); i++) {
        if( roomAcounts[i].name == acount.name )
        {
            break;
        }
    }
    roomAcounts.remove(i);
}

int Room::getRoomNumber()
{
    int roomId = roomNumber++;
    return roomId;
}

int Room::getGroupPortNumber()
{
    int groupPort = groupPortNumber++;
    return groupPort;
}


