#include "UserHandler.h"
#include <QVariant>
#include <QDebug>

UserHandler::UserHandler()
{

}

bool UserHandler::UserInsert(const User &user)
{
    DBHelper* instance = DBHelper::getInstance();
    instance->connectSqlBase();
    QSqlQuery query;
    query.prepare("insert into lct_tb_user(user_name, user_password) values(:name, :password);");
    query.bindValue(":name", user.getName());
    query.bindValue(":password", user.getPassword());
    bool ret = query.exec();
    if( !ret )
    {
        qDebug() << query.lastError().text();
    }

    instance->closeSqlBase();
    return ret;
}

bool UserHandler::UserLogin(const User &user)
{
    DBHelper* instance = DBHelper::getInstance();
    instance->connectSqlBase();
    QSqlQuery query;
    query.prepare("select * from lct_tb_user where user_name = :name "
                  "and user_password = :password and onLine = false;");
    query.bindValue(":name", user.getName());
    query.bindValue(":password", user.getPassword());
    bool ret = query.exec();
    if( !ret )
    {
        qDebug() << query.lastError().text();
    }

    //关闭数据库
    instance->closeSqlBase();
    return query.next();   //如果查到结果，，会是true
}

void UserHandler::UserSetOnline(const User &user)
{
    DBHelper* instance = DBHelper::getInstance();
    instance->connectSqlBase();
    QSqlQuery query;
    query.prepare("update lct_tb_user set onLine = true where user_name = :name "
                  "and user_password = :password and onLine = false;");
    query.bindValue(":name", user.getName());
    query.bindValue(":password", user.getPassword());
    query.exec();
    //关闭数据库
    instance->closeSqlBase();
}

void UserHandler::UserSetDownline(const User &user)
{
    DBHelper* instance = DBHelper::getInstance();
    instance->connectSqlBase();
    QSqlQuery query;
    query.prepare("update lct_tb_user set onLine = false where user_name = :name and user_password = :password and onLine = true;");
    query.bindValue(":name", user.getName());
    query.bindValue(":password", user.getPassword());

    bool ret = query.exec();
    if( !ret )
    {
        qDebug() << query.lastError().text();
    }
    //关闭数据库
    instance->closeSqlBase();
    qDebug()<< "make it down" << user.getName() << user.getPassword();
}

bool UserHandler::UserChargeMoney(const User &user, float money)
{
    DBHelper* instance = DBHelper::getInstance();
    instance->connectSqlBase();
    QSqlQuery query;
    query.prepare("update lct_tb_user set user_money = user_money + :money where user_name = :name and user_password = :password;");

    query.bindValue(":name", user.getName());
    query.bindValue(":password", user.getPassword());
    query.bindValue(":money", money);

    bool ret = query.exec();
    if( !ret )
    {
        qDebug() << query.lastError().text();
    }
    //关闭数据库
    instance->closeSqlBase();
    qDebug()<< "charge money is " << user.getName() << user.getPassword() << money;
    return ret;
}

bool UserHandler::giveGift(const Room &room, User &user, int giftMoney, const QString& giftName)
{
    //给用户扣钱
    int ret = UserSubGiftMoney(user, giftMoney);
    if( !ret ) return ret;
    //给主播加钱
    RoomMasterAddGiftMoney(room, giftMoney);

    return ret;
}

bool UserHandler::UserSubGiftMoney(User &user, int giftMoney)
{
    DBHelper* instance = DBHelper::getInstance();
    instance->connectSqlBase();
    QSqlQuery query;

    //判断余额够不够
    query.prepare("select * from lct_tb_user where user_name = :name;");
    query.bindValue(":name", user.getName());

    bool ret = query.exec();
    if( !ret )
    {
        qDebug() << query.lastError().text();
    }

    if( query.next())
    {
        float money = query.value(3).toFloat();
        user.setMoney(money);
        if( (money - giftMoney) < 0 )
        {
            qDebug() << " money is not enough";
            return false;
        }
    }

    query.prepare("update lct_tb_user set user_money = user_money - :money where user_name = :name;");

    query.bindValue(":name", user.getName());
    query.bindValue(":money", giftMoney);

    ret = query.exec();
    if( !ret )
    {
        qDebug() << query.lastError().text();
    }
    //关闭数据库
    instance->closeSqlBase();
    qDebug()<< "gift sub money is " << user.getName() << user.getPassword() << giftMoney;
    return ret;
}

bool UserHandler::RoomMasterAddGiftMoney(const Room &room, int giftMoney)
{
    DBHelper* instance = DBHelper::getInstance();
    instance->connectSqlBase();
    QSqlQuery query;
    query.prepare("update lct_tb_user set user_money = user_money + :money where user_name = :name;");

    query.bindValue(":name", room.getRoomName());
    query.bindValue(":money", giftMoney);

    bool ret = query.exec();
    if( !ret )
    {
        qDebug() << query.lastError().text();
    }
    //关闭数据库
    instance->closeSqlBase();
    qDebug()<< "gift add money is " << room.getRoomName() << giftMoney;
    return ret;
}


