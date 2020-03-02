#include "Login.h"
#include "ui_Login.h"
#include "package.h"
#include <QDebug>
#include <QThread>

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    this->idChoose = nullptr;

    this->client_socket = new QTcpSocket(this);

    //客户端连接上服务器状态的信息处理
    connect(this->client_socket, SIGNAL(connected()), this, SLOT(connectToHostSuccessSlot()));
    connect(this->client_socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(connectErrorSlot(QAbstractSocket::SocketError)));

    this->client_socket->connectToHost(QHostAddress("127.0.0.1"), 8888);
    connect(this->client_socket, SIGNAL(readyRead()), this, SLOT(readReadySlot()));

    this->loadingUI  = nullptr;

    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(slotTimeOut()));

    //设置样式

    initUi();

}

Login::~Login()
{
    delete ui;
    if( idChoose != nullptr)
    {
        delete idChoose;
    }
}

void Login::on_btn_login_clicked()
{
    QString user_name = ui->user_name->text();
    QString user_password = ui->user_password->text();
    if( user_name.isEmpty() || user_password.isEmpty())
    {
        QMessageBox::warning(this, "failed", "please input you name and password");
        return;
    }

    Protocol packReq;

    //将数据打包
    packReq.setType(Protocol::login);

    packReq["userName"] = user_name;
    packReq["userPassword"] = user_password;

    //填写本地的用户
    user.setName(user_name);
    user.setPassword(user_password);

    //发送
    this->client_socket->write(packReq.pack());
    qDebug() << user_name << " " << user_password;
}

void Login::on_btn_register_clicked()
{
    QString user_name = ui->user_name->text();
    QString user_password = ui->user_password->text();
    if( user_name.isEmpty() || user_password.isEmpty())
    {
        QMessageBox::warning(this, "failed", "please input you name and password");
        return;
    }

    Protocol packReq;

    //将数据打包
    packReq.setType(Protocol::regist);

    packReq["userName"] = user_name;
    packReq["userPassword"] = user_password;
    //发送
    this->client_socket->write(packReq.pack());
    qDebug() << user_name << " " << user_password;
}

void Login::readReadySlot()
{

    QByteArray ret = this->client_socket->readAll();

    //解包
    Protocol packRet;
    int len = 0;
    while ( (len = packRet.unpack(ret)) > 0) {
        switch (packRet.getType()) {
        case Protocol::loginsucceed:
        {
                creatIdChooseUi();
                this->hide();
                QTimer::singleShot(1000, this, [=](){
                    this->loadingUI->hide();
                    this->idChoose->show();
                    disconnect(this->client_socket, SIGNAL(readyRead()), this, SLOT(readReadySlot()));
                });
        }
            break;
        case Protocol::loginfailed:
            QMessageBox::information(this, "提示", packRet["errText"].toString());
            break;
        case Protocol::registfailed:
            QMessageBox::information(this, "提示", packRet["errText"].toString());
            break;
        case Protocol::registsucceed:
            QMessageBox::information(this, "提示", packRet["errText"].toString());
            break;
        case Protocol::chat:
            QMessageBox::information(this, "提示", packRet["errText"].toString());
            break;
        default:
            break;
        }
        ret = ret.mid(len);
    }
}

void Login::creatIdChooseUi()
{
    if( idChoose == nullptr )
    {
        idChoose = new IdChooseUi(this);
    }
    //设置加载界面
    loadingUI = new LoadingUi();
    loadingUI->show();

    //改变全局用户
    DominantUser* instance = DominantUser::getInstance();
    instance->setUser(this->user);
    instance->setTcpSocket(this->client_socket);
}

void Login::connectToHostSuccessSlot()
{
    qDebug() << "connect success ==";
    disconnect(&m_Timer, SIGNAL(timeout()), this, SLOT(slotTimeOut()));
    QMessageBox::information(this, "提示","连接服务器成功...");
     this->m_Timer.stop();
//    //暂停连接线程
//    this->ConThread->terminate();
//    this->ConThread->wait();
    //改变界面颜色

}

void Login::connectErrorSlot(QAbstractSocket::SocketError type)
{
    if( type == QAbstractSocket::ConnectionRefusedError || QAbstractSocket::HostNotFoundError)
    {
       qDebug() << "not connect ==";
       QMessageBox::information(this, "提示","未连接服务器....");
       this->m_Timer.start(1000);
    }
}


//使用定时器来重连服务器
void Login::slotTimeOut()
{
    qDebug() << "连接服务器中...";

    client_socket->abort();
    this->client_socket->connectToHost(QHostAddress("127.0.0.1"), 8888);
}


