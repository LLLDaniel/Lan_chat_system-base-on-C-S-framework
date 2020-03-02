#include "RoomUi.h"
#include "ui_RoomUi.h"
#include <QCameraInfo>
#include <QDesktopWidget>


RoomUi::RoomUi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomUi)
{
    ui->setupUi(this);

//    this->setAttribute(Qt::WA_DeleteOnClose);

    DominantUser* instance = DominantUser::getInstance();
    client_socket = instance->getTcpSocket();

//    emit disconnectUserUiWithSocket();
    QObject::connect(client_socket, SIGNAL(readyRead()), this, SLOT(readReadySlot()));
    on_btn_flash_clicked();

    //创建视频管理对象
    this->videohandler = new VideoHandler(instance->getVideoGroupAddress(), instance->getGroupPort());
    this->videoThread = new QThread(this);
    videohandler->moveToThread(videoThread);
    connect(this->videohandler, SIGNAL(sendVideoToUi(QPixmap)),
                    this, SLOT(frameChangedSlot(QPixmap)));
    connect(this, SIGNAL(anchorStartVideo()),
                    this->videohandler, SLOT(anchorStartVideo()));
    connect(this, SIGNAL(anchorEndVideo()),
                    this->videohandler, SLOT(anchorEndVideo()));

    connect(this, SIGNAL(touristStartVideo()),
                    this->videohandler, SLOT(touristStartVideo()));
    connect(this, SIGNAL(touristEndVideo()),
                    this->videohandler, SLOT(touristEndVideo()));
    videoThread->start();

    move = new QMovie(":/image/timg.gif");
    ui->label->setMovie(move);
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label->setScaledContents(true);
    move->start();

    //初始化弹幕开关
    instance->setDanmu(true);
    //连接弹幕槽函数
    connect(this, SIGNAL(makeDanMu(QString)), this->ui->label, SLOT(makeDanMu(QString)));

    //创建音频管理对象
    this->audiohandler = new AudioHandler(instance->getAudioGroupAddress(), instance->getGroupPort());
    this->audioThread = new QThread(this);
    audiohandler->moveToThread(audioThread);
    audioThread->start();
    connect(this, SIGNAL(anchorStartAudio()),
                    this->audiohandler, SLOT(anchorStartAudio()));
    connect(this, SIGNAL(anchorEndAudio()),
                    this->audiohandler, SLOT(anchorEndAudio()));
    connect(this, SIGNAL(touristStartAudio()),
                    this->audiohandler, SLOT(touristStartAudio()));
    connect(this, SIGNAL(touristEndAudio()),
                    this->audiohandler, SLOT(touristEndAudio()));

    this->setWindowStyle();

}


void RoomUi::frameChangedSlot(QPixmap pixmap)
{
    qDebug() << "start show video";
    ui->label->setPixmap(pixmap.scaled(ui->label->size()));
}


RoomUi::~RoomUi()
{
    delete ui;
    videoThread->terminate();
    videoThread->wait();
    audioThread->terminate();
    audioThread->wait();

}


void RoomUi::on_btn_flash_clicked()
{
    ui->list_roommate->clear();
    QString head = "      姓名    |    热度";
    ui->list_roommate->addItem(head);
    DominantUser* instance = DominantUser::getInstance();
    QTcpSocket* clientSocket = instance->getTcpSocket();

    Protocol packReq(Protocol::getRoomMateInfos);
    int roomId = instance->getRoomId();
    QString roomName = instance->getRoomName();
    qDebug() << "get the room mate ____" << roomName;
    packReq["roomId"] = roomId;
    packReq["roomName"] = roomName;
    clientSocket->write(packReq.pack());
}


void RoomUi::getRoomMateInfos(Protocol& packRet)
{
    QString userName = packRet["userName"].toString();
    int userHeatNum = packRet["userHeatNum"].toInt();
    QString item = "      " +userName + "    |    " + QString::number(userHeatNum);

    ui->list_roommate->addItem(item);
}