bool UserHandler::UserGetPersonInfo(User &user)
{
    DBHelper* instance = DBHelper::getInstance();
    instance->connectSqlBase();
    QSqlQuery query;
    query.prepare("select * from lct_tb_user where user_name = :name "
                  "and user_password = :password;");
    query.bindValue(":name", user.getName());
    query.bindValue(":password", user.getPassword());

    bool ret = query.exec();
    if(!ret){
        qDebug() << query.lastError().text();
    }

    //获取结果
    if( (ret = query.next()) ){        //获取行记录，并自动迭代
       float money = query.value(3).toFloat();
       user.setMoney(money);
    }

    //关闭数据库
    instance->closeSqlBase();
    qDebug()<< "getPerson info" << user.getName() << user.getPassword() << user.getMoney() << " ";
    return ret;   //如果查到结果，，会是true
}

bool UserHandler::UserModifyPassword(const User &user, const QString &newPassword)
{
    DBHelper* instance = DBHelper::getInstance();
    instance->connectSqlBase();

    QSqlQuery query;
    query.prepare("update lct_tb_user set user_password = :newPassword where user_name = :name "
                  "and user_password = :password;");
    query.bindValue(":name", user.getName());
    query.bindValue(":password", user.getPassword());
    query.bindValue(":newPassword", newPassword);

    bool ret = query.exec();
    if( !ret )
    {
        qDebug() << query.lastError().text();
    }

    //关闭数据库
    instance->closeSqlBase();
    qDebug()<< "modify user password " << user.getName() << user.getPassword() << " ";
    return ret;   //如果查到结果，，会是true
}

bool UserHandler::UserCreateRoom(QTcpSocket *client, const User &user)
{
    Room newroom(user.getName(),client);

    RoomHelper* instance = RoomHelper::getInstance();

    return instance->addRoom(newroom);

}

groupInfo_t UserHandler::UserEnterRoom(const Room& room, const Acount_t user_t)
{
    RoomHelper* instance = RoomHelper::getInstance();
    QVector<Room>& rooms = instance->getRooms();

    groupInfo_t packRet;
    memset(&packRet, 0, sizeof (packRet));

    for (int i = 0; i < rooms.size() ; i++) {
        qDebug() << room.getRoomId() <<" " << room.getRoomName()<< "------";
        qDebug() << rooms[i].getRoomId() <<" " << rooms[i].getRoomName();
        if( rooms[i].getRoomId() == room.getRoomId()
                && rooms[i].getRoomName() == room.getRoomName() )
        {
            rooms[i].addAccouts(user_t);

            packRet.ret = true;
            packRet.groupPort = rooms[i].getGroupPort();
            packRet.audioGroupAdress = rooms[i].getAudioGroupAdress();
            qDebug() << rooms[i].getAudioGroupAdress().toString();
            packRet.videoGroupAdress = rooms[i].getVideoGroupAdress();
            qDebug() << rooms[i].getVideoGroupAdress().toString();
            qDebug() << "add acount succeed" << rooms.size()<< " " << rooms[i].getRoomAcount_t().size();
            return packRet;
        }
    }
    packRet.ret = false;
    return packRet;
}

bool UserHandler::UserExitRoom(const Room& room, const Acount_t user_t)
{
    RoomHelper* instance = RoomHelper::getInstance();
    QVector<Room>& rooms = instance->getRooms();
    for (int i = 0; i < rooms.size() ; i++) {
        qDebug() << room.getRoomId() <<" " << room.getRoomName()<< "------";
        qDebug() << rooms[i].getRoomId() <<" " << rooms[i].getRoomName();
        if( rooms[i].getRoomId() == room.getRoomId()
                && rooms[i].getRoomName() == room.getRoomName() )
        {
            rooms[i].deleteAccouts(user_t);
            qDebug() << "delete acount succeed" << rooms.size()<< " " << rooms[i].getRoomAcount_t().size();
            return true;
        }
    }
    return false;
}


QVector<Room> UserHandler::UserGetRoomInfo()
{
    RoomHelper* instance = RoomHelper::getInstance();
    return instance->getRooms();
}


QVector<Acount_t> UserHandler::UserGetRoommateInfo(Room& room)
{
    RoomHelper* instance = RoomHelper::getInstance();

    QVector<Room> rooms = instance->getRooms();
    QVector<Acount_t> acount;
    qDebug() << room.getRoomId() << " " << room.getRoomName() << "====";
    for(int i =0; i< rooms.size(); i++)
    {
        qDebug() << rooms[i].getRoomId() << " " << rooms[i].getRoomName() << "+++";
        if( rooms[i].getRoomId() == room.getRoomId()
                && rooms[i].getRoomName() == room.getRoomName())
        {
            acount = rooms[i].getRoomAcount_t();
            qDebug() << acount.size() << "0-0-0-0-";
            return acount;
        }
    }
    return acount;
}



