#include "RegistHandler.h"

RegistHandler::RegistHandler()
{

}

void RegistHandler::handler(QTcpSocket *client, const Protocol& pack)
{
    qDebug() << "begin regist";
    qDebug() << (pack)["userName"];
    QString name = (pack)["userName"].toString();
    QString password = (pack)["userPassword"].toString();

    User user(name, password);
    UserHandler uh;
    bool ret = uh.UserInsert(user);

    Protocol packRet;
    if( !ret )
    {
        qDebug() << "注册失败";
        packRet.setType(Protocol::registfailed);
        packRet["errText"] = "注册失败";
    }else {
        qDebug() << "注册成功";
        packRet.setType(Protocol::registsucceed);
        packRet["errText"] = "注册成功";
    }
    client->write(packRet.pack());
}
