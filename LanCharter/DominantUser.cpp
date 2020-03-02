#include "DominantUser.h"

QMutex DominantUser::mutex;
DominantUser* DominantUser::instance = nullptr;

DominantUser::DominantUser()
{
    beRoomMaster = false;
}

DominantUser* DominantUser::getInstance()
{
    if( instance == nullptr )
    {
       mutex.lock();
       if( instance == nullptr )
       {
           instance = new DominantUser();
       }
       mutex.unlock();
    }
    return instance;
}
