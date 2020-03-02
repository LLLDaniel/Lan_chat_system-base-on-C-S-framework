#include "UserUi.h"
#include "ui_UserUi.h"

UserUi::UserUi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserUi)
{
    ui->setupUi(this);
    setWindowTitle("please choose your server");
    this->setFixedSize(QSize(496,625));
    roomUi = nullptr;
    userinfodialog = new UserInfoDialog(this);
    userchargemoneydialoy = new UserChargeMoneyDialog(this);
    modifypassworddialog = new ModifyPasswordDialog(this);

    DominantUser* instance = DominantUser::getInstance();
    client_socket = instance->getTcpSocket();

    QObject::connect(client_socket, SIGNAL(readyRead()), this, SLOT(readReadySlot()));
    QObject::connect(ui->list_room, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                     this, SLOT(doubleClickSlot(QListWidgetItem*)));


    on_btn_flashroom_clicked();
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(slotTimeOut()));
    m_timer.start(5000);
}

UserUi::~UserUi()
{
    delete ui;
    if( roomUi != nullptr )
    {
        delete roomUi;
    }
}

void UserUi::on_btn_user_info_clicked()
{
    qDebug() << "get person info";
    DominantUser* instance = DominantUser::getInstance();

    QTcpSocket* clientSocket = instance->getTcpSocket();
    User user = instance->getUser();

    Protocol packReq;
    packReq.setType(Protocol::getpersoninfo);
    packReq["userName"] = user.getName();
    packReq["userPassword"] = user.getPassword();

    clientSocket->write(packReq.pack());

}

void UserUi::on_btn_exit_clicked()
{
    this->hide();
    emit userUiExit();
}

void UserUi::on_btn_makeroom_clicked()
{
    qDebug() << "begin makeroom";
    DominantUser* instance = DominantUser::getInstance();
    if( instance->getBeRoomMaster() )  //防止重复创建房间
    {
        QMessageBox::warning(this, "failed", "can't create live room again");
        return;
    }
    if( instance->getUser().getIndentity() == User::TOURIST )  //用户不能创建房间
    {
        QMessageBox::warning(this, "failed", "you are tourist, can't not create live room");
        return;
    }
    QTcpSocket* clientSocket = instance->getTcpSocket();
    User user = instance->getUser();

    Protocol packReq;
    packReq.setType(Protocol::creatliveroom);
    packReq["userName"] = user.getName();

    clientSocket->write(packReq.pack());
}

void UserUi::on_btn_modify_password_clicked()
{
    qDebug() << "modify user password";

    //显示修改密码界面
    this->modifypassworddialog->exec();
}

void UserUi::on_btn_flashroom_clicked()
{
    qDebug() << "flash start";
    DominantUser* instance = DominantUser::getInstance();
    QTcpSocket* clientSocket = instance->getTcpSocket();

    Protocol packReq;
    packReq.setType(Protocol::getRoomInfos);

    clientSocket->write(packReq.pack());
    ui->list_room->clear();
    QString head = "  房间编号\t\t房间名\t\t房间在线人数  ";
    ui->list_room->addItem(head);
}

void UserUi::doubleClickSlot(QListWidgetItem *item)
{
    qDebug() << item->text();
    if( item->text() == "  房间编号\t\t房间名\t\t房间在线人数  ")
        return;
    QStringList list = item->text().split("\t\t");
    //标记进入房间的编号
     DominantUser* instance = DominantUser::getInstance();
     instance->setRoomId(list[0].toInt());
     instance->setRoomName(list[1]);

     //普通用户进入房间函数
     QTcpSocket* clientSocket = instance->getTcpSocket();

     Protocol packReq;
     packReq.setType(Protocol::enterliveroom);
     packReq["roomId"] = list[0].toInt();
     packReq["roomName"] = list[1];
     packReq["userName"] = instance->getUser().getName();

     clientSocket->write(packReq.pack());
}