void RoomUi::readReadySlot()
{
    QByteArray ret = this->client_socket->readAll();
    qDebug() << "RoomUi recived";
    //解包
    Protocol packRet;
    int len = 0;
    while ( (len = packRet.unpack(ret)) > 0) {
        qDebug() << "RoomUi recived  1";
        switch (packRet.getType()) {
        case Protocol::getRoomMateInfos:
            qDebug() << " get room mate";
            getRoomMateInfos(packRet);
            break;
        case Protocol::exitliveroomsucceed:
            QMessageBox::information(this, "提示", packRet["errText"].toString());
            break;
        case Protocol::exitliveroomfailed:
            QMessageBox::information(this, "提示", packRet["errText"].toString());
            break;
        case Protocol::chat:
            qDebug() << "chat";
            on_btn_flash_clicked();
            getChatText(packRet);
            break;
        case Protocol::anyoneexitliveroom:
            qDebug() <<"anyoneexiteliveroom";
            getRoomMateExitMessage(packRet);
            break;
        case Protocol::anyoneenterliveroom:
            qDebug() <<"anyoneenterliveroom";
            getRoomMateEnterMessage(packRet);
            break;
        case Protocol::roomoverdue:
            qDebug() <<"roomoverdue";
            getRoomOverDueMessage(packRet);
            break;
        case Protocol::videosharestop:
            qDebug() <<"videosharestop";
            handleVideoStop(packRet);
            break;
        case Protocol::anyonegivegifttoanchor:
            qDebug() <<"anyonegivegifttoanchor";
            handleAnyoneGiveGiftToAnchor(packRet);
            break;
        case Protocol::givinggiftsucceed:
            qDebug() <<"givinggiftsucceed";
            QMessageBox::information(this, "提示", packRet["errText"].toString());
            break;
        case Protocol::givinggiftfailed:
            qDebug() <<"givinggiftfailed";
            QMessageBox::information(this, "提示", packRet["errText"].toString());
            break;
        default:
            break;
        }
        ret = ret.mid(len);
    }
}

void RoomUi::on_btn_send_clicked()
{
    DominantUser* instance = DominantUser::getInstance();

    QString text = ui->lineEdit->text();
    //判断有没有消息，以及礼物
    if( text.isEmpty() &&
            ui->checkBox_flower->checkState() == Qt::Unchecked &&
            ui->checkBox_diamond->checkState() == Qt::Unchecked &&
            ui->checkBox_runcar->checkState() == Qt::Unchecked )
    {
        return;
    }else if( !text.isEmpty() &&
             ui->checkBox_flower->checkState() == Qt::Unchecked &&
             ui->checkBox_diamond->checkState() == Qt::Unchecked &&
             ui->checkBox_runcar->checkState() == Qt::Unchecked )
    {
        Protocol packReq;
        packReq["roomId"] = instance->getRoomId();
        packReq["userName"] = instance->getUser().getName();
        qDebug() << "fdsfsd" << instance->getUser().getName();
        if( !text.isEmpty() )
        {
            packReq.setType(Protocol::chat);

            packReq["chatText"] = text;

            client_socket->write(packReq.pack());
            ui->lineEdit->clear();
        }
    }


    int ret = getPermitToGiveGift();
    //判断用户愿不愿意送礼物
    if( !ret ) return;

    //发送礼物开始
    toGiveGift();
}

void RoomUi::getChatText(Protocol& pack)
{
    QString text = pack["chatText"].toString();
    //触发弹幕信号
    qDebug() << "触发弹幕信号";
    emit makeDanMu(text);
    text = "   " + text;
    QString userName = pack["userName"].toString();
    QString chat = userName + " says:";
    chat = QString("<font color=\'green\'>%1</b>").arg(chat);

    ui->textEdit->append(chat);
    ui->textEdit->append(text);
}

void RoomUi::getRoomMateExitMessage(Protocol& pack)
{
    on_btn_flash_clicked();
    QString userName = pack["userName"].toString();
    QString text = userName + " exit the room, bye bye";
    text = QString("<font color=\'red\'>%1</b>").arg(text);
    ui->textEdit->append(text);

}

void RoomUi::getRoomMateEnterMessage(Protocol& pack)
{
    on_btn_flash_clicked();
    QString userName = pack["userName"].toString();
    QString text = userName + " enter the room, welcome!!!~~~~";
    text = QString("<font color=\'purple\'>%1</b>").arg(text);
    ui->textEdit->append(text);

}

void RoomUi::handleAnyoneGiveGiftToAnchor(Protocol& pack)
{
    QString text = pack["errText"].toString();
    qDebug() << "触发弹幕信号";
    emit makeDanMu(text);
    text = QString("<font color=\'purple\'>%1</b>").arg(text);
    ui->textEdit->append(text);
}

void RoomUi::getRoomOverDueMessage(Protocol& pack)
{
    QString text = pack["errText"].toString();
    text = QString("<font color=\'red\'>%1</b>").arg(text);
    ui->textEdit->append(text);

    //设置视频屏保
    QTimer::singleShot(1000, this, [=](){
        ui->label->setMovie(move);
    });
}