//初始化界面
void Login::initUi()
{
    this->setFixedSize(QSize(554,738));
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    //设置窗体透明
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    //实例阴影shadow
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
     QGraphicsDropShadowEffect *shadow_a = new QGraphicsDropShadowEffect(this);
     //设置阴影距离
     shadow_a->setOffset(5, 5);
     //设置阴影颜色
     shadow_a->setColor(QColor("#444444"));
     //设置阴影圆角
     shadow_a->setBlurRadius(80);

    //设置阴影距离
    shadow->setOffset(5, 5);
    //设置阴影颜色
    shadow->setColor(QColor("#444444"));
    //设置阴影圆角
    shadow->setBlurRadius(80);
    //给嵌套QWidget设置阴影
    ui->widget_bg->setGraphicsEffect(shadow);
    ui->widget_bg->setStyleSheet("border-radius:8px;border-image: url(:/image/bg.jpg);");
//    ui->name_icon->setStyleSheet("border-image: url(:/name_pic.png);background:transparent;");
//    ui->passwd_icon->setStyleSheet("border-image: url(:/passwd_icon.png);background:transparent;");

    ui->toolButton_min->setStyleSheet("background:transparent;color:rgba(0,255,0);");
    ui->toolButton_close->setStyleSheet("background:transparent;color:rgba(0,255,0);");

    ui->user_name->setStyleSheet("border:1px groove gray;border-radius:8px;padding:2px 4px;lineedit-password-character:25FE;border-left-color: rgb(255, 255, 255);\
                                 border-right-color: rgb(255, 255, 255);\
                                 border-top-color: rgb(255, 255, 255);font: 75 12pt");
//    ui->user_name->setFont(QFont( "Timers" , 16 ,  QFont::Bold) );
    ui->user_password->setStyleSheet("border:1px groove gray;border-radius:8px;padding:2px 4px;border-left-color: rgb(255, 255, 255);\
                               border-right-color: rgb(255, 255, 255);\
                               border-top-color: rgb(255, 255, 255);font: 12px italic 5pt 'Book Antiqua';");
//    ui->user_password->setFont(QFont( "Timers" , 16 ,  QFont::Bold) );

    ui->btn_login->setStyleSheet("QPushButton{border:0px groove gray;border-radius:5px;padding:2px 4px;background-color: rgb(0, 170, 255);}"

                                 "QPushButton:pressed{background-color:rgba(85, 170, 255,200); border-style: inset; }"

                                 "QPushButton{"
                                 "background-color:rgba(100,225,100,30);"//背景色（也可以设置图片）
                                 "border-style:outset;"                  //边框样式（inset/outset）
                                 "border-width:4px;"                     //边框宽度像素
                                 "border-radius:10px;"                   //边框圆角半径像素
                                 "border-color:rgba(255,255,255,30);"    //边框颜色
                                 "font:bold 10px;"                       //字体，字体大小
                                 "color:rgba(0,0,0,100);"                //字体颜色
                                 "padding:6px;"                          //填衬
                                 "}"
                                 //鼠标按下样式
                                 "QPushButton:pressed{"
                                 "background-color:rgba(100,255,100,200);"
                                 "border-color:rgba(255,255,255,30);"
                                 "border-style:inset;"
                                 "color:rgba(0,0,0,100);"
                                 "border-style: inset;}"
                                 //鼠标悬停样式
                                 "QPushButton:hover{"
                                 "background-color:rgba(100,255,100,100);"
                                 "border-color:rgba(255,255,255,200);"
                                 "color:rgba(0,0,0,200);"
                                 "}");
    ui->btn_register->setStyleSheet("QPushButton{border:0px groove gray;border-radius:5px;padding:2px 4px;background-color: rgb(0, 170, 255);}"

                                 "QPushButton:pressed{background-color:rgba(85, 170, 255,200); border-style: inset; }"

                                 "QPushButton{"
                                 "background-color:rgba(100,225,100,30);"//背景色（也可以设置图片）
                                 "border-style:outset;"                  //边框样式（inset/outset）
                                 "border-width:4px;"                     //边框宽度像素
                                 "border-radius:10px;"                   //边框圆角半径像素
                                 "border-color:rgba(255,255,255,30);"    //边框颜色
                                 "font:bold 10px;"                       //字体，字体大小
                                 "color:rgba(0,0,0,100);"                //字体颜色
                                 "padding:6px;"                          //填衬
                                 "}"
                                 //鼠标按下样式
                                 "QPushButton:pressed{"
                                 "background-color:rgba(100,255,100,200);"
                                 "border-color:rgba(255,255,255,30);"
                                 "border-style:inset;"
                                 "color:rgba(0,0,0,100);"
                                 "border-style: inset;}"
                                 //鼠标悬停样式
                                 "QPushButton:hover{"
                                 "background-color:rgba(100,255,100,100);"
                                 "border-color:rgba(255,255,255,200);"
                                 "color:rgba(0,0,0,200);"
                                 "}");
     ui->btn_login->setGraphicsEffect(shadow_a);
     ui->btn_register->setGraphicsEffect(shadow_a);
    QPalette   pal;
    pal.setColor(QPalette::ButtonText, QColor(255,255,255));

    ui->btn_login->setPalette(pal);
    ui->btn_register->setPalette(pal);

    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::cyan);
    QPalette pe_1;
    pe_1.setColor(QPalette::WindowText,Qt::magenta);
    QPalette pe_2;
    pe_2.setColor(QPalette::WindowText,Qt::lightGray);

    ui->label_pw->setFont(QFont("SimHei",10,100));
    ui->label_un->setFont(QFont("SimHei",10,100));
    ui->label_pw->setPalette(pe);
    ui->label_un->setPalette(pe);

    ui->label->setFont(QFont("SimHei",12,100));
    ui->label_2->setFont(QFont("SimHei",20,100));
    ui->label_3->setFont(QFont("SimHei",16,100));
    ui->label->setPalette(pe);
    ui->label_2->setPalette(pe_1);
    ui->label_3->setPalette(pe_2);


}




void Login::on_toolButton_min_clicked()
{
    this->showMinimized();
}

void Login::on_toolButton_close_clicked()
{
    this->close();
}

void Login::mousePressEvent(QMouseEvent *e)
{
    last=e->globalPos();
}

void Login::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
        int dy = e->globalY() - last.y();
        last = e->globalPos();
        move(x()+dx, y()+dy);
}


void Login::mouseReleaseEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
}

