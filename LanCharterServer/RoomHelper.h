#ifndef ROOMHELPER_H
#define ROOMHELPER_H
#include "Room.h"
#include <QMutex>
#include <QVector>
#include <vector>

class RoomHelper
{
public:
    static RoomHelper* getInstance();
    QVector<Room>& getRooms();
    bool addRoom(Room& room);

    bool deleteRoom(int roomId, const QString& roomName);


private:
    RoomHelper();
    QVector<Room> rooms;

    static RoomHelper* instance;
    static QMutex mutex;

};

#endif // ROOMHELPER_H
