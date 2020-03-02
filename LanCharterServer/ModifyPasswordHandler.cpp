#include "ModifyPasswordHandler.h"

ModifyPasswordHandler::ModifyPasswordHandler()
{

}

void ModifyPasswordHandler::handler(QTcpSocket *client, const Protocol &pack)
{
    QString name = pack["userName"].toString();
    QString passWord = pack["userOldPassword"].toString();
    QString newPassword = pack["userNewPassword"].toString();

    qDebug() << "userName" << name;
    qDebug() << "userOldPassword" << passWord;
    qDebug() << "userNewPassword" << newPassword;

    User user(name, passWord);

    UserHandler uh;
    bool ret = uh.UserModifyPassword(user, newPassword);

    Protocol packRet;

    if( !ret )
    {
        qDebug() << "修改失败";
        packRet.setType(Protocol::modifypasswordfaild);
        packRet["errText"] = "修改失败";
    }else {
        qDebug() << "修改成功";
        packRet.setType(Protocol::modifypasswordsucceed);
        packRet["errText"] = "修改成功";
        packRet["userNewPassword"] = newPassword;
    }
    client->write(packRet.pack());
}
