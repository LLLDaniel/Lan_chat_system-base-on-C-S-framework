#ifndef CHARGEMONEYHANDLER_H
#define CHARGEMONEYHANDLER_H
#include "IHandler.h"
#include "User.h"
#include "UserHandler.h"
#include <QVariant>

class ChargeMoneyHandler:public IHandler
{
public:
    ChargeMoneyHandler();
    void handler(QTcpSocket* client, const Protocol& pack);
};

#endif // CHARGEMONEYHANDLER_H