void RoomUi::closeEvent(QCloseEvent *event)  //重写关闭事件，使得其关闭时给服务器发送退出房间信号
{
    qDebug() << "close roomui";
    DominantUser* instance = DominantUser::getInstance();
    QTcpSocket* clientSocket = instance->getTcpSocket();
    if( instance->getBeRoomMaster() )
    {
        //说明这是主播，离开要销毁房间
        if( QMessageBox::question(this, "提示", "are you sure leave the room?") == QMessageBox::Yes )
        {
            qDebug() <<" this is anchor leave";
            //向服务器发送销毁房间的通知
            Protocol packReq(Protocol::destroyliveroom);
            packReq["roomId"] = instance->getRoomId();
            packReq["roomName"] = instance->getUser().getName();
            qDebug() <<" want to delte the room " << instance->getRoomId()
                    << " " << instance->getUser().getName();
            clientSocket->write(packReq.pack());

            //触发关闭一下设备,初始化控件
            initWiget();
//            emit this->ui->btn_camera_close->clicked();
            qDebug() << "close camera";
            on_btn_camera_close_clicked();
            on_btn_make_close_clicked();

            //改变权限
            instance->setBeRoomMaster(false);

            //触发信号，让userUI显示
            emit RoomUiClose();
            QObject::disconnect(client_socket, SIGNAL(readyRead()), this, SLOT(readReadySlot()));
            this->close();

//            if( this->camera->status() == QCamera::ActiveStatus )
//            {
//                this->camera->stop();
//            }
        }else{
            event->ignore();
            return;
        }
    }else if( !instance->getBeRoomMaster() )  //普通用户退出房间
    {
        qDebug() <<" this is tourist leave";
        if( QMessageBox::question(this, "提示", "are you sure leave the room?") == QMessageBox::Yes)
        {
            Protocol packReq(Protocol::exitliveroom);
            packReq["roomId"] = instance->getRoomId();
            packReq["roomName"] = instance->getRoomName();
            packReq["userName"] = instance->getUser().getName();
            clientSocket->write(packReq.pack());

            //退出房间把房间标记清零
            instance->setRoomId(0);

            //初始化控件，关闭接收音频和视频
            initWiget();
            on_btn_make_close_clicked();
            on_btn_camera_close_clicked();

            //触发信号，让userUI显示
            emit RoomUiClose();
            QObject::disconnect(client_socket, SIGNAL(readyRead()), this, SLOT(readReadySlot()));
            this->close();
        }else{
            event->ignore();
            return;
        }
    }
}

void RoomUi::on_btn_camera_open_clicked()
{
    ui->btn_camera_open->setStyleSheet("background-color: rgb(175,238,238)");
    ui->btn_camera_close->setStyleSheet("background-color: rgb(255,255,255)");
    //判断用户的身份，确定是接受视频还是发送视频
    DominantUser* instance = DominantUser::getInstance();
    if(instance->getBeRoomMaster())
    {
       qDebug()<< " anchor video";
       emit anchorStartVideo();

    }else {
       qDebug() << " tourist video";
       QMessageBox::information(this, "提示", "只有房主才能打开摄像头,您将获取视频直播");
        emit touristStartVideo();
    }
}

void RoomUi::on_btn_camera_close_clicked()
{
    ui->btn_camera_close->setStyleSheet("background-color: rgb(175,238,238)");
    ui->btn_camera_open->setStyleSheet("background-color: rgb(255,255,255)");
    //关闭摄像头
    //判断用户的身份，确定是
    DominantUser* instance = DominantUser::getInstance();
    if(instance->getBeRoomMaster())
    {
       qDebug()<< " anchor video end ";
       emit anchorEndVideo();
       sendStopVideoToViewers();
       QTimer::singleShot(1000, this, [=](){
           ui->label->setMovie(move);
       });

    }else{
       qDebug() << " tourist video end ";
       emit touristEndVideo();
       QTimer::singleShot(1000, this, [=](){
           ui->label->setMovie(move);
       });
    }
}

void RoomUi::on_btn_make_open_clicked()
{
    ui->btn_make_open->setStyleSheet("background-color: rgb(175,238,238)");
    ui->btn_make_close->setStyleSheet("background-color: rgb(255,255,255)");
    //判断用户的身份，确定是接受视频还是发送音频
    DominantUser* instance = DominantUser::getInstance();
    if(instance->getBeRoomMaster())
    {
       qDebug()<< " anchor audio";
       emit anchorStartAudio();

    }else {
       qDebug() << " tourist audio";
        emit touristStartAudio();
    }
}

