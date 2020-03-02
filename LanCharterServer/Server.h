#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "SocketHandler.h"
#include "NetWorkHelper.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    ~Server();

signals:

public slots:
    void newConnectSlot();
    void clientSocketDisconnectedSlot();
    void MSGError(QAbstractSocket::SocketError);

private:
    QTcpServer* lanServer;
    void builtServer();

};

#endif // SERVER_H
