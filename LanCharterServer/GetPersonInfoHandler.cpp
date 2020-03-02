#include "GetPersonInfoHandler.h"

GetPersonInfoHandler::GetPersonInfoHandler()
{

}

void GetPersonInfoHandler::handler(QTcpSocket *client, const Protocol &pack)
{
    QString name = (pack)["userName"].toString();
    QString password = (pack)["userPassword"].toString();

    User user(name, password);

    UserHandler uh;
//    uh.UserSetDownline(user);
    bool ret = uh.UserGetPersonInfo(user);
    qDebug() << "查新结果" << ret;
    Protocol packRet;

    if( !ret )
    {
        qDebug() << "查询失败";
        packRet.setType(Protocol::getpersoninfofailed);
        packRet["errText"] = "查询失败";
    }else {
        qDebug() << "查询成功";
        packRet.setType(Protocol::getpersoninfosucceed);
        packRet["errText"] = "查询成功";
        packRet["userName"] = user.getName();
        packRet["userPassword"] = user.getPassword();

        float money = user.getMoney();
        QString smoney = QString("%1").arg((double)money);
        qDebug() << "the money is " << smoney;
        packRet["money"] = smoney;
    }
    client->write(packRet.pack());

}