void UserUi::readReadySlot()  //接收数据
{
    QByteArray ret = this->client_socket->readAll();
    qDebug() << "userUI recived";
    //解包
    Protocol packRet;
    int len = 0;
    while ( (len = packRet.unpack(ret)) > 0) {
         qDebug() << "userUI recived  1";
        switch (packRet.getType()) {
        case Protocol::creatliveroomsucceed:
        {
            QMessageBox::information(this, "提示", packRet["errText"].toString());
            int roomId = packRet["roomId"].toInt();
            QString roomName = packRet["roomName"].toString();
            qDebug() << roomName << "dfsfsdfsdfsfsdf";
            int groupPort = packRet["groupPort"].toInt();
            QString audioIp = packRet["audioIp"].toString();
            QString videoIp = packRet["videoIp"].toString();
            creatliveroomsucceed(roomId, roomName, groupPort, audioIp, videoIp);
        }
            break;
        case Protocol::creatliveroomfailed:
            QMessageBox::information(this, "提示", packRet["errText"].toString());
            break;
        case Protocol::enterliveroomsucceed:
        {
            qDebug() << "tourist enter live roomsucceed";
            int roomId = packRet["roomId"].toInt();
            QString roomName = packRet["roomName"].toString();
            int groupPort = packRet["groupPort"].toInt();            
            QString audioIp = packRet["audioIp"].toString();          
            QString videoIp = packRet["videoIp"].toString();

            qDebug() << roomId << "roomId";
            qDebug() << roomName << "roomName";
            qDebug() << groupPort << "groupPort";
            qDebug() << audioIp << "audioip";
            qDebug() << videoIp << "videoip";
            enterliveroomsucceed(roomId, roomName, groupPort, audioIp, videoIp);
        }
            break;
        case Protocol::enterliveroomfailed:
            QMessageBox::information(this, "提示", packRet["errText"].toString());
            on_btn_flashroom_clicked();
            break;
        case Protocol::exitliveroomsucceed:
            qDebug() << "exitliveroomsucceed";
            exitliveroomsucceed();
            break;
        case Protocol::exitliveroomfailed:
            break;
        case Protocol::destroyliveroomsucceed:
            qDebug() << "3234234";
            QMessageBox::information(this, "提示", packRet["errText"].toString());
            destroyliveroomsucceed();
            break;
        case Protocol::destroyliveroomfailed:
            QMessageBox::information(this, "提示", packRet["errText"].toString());
            break;
        case Protocol::getRoomInfos:
            getRoomInfos(packRet);
            break;
        case Protocol::getpersoninfosucceed:
            getUserPersonInfos(packRet);
            break;
        case Protocol::modifypasswordsucceed:
            QMessageBox::information(this, "提示", packRet["errText"].toString());
            modifyUserPassword(packRet);
            break;
        case Protocol::chargemoneysucceed:
            QMessageBox::information(this, "提示", packRet["errText"].toString());
            break;
        case Protocol::chargemoneyfailed:
            QMessageBox::information(this, "提示", packRet["errText"].toString());
            break;
        default:
            break;
        }
        ret = ret.mid(len);
    }
}

void UserUi::slotTimeOut()
{
    //使用定时器来刷新列表
    on_btn_flashroom_clicked();
}

//主播房间创建成功后的操作
void UserUi::creatliveroomsucceed(int roomId , QString roomName,
                                                int groupPort, QString audioIp, QString videoIp )
{
    //将用户的状态设置为主播，
    DominantUser* instance = DominantUser::getInstance();
    instance->setBeRoomMaster(true);
    //标记进入的房间
    instance->setRoomId(roomId);
    instance->setRoomName(roomName);
    instance->setGroupPort(groupPort);
    instance->setAudioGroupAddress(audioIp);
    instance->setVideoGroupAddress(videoIp);

    //切换进入房间界面
    QObject::disconnect(client_socket, SIGNAL(readyRead()), this, SLOT(readReadySlot()));
    if( roomUi == nullptr )
    {
        roomUi = new RoomUi();
        connect(roomUi, &RoomUi::RoomUiClose, this, [=](){
            this->show();
            QObject::connect(client_socket, SIGNAL(readyRead()), this, SLOT(readReadySlot()));
        });

    }
    qDebug() << "23232323";

    this->hide();
    qDebug() << "23232323";
    this->roomUi->show();
    //重新连接可读信号
    QObject::connect(client_socket, SIGNAL(readyRead()), this->roomUi, SLOT(readReadySlot()));
    qDebug() << "23232323";
}