void RoomUi::on_btn_make_close_clicked()
{
    ui->btn_make_close->setStyleSheet("background-color: rgb(175,238,238)");
    ui->btn_make_open->setStyleSheet("background-color: rgb(255,255,255)");
    //关闭麦克风
    DominantUser* instance = DominantUser::getInstance();
    if(instance->getBeRoomMaster())
    {
       qDebug()<< " anchor audio end ";
       emit anchorEndAudio();

    }else{
       qDebug() << " tourist audio end";
        emit touristEndAudio();
    }
}

void RoomUi::paintEvent(QPaintEvent *)
{
    // 设置画笔
    QPen objPen(Qt::NoBrush,1);
    QPainter Painter(this);
    Painter.setPen(objPen);
    //设置渐变画刷
    QLinearGradient objLinear(rect().topLeft(),rect().bottomLeft());
    //顶部颜色和透明度
    objLinear.setColorAt(0,QColor(50,50,50,240));
    //中间颜色和透明度
    objLinear.setColorAt(0.8,QColor(34,34,34,255));
    //底部颜色和透明度
    objLinear.setColorAt(1,QColor(7,7,7,255));
    QBrush objBrush(objLinear);
    Painter.setBrush(objBrush);
    //画圆角矩形
    //Painter.drawRoundedRect(rect(),5,5);
    Painter.drawRect(rect());
}


void RoomUi::on_btn_danmu_clicked()
{
    qDebug() << "danmu";
    DominantUser* instance = DominantUser::getInstance();
    if( instance->getDanmu() )
    {
        this->ui->btn_danmu->setText("close");
        ui->btn_danmu->setStyleSheet("background-color: rgb(255,255,255)");
        //连接弹幕槽函数
        disconnect(this, SIGNAL(makeDanMu(QString)), this->ui->label, SLOT(makeDanMu(QString)));
        instance->setDanmu(false);
    }else{
        this->ui->btn_danmu->setText("open");
        ui->btn_danmu->setStyleSheet("background-color: rgb(175,238,238)");
        //连接弹幕槽函数
        connect(this, SIGNAL(makeDanMu(QString)), this->ui->label, SLOT(makeDanMu(QString)));
        instance->setDanmu(true);
    }

}

void RoomUi::setWindowStyle()
{
    DominantUser* instance = DominantUser::getInstance();
    QString title = instance->getRoomName()+"'s living room";
    setWindowTitle(title);
    this->setFixedSize(QSize(908,654));

    this->ui->label_make->setFont(QFont("SimHei",10,100));
    QPalette pe;
    QPalette pe_1;
    pe.setColor(QPalette::WindowText,Qt::green);
    pe_1.setColor(QPalette::WindowText,Qt::magenta);

    this->ui->label_make->setPalette(pe);

    this->ui->label_roommate->setFont(QFont("SimHei",10,100));
    this->ui->label_roommate->setPalette(pe);

    this->ui->lable_camera->setFont(QFont("SimHei",10,100));
    this->ui->lable_camera->setPalette(pe);

    this->ui->label_danmu->setFont(QFont("SimHei",10,100));
    this->ui->label_danmu->setPalette(pe);

    this->ui->checkBox_flower->setFont(QFont("SimHei",10,100));
    this->ui->checkBox_flower->setPalette(pe);
    this->ui->checkBox_diamond->setFont(QFont("SimHei",10,100));
    this->ui->checkBox_diamond->setPalette(pe);
    this->ui->checkBox_runcar->setFont(QFont("SimHei",10,100));
    this->ui->checkBox_runcar->setPalette(pe);

    this->ui->groupBox->setFont(QFont("FZShuTi",8,100));
    this->ui->groupBox->setPalette(pe_1);

    //初始化按钮颜色
    ui->btn_camera_close->setStyleSheet("background-color: rgb(175,238,238)");
    ui->btn_camera_open->setStyleSheet("background-color: rgb(255,255,255)");

    ui->btn_make_close->setStyleSheet("background-color: rgb(175,238,238)");
    ui->btn_make_open->setStyleSheet("background-color: rgb(255,255,255)");

    //初始化弹幕按钮
    ui->btn_danmu->setStyleSheet("background-color: rgb(175,238,238)");
    this->ui->btn_danmu->setText("open");

}


