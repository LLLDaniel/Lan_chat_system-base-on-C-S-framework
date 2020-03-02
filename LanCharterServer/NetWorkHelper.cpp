#include "NetWorkHelper.h"

NetWorkHelper* NetWorkHelper::instance;
QMutex NetWorkHelper::mutex;

NetWorkHelper::NetWorkHelper()
{

}

NetWorkHelper* NetWorkHelper::getInstance()
{
    if( instance == nullptr)
    {
        mutex.lock();
        if( instance == nullptr)
        {
            instance = new NetWorkHelper();
        }
        mutex.unlock();
    }
    return instance;
}

void NetWorkHelper::addClient(SocketHandler *client)
{
    this->clients.push_back(client);

}