//房间销毁成功后的操作
void UserUi::destroyliveroomsucceed()
{
    //将用户的状态设置为无，
    qDebug() << "destroy succeed";
    DominantUser* instance = DominantUser::getInstance();
    instance->setBeRoomMaster(false);

    //标记房间清零
    instance->setRoomId(0);

    //删除roomui
    delete this->roomUi;
    this->roomUi = nullptr;
}

//游客进入房间成功后的操作
void UserUi::enterliveroomsucceed(int roomId , QString roomName,
                                                int groupPort, QString audioIp, QString videoIp)
{
    DominantUser* instance = DominantUser::getInstance();

    //标记进入的房间
    instance->setGroupPort(groupPort);
    instance->setAudioGroupAddress(audioIp);
    instance->setVideoGroupAddress(videoIp);

    instance->setRoomId(roomId);
    instance->setRoomName(roomName);

    //切换进入房间界面,
    QObject::disconnect(client_socket, SIGNAL(readyRead()), this, SLOT(readReadySlot()));

    if( roomUi == nullptr )
    {
        roomUi = new RoomUi();
        connect(roomUi, &RoomUi::RoomUiClose, this, [=](){
            this->show();
            QObject::connect(client_socket, SIGNAL(readyRead()), this, SLOT(readReadySlot()));
        });
    }

    this->hide();
    QString title = instance->getRoomName()+"'s living room";
    roomUi->setWindowTitle(title);
    roomUi->show();

    //重新连接可读信号
    QObject::connect(client_socket, SIGNAL(readyRead()), this->roomUi, SLOT(readReadySlot()));
}

//游客离开房间成功后的操作
void UserUi::exitliveroomsucceed()
{
    delete this->roomUi;
    this->roomUi = nullptr;
}

//获取房间信息
void  UserUi::getRoomInfos(Protocol& packRet)
{
    QString roomId = QString::number(packRet["roomId"].toInt());
    QString roomName = packRet["roomName"].toString();
    QString roomAcount = QString::number(packRet["roomAcount"].toInt());
    QString item = roomId + "\t\t" + roomName + "\t\t" + roomAcount;

    ui->list_room->addItem(item);
}

void  UserUi::getUserPersonInfos(Protocol& packRet)
{
    QString userName = packRet["userName"].toString();
    QString userPassword = packRet["userPassword"].toString();
    QString userMoney = packRet["money"].toString();
    qDebug() << "money is " << userMoney;
    this->userinfodialog->getUserInfo(userName, userPassword, userMoney);
    this->userinfodialog->exec();
}

void  UserUi::modifyUserPassword(Protocol& packRet)
{
    QString newPassword = packRet["userNewPassword"].toString();
    qDebug() << "get the new password" << newPassword;
    DominantUser* instance = DominantUser::getInstance();
    instance->getUser().setPassword(newPassword);

    qDebug() << instance->getUser().getPassword() << "777777";

    //修改本地的user对象
    this->user.setPassword(newPassword);
}


//重写关闭事件，使其关闭的时候向服务端发送登出请求
void UserUi::closeEvent(QCloseEvent *event)
{
    qDebug() << "begin ogout";
    if( QMessageBox::question(this, "提示", "are you sure exit?") == QMessageBox::Yes )
    {
        qDebug() << "begin ogout  1 userui";
        DominantUser* instance = DominantUser::getInstance();
        User user_t = instance->getUser();
        //在最后发送一个下线请求
        Protocol packReq;
        packReq.setType(Protocol::logout);

        QString userName = user_t.getName();
        QString userPassword = user_t.getPassword();
        qDebug() << userName;
        qDebug() << userPassword;
        packReq["userName"] = userName;
        packReq["userPassword"] = userPassword;

        client_socket->write(packReq.pack());
    }else{
        qDebug() << "begin ogout  2";
        event->ignore();
        return;
    }
}

void UserUi::on_btn_charge_clicked()
{
    qDebug() << "user charge money";

    //显示充值界面
    this->userchargemoneydialoy->exec();
}
