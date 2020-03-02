#include "FactoryHandler.h"

FactoryHandler::FactoryHandler()
{

}

IHandler *FactoryHandler::createFactory(Protocol::dateType type)
{
    qDebug() << " last mess";
    IHandler* handler = nullptr;
    switch (type) {
    case Protocol::login:
        handler = new LoginHandler();
        break;
    case Protocol::regist:
        handler = new RegistHandler();
        break;
    case Protocol::creatliveroom:
        handler = new CreateRoomHandler();
        break;
    case Protocol::getRoomInfos:
        handler = new GetRoomInfoHelper();
        break;
    case Protocol::destroyliveroom:
        qDebug() << "begin destroy room";
        handler = new DestroyRoomHandler();
        break;
    case Protocol::exitliveroom:
        qDebug() << "exit room";
        handler = new TouristExitRoom();
        break;
    case Protocol::enterliveroom:
        handler = new TouristEnterRoom();
        break;
    case Protocol::getRoomMateInfos:
        qDebug() << "get roommate";
        handler = new GetRoommateInfoHelper();
        break;
    case Protocol::chat:
        qDebug() << "chat";
        handler = new ChatHandler();
        break;
    case Protocol::logout:
        qDebug() << "logout";
        handler = new LogoutHandler();
        break;
    case Protocol::getpersoninfo:
        qDebug() << "getpersoninfo";
        handler = new GetPersonInfoHandler();
        break;
    case Protocol::modifypassword:
        qDebug() << "modifypassword";
        handler = new ModifyPasswordHandler();
        break;
    case Protocol::videosharestop:
        qDebug() << "videosharestop";
        handler = new RoomStopVideoHandler();
        break;
    case Protocol::chargemoney:
        qDebug() << "chargemoney";
        handler = new ChargeMoneyHandler();
        break;
    case Protocol::givinggift:
        qDebug() << "givinggift";
        handler = new GiveGiftHandler();
        break;
    default:
        break;
    }
    return handler;
}
