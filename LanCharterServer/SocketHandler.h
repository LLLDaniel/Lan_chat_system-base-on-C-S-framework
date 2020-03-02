#ifndef SOCKETHANDLER_H
#define SOCKETHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include "protocol.h"
#include <QByteArray>
#include "FactoryHandler.h"


class SocketHandler : public QObject
{
    Q_OBJECT
public:
    explicit SocketHandler(QObject *parent = nullptr);
    explicit SocketHandler(QTcpSocket* g_clientSocket, QObject *parent = nullptr);

    const QTcpSocket* getClientSocket() const { return this->clientSocket; }

signals:

public slots:
    void readReadySlot();

private:
    QTcpSocket* clientSocket;
    QHostAddress clientAdress;
    int port;
    QString name;

};

#endif // SOCKETHANDLER_H
