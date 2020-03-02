#include "LoginHandler.h"
#include "User.h"
#include "UserHandler.h"
#include <QVariant>

LoginHandler::LoginHandler()
{

}

void LoginHandler::handler(QTcpSocket *client, const Protocol &pack)
{
    QString name = (pack)["userName"].toString();
    QString password = (pack)["userPassword"].toString();

    User user(name, password);

    UserHandler uh;
    bool ret = uh.UserLogin(user);
    Protocol packRet;

    if( !ret )
    {
        qDebug() << "登陆失败";
        packRet.setType(Protocol::loginfailed);
        packRet["errText"] = "登陆失败";
    }else {
        qDebug() << "登陆成功";
        packRet.setType(Protocol::loginsucceed);
        packRet["errText"] = "登陆成功";
        //设置用户登录状态为true
        uh.UserSetOnline(user);
    }
    client->write(packRet.pack());
}
