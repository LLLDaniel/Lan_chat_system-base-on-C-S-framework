#ifndef ROOM_H
#define ROOM_H
#include <QString>
#include <QVector>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QVariant>

typedef struct Accout{
    QString name;   //姓名
    QTcpSocket* socket;     //套接字
    int heatNum;
}Acount_t;

typedef struct groupInfo{
    bool ret;
    int groupPort;
    QHostAddress audioGroupAdress;
    QHostAddress videoGroupAdress;
}groupInfo_t;

class Room
{
public:
    Room();
    Room(const QString& roomName, QTcpSocket* socketRoom);
    Room(const Room& m_room);

private:
    static int roomNumber;
    static int groupPortNumber;   //广播组端口

    int roomId; //房间号
    QString roomName; //房主名
    QTcpSocket* socketRoom; //房主的套接字
    QHostAddress audioGroupAdress;  //音频广播组地址
    QHostAddress videoGroupAdress;  //视频广播组地址
    int groupPort;   //广播组端口
    QVector<Acount_t> roomAcounts;  //存放房间所有人的信息

public:
    void setRoomId( int roomid ) { this->roomId = roomid; }
    int getRoomId() const { return this->roomId; }

    void setRoomName(const QString& roomName) { this->roomName = roomName; }
    const QString& getRoomName() const { return this->roomName; }

    void setSocketRoom(QTcpSocket* socketRoom) { this->socketRoom = socketRoom; }
    QTcpSocket* getSocketRoom() { return this->socketRoom; }

    void setAudioGroupAdress( const QHostAddress audioGroupAdress) { this->audioGroupAdress = audioGroupAdress; }
    const QHostAddress& getAudioGroupAdress() { return this->audioGroupAdress; }

    void setVideoGroupAdress( const QHostAddress videoGroupAdress) { this->videoGroupAdress = videoGroupAdress; }
    const QHostAddress& getVideoGroupAdress() { return this->videoGroupAdress; }

    void setGroupPort( int groupPort ) { this->groupPort = groupPort; }
    int getGroupPort() { return this->groupPort; }

    void addAccouts( const Acount_t acount ) { this->roomAcounts.append(acount); }
    void deleteAccouts( const Acount_t acount );

    QVector<Acount_t>& getRoomAcount_t() { return this->roomAcounts; }

    static int getRoomNumber();
    static int getGroupPortNumber();

};

#endif // ROOM_H
