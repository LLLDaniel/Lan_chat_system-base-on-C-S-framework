QT -= gui
QT       +=  network
QT       +=  sql
CONFIG += c++11 console
CONFIG -= app_bundle


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    DBHelper.cpp \
    User.cpp \
    Server.cpp \
    SocketHandler.cpp \
    NetWorkHelper.cpp \
    IHandler.cpp \
    FactoryHandler.cpp \
    RegistHandler.cpp \
    UserHandler.cpp \
    LoginHandler.cpp \
    Room.cpp \
    RoomHelper.cpp \
    CreateRoomHandler.cpp \
    GetRoomInfoHelper.cpp \
    DestroyRoomHandler.cpp \
    TouristEnterRoom.cpp \
    GetRoommateInfoHelper.cpp \
    TouristExitRoom.cpp \
    ChatHandler.cpp \
    LogoutHandler.cpp \
    GetPersonInfoHandler.cpp \
    ModifyPasswordHandler.cpp \
    RoomStopVideoHandler.cpp \
    ChargeMoneyHandler.cpp \
    GiveGiftHandler.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    DBHelper.h \
    User.h \
    Server.h \
    SocketHandler.h \
    NetWorkHelper.h \
    protocol.h \
    IHandler.h \
    FactoryHandler.h \
    RegistHandler.h \
    UserHandler.h \
    LoginHandler.h \
    Room.h \
    RoomHelper.h \
    CreateRoomHandler.h \
    GetRoomInfoHelper.h \
    DestroyRoomHandler.h \
    TouristEnterRoom.h \
    GetRoommateInfoHelper.h \
    TouristExitRoom.h \
    ChatHandler.h \
    LogoutHandler.h \
    GetPersonInfoHandler.h \
    ModifyPasswordHandler.h \
    RoomStopVideoHandler.h \
    ChargeMoneyHandler.h \
    GiveGiftHandler.h
