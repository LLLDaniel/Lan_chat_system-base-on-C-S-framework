#ifndef FACTORYHANDLER_H
#define FACTORYHANDLER_H
#include "IHandler.h"
#include "protocol.h"
#include "RegistHandler.h"
#include "LoginHandler.h"
#include "LogoutHandler.h"
#include "CreateRoomHandler.h"
#include "GetRoomInfoHelper.h"
#include "DestroyRoomHandler.h"
#include "TouristEnterRoom.h"
#include "TouristExitRoom.h"
#include "GetRoommateInfoHelper.h"
#include "GetPersonInfoHandler.h"
#include "ChatHandler.h"
#include "ModifyPasswordHandler.h"
#include "RoomStopVideoHandler.h"
#include "ChargeMoneyHandler.h"
#include "GiveGiftHandler.h"

class FactoryHandler
{
public:
    FactoryHandler();
    IHandler* createFactory(Protocol::dateType type = Protocol::none);
};

#endif // FACTORYHANDLER_H
