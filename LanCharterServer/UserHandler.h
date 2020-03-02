#ifndef USERHANDLER_H
#define USERHANDLER_H
#include "User.h"
#include "DBHelper.h"
#include <QSqlQuery>
#include <QTcpSocket>
#include "Room.h"
#include "RoomHelper.h"

class UserHandler
{
public:   
    UserHandler();
    bool UserInsert(const User& user);
    bool UserLogin(const User& user);

    void UserSetOnline(const User& user);
    void UserSetDownline(const User& user);

    //用户充值
    bool UserChargeMoney(const User& user, float money);

    //用户送礼
    bool giveGift(const Room& room, User& user, int giftMoney,const QString& giftName);
    bool UserSubGiftMoney(User&user, int giftMoney);
    bool RoomMasterAddGiftMoney(const Room&room, int giftMoney);


    //用户查询自己的信息
    bool UserGetPersonInfo(User& user);

    //用户修改密码
    bool UserModifyPassword(const User& user, const QString& newPassword);

    //用户创建房间
    bool UserCreateRoom(QTcpSocket *client, const User& user);

    //游客用户进入房间
    groupInfo_t UserEnterRoom(const Room& room, const Acount_t user_t);
    //游客用户离开房间
    bool UserExitRoom(const Room& room, const Acount_t user_t);
    QVector<Room> UserGetRoomInfo();
    QVector<Acount_t> UserGetRoommateInfo(Room& room);

private:

};

#endif // USERHANDLER_H
