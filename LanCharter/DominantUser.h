#ifndef DOMINANTUSER_H
#define DOMINANTUSER_H
#include <QMutex>
#include "User.h"
#include <QTcpSocket>
#include <QHostAddress>

class DominantUser
{
public:
    static DominantUser* getInstance();

    void setUser(const User& user){ this->user = user; }
    User& getUser() { return this->user; }

    void setTcpSocket(QTcpSocket* userTcpSocket){ this->client_socket = userTcpSocket; }
    QTcpSocket* getTcpSocket() { return this->client_socket; }

    void setBeRoomMaster(int flag = false ) { this->beRoomMaster = flag; }
    bool getBeRoomMaster() { return this->beRoomMaster; }

    void setRoomId(int roomId ) { this->roomId = roomId; }
    int getRoomId() { return this->roomId; }

    void setRoomName(const QString& roomName ) { this->roomName = roomName; }
    const QString& getRoomName() { return this->roomName; }

    void setGroupPort(int groupPort ) { this->groupPort = groupPort; }
    int getGroupPort() { return this->groupPort; }

    void setAudioGroupAddress( const QString& audioIp ) { this->audioGroupAddress = QHostAddress(audioIp); }
    const QHostAddress& getAudioGroupAddress() { return this->audioGroupAddress; }

    void setVideoGroupAddress( const QString& videoIp ) { this->videoGroupAddress = QHostAddress(videoIp); }
    const QHostAddress& getVideoGroupAddress() { return this->videoGroupAddress; }

    void setDanmu(bool danmu) { this->danmu = danmu; }
    bool getDanmu() {return this->danmu; }

private:
    DominantUser();
    static QMutex mutex;
    static DominantUser* instance;
    User  user;
    QTcpSocket* client_socket;
    //是否成为主播，也就是开房间
    bool beRoomMaster;

    //房间的信息
    QString roomName;
    int roomId;
    //房间组播信息
    int groupPort;
    QHostAddress audioGroupAddress;
    QHostAddress videoGroupAddress;

    //弹幕状态
    bool danmu;

};

#endif // DOMINANTUSER_H
