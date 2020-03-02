#-------------------------------------------------
#
# Project created by QtCreator 2020-01-13T16:49:14
#
#-------------------------------------------------

QT       += core gui network
QT       += multimediawidgets
QT += multimedia



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LanCharter
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        Login.cpp \
    User.cpp \
    IdChooseUi.cpp \
    UserUi.cpp \
    DominantUser.cpp \
    RoomUi.cpp \
    LoadingUi.cpp \
    VideoSurface.cpp \
    VideoHandler.cpp \
    AudioHandler.cpp \
    Danmu.cpp \
    VideoLabel.cpp \
    UserInfoDialog.cpp \
    ModifyPasswordDialog.cpp \
    UserChargeMoneyDialog.cpp

HEADERS += \
        Login.h \
    package.h \
    protocol.h \
    User.h \
    IdChooseUi.h \
    UserUi.h \
    DominantUser.h \
    RoomUi.h \
    LoadingUi.h \
    VideoSurface.h \
    void run();.h \
    VideoHandler.h \
    AudioHandler.h \
    Danmu.h \
    VideoLabel.h \
    UserInfoDialog.h \
    ModifyPasswordDialog.h \
    UserChargeMoneyDialog.h

FORMS += \
        Login.ui \
    IdChooseUi.ui \
    UserUi.ui \
    RoomUi.ui \
    LoadingUi.ui \
    UserInfoDialog.ui \
    ModifyPasswordDialog.ui \
    UserChargeMoneyDialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc
