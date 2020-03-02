#include "SocketHandler.h"

SocketHandler::SocketHandler(QObject *parent) : QObject(parent)
{

}


SocketHandler::SocketHandler(QTcpSocket* g_clientSocket, QObject *parent)
    : QObject(parent),clientSocket(g_clientSocket)

{
    //设置地址还有端口
    clientAdress = clientSocket->peerAddress();
    port = clientSocket->peerPort();
    //绑定套接字信号
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(readReadySlot()));
}

void SocketHandler::readReadySlot()
{
    Protocol packReq;
    QByteArray byte = this->clientSocket->readAll();

//    qDebug() << byte.size() << " ----";
    int len = 0;
    while ( len = packReq.unpack(byte) > 0 ) {
        //创建工厂来处理不同的请求

        FactoryHandler ft;
        qDebug() << packReq.getType() << "-----" << len;
        IHandler* handler = ft.createFactory(packReq.getType());
        //执行产品操作
        if( handler != nullptr )
        {
            qDebug() << "进入handler";
            handler->handler(this->clientSocket, packReq);
            delete handler;
        }

        byte = byte.mid(len);
    }
}