void RoomUi::sendStopVideoToViewers()
{

    DominantUser* instance = DominantUser::getInstance();
    QTcpSocket* clientSocket = instance->getTcpSocket();

    Protocol packReq(Protocol::videosharestop);
    packReq["roomId"] = instance->getRoomId();
    packReq["roomName"] = instance->getRoomName();
    qDebug() << "send stop video to viewers" << instance->getRoomId() << instance->getRoomName();
    clientSocket->write(packReq.pack());
}

void RoomUi::handleVideoStop(Protocol& pack)
{
    QTimer::singleShot(1000, this, [=](){
        ui->label->setMovie(move);
    });
}

int RoomUi::getGiftMoney()
{
    int giftMoney = 0;
    if( ui->checkBox_flower->checkState() == Qt::Checked )
    {
        giftMoney += 1;
    }

    if( ui->checkBox_diamond->checkState() == Qt::Checked )
    {

        giftMoney += 10;
    }

    if( ui->checkBox_runcar->checkState() == Qt::Checked )
    {
        giftMoney += 100;
    }
    return giftMoney;
}

bool RoomUi::getPermitToGiveGift()
{
    bool ret = false;
    //主播不能送礼
     DominantUser* instance = DominantUser::getInstance();
    if( instance->getBeRoomMaster() )
    {
        QMessageBox::information(this, "提示", "你是主播，不能送礼");
        initGiftCheckBox();
        return ret;
    }
    int giftMoney = getGiftMoney();

    if( 0 != giftMoney )
    {
        QString text = "礼物将花费您" + QString::number(giftMoney) + "元";

        QMessageBox:: StandardButton result= QMessageBox::information(this, "提示", text,QMessageBox::Yes|QMessageBox::No);

        switch (result)
        {
        case QMessageBox::Yes:
            qDebug()<<"Yes";
            ret = true;
            break;
        case QMessageBox::No:
            qDebug()<<"NO";
            break;
        default:
            break;
        }
    }
    return ret;
}

void RoomUi::toGiveGift()
{
    DominantUser* instance = DominantUser::getInstance();
    Protocol packReq;
    packReq["roomId"] = instance->getRoomId();
    packReq["roomName"] = instance->getRoomName();
    packReq["userName"] = instance->getUser().getName();

    if( ui->checkBox_flower->checkState() == Qt::Checked )
    {
        packReq.setType(Protocol::givinggift);
        qDebug() << "gift number is " << Protocol::giftType::flower;
        packReq["giftType"] = Protocol::giftType::flower;

        client_socket->write(packReq.pack());
        ui->checkBox_flower->setCheckState(Qt::Unchecked);
    }

    if( ui->checkBox_diamond->checkState() == Qt::Checked )
    {
        packReq.setType(Protocol::givinggift);
        qDebug() << "gift number is " << Protocol::giftType::diamond;
        packReq["giftType"] = Protocol::giftType::diamond;

        client_socket->write(packReq.pack());
        ui->checkBox_diamond->setCheckState(Qt::Unchecked);
    }

    if( ui->checkBox_runcar->checkState() == Qt::Checked )
    {
        packReq.setType(Protocol::givinggift);
        qDebug() << "gift number is " << Protocol::giftType::runcar;
        packReq["giftType"] = Protocol::giftType::runcar;

        client_socket->write(packReq.pack());
        ui->checkBox_runcar->setCheckState(Qt::Unchecked);
    }

    QString text = ui->lineEdit->text();
    if( !text.isEmpty() )
    {
        packReq.setType(Protocol::chat);

        packReq["chatText"] = text;

        client_socket->write(packReq.pack());
        ui->lineEdit->clear();
    }
}

void RoomUi::initGiftCheckBox()
{

    ui->checkBox_flower->setCheckState(Qt::Unchecked);
    ui->checkBox_diamond->setCheckState(Qt::Unchecked);
    ui->checkBox_runcar->setCheckState(Qt::Unchecked);

}

void RoomUi::initWiget()
{
    QTimer::singleShot(1000, this, [=](){
        ui->label->setMovie(move);
    });
    this->ui->list_roommate->clear();
    this->ui->textEdit->clear();

    //初始化按钮颜色
    ui->btn_camera_close->setStyleSheet("background-color: rgb(175,238,238)");
    ui->btn_camera_open->setStyleSheet("background-color: rgb(255,255,255)");

    ui->btn_make_close->setStyleSheet("background-color: rgb(175,238,238)");
    ui->btn_make_open->setStyleSheet("background-color: rgb(255,255,255)");

    //初始化弹幕按钮
    ui->btn_danmu->setStyleSheet("background-color: rgb(175,238,238)");
    this->ui->btn_danmu->setText("open");
}
