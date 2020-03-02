#include "ChargeMoneyHandler.h"

ChargeMoneyHandler::ChargeMoneyHandler()
{

}

void ChargeMoneyHandler::handler(QTcpSocket *client, const Protocol &pack)
{
    QString userName = pack["userName"].toString();
    QString userPassword = pack["userPassword"].toString();
    float money = pack["chargeMoney"].toString().toFloat();

    User user(userName, userPassword);

    UserHandler uh;
    bool ret = uh.UserChargeMoney(user, money);

    Protocol packRet;

    if( !ret )
    {
        qDebug() << "充值失败";
        packRet.setType(Protocol::chargemoneyfailed);
        packRet["errText"] = "充值失败";
    }else {
        qDebug() << "充值成功";
        packRet.setType(Protocol::chargemoneysucceed);
        packRet["errText"] = "充值成功";
    }
    client->write(packRet.pack());

}
