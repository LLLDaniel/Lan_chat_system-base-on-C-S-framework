#include "LogoutHandler.h"
#include "User.h"
#include "UserHandler.h"
#include <QVariant>

LogoutHandler::LogoutHandler()
{

}

void LogoutHandler::handler(QTcpSocket *client, const Protocol &pack)
{
    QString name = (pack)["userName"].toString();
    QString password = (pack)["userPassword"].toString();

    User user(name, password);

    UserHandler uh;
    uh.UserSetDownline(user);
//    if( !ret )
//    {
//        qDebug() << "登陆失败";
//        packRet.setType(Protocol::loginfailed);
//        packRet["errText"] = "登陆失败";
//    }else {
//        qDebug() << "登陆成功";
//        packRet.setType(Protocol::loginsucceed);
//        packRet["errText"] = "登陆成功";
//        //设置用户登录状态为true
//        uh.UserSetOnline(user);
//    }
//    client->write(packRet.pack());

}
