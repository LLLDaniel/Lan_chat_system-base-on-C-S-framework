#include "Server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    qDebug() << "hello world";
    this->lanServer = new QTcpServer(this);

    builtServer();
    connect(this->lanServer, SIGNAL(newConnection()), this, SLOT(newConnectSlot()));

}

Server::~Server()
{
    if( this->lanServer != nullptr)
    {
        delete lanServer;
    }

}

void Server::newConnectSlot()
{
    QTcpSocket* clientSocket = this->lanServer->nextPendingConnection();

    //客户端连接错误之后的处理
    connect(clientSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                            this, SLOT(MSGError(QAbstractSocket::SocketError)));//建立槽函数，获取错误信息

    //客户端断开链接之后处理函数
    connect(clientSocket, SIGNAL(disconnected()), this, SLOT(clientSocketDisconnectedSlot()));

    qDebug() << clientSocket->peerAddress().toString() << " "
             << clientSocket->peerPort() << " " << "connect to server";

    SocketHandler* sockhandler = new SocketHandler(clientSocket,this);
    NetWorkHelper* nh = NetWorkHelper::getInstance();
    nh->addClient(sockhandler);
}

void Server::builtServer()
{
    if( this->lanServer->isListening() )
    {
        qDebug() << " already listening";
        return;
    }
    else {
        if( this->lanServer->listen(QHostAddress("127.0.0.1"), 8888) )
        {
            qDebug() << " listening succeed";
        }else {
            qDebug() << " listening failed";
        }
    }
    return;
}


void Server::clientSocketDisconnectedSlot()
{
//    sender()->deleteLater();
    sender()->objectName();
    qDebug() << "a socket down" << ((QTcpSocket*)sender())->peerAddress()
             << " " << ((QTcpSocket*)sender())->peerPort();
}

void Server::MSGError(QAbstractSocket::SocketError type)
{

       switch(type)
       {
       case QAbstractSocket::RemoteHostClosedError://客户端断开
       {
            qDebug() << "客户端断开";
           break;
       }
       default:
       {
            qDebug() << "客户端断开-----";
           break;
       }
       }
}

