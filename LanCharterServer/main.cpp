#include <QCoreApplication>
#include <QDebug>
#include "Server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server(&a);

    return a.exec();
}
