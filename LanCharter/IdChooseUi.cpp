#include "IdChooseUi.h"
#include "ui_IdChooseUi.h"

IdChooseUi::IdChooseUi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IdChooseUi)
{
    ui->setupUi(this);
    userUi = nullptr;
    setWindowTitle("please choose your identity");
    this->setFixedSize(QSize(369,376));
}

IdChooseUi::~IdChooseUi()
{
    delete ui;
    delete userUi;
}

void IdChooseUi::on_btn_anchor_clicked()
{
    if( userUi == nullptr )
    {
        userUi = new UserUi();
        connect(userUi, SIGNAL(userUiExit()), this, SLOT(show()));
    }


    setUserId(User::ANCHOR);
    this->hide();
    userUi->show();
}

void IdChooseUi::on_btn_tourist_clicked()
{
    if( userUi == nullptr )
    {
        userUi = new UserUi();
        connect(userUi, SIGNAL(userUiExit()), this, SLOT(show()));
    }


    setUserId(User::TOURIST);
    this->hide();
    userUi->show();
}

void IdChooseUi::setUserId(User::indentity type)
{
    DominantUser* instance = DominantUser::getInstance();
    instance->getUser().setOnline(true);
    instance->getUser().setIndentity(type);
}

//重写关闭事件，使其关闭的时候向服务端发送登出请求
void IdChooseUi::closeEvent(QCloseEvent *event)
{
    qDebug() << "begin ogout";
    if( QMessageBox::question(this, "提示", "are you sure exit?") == QMessageBox::Yes )
    {
        DominantUser* instance = DominantUser::getInstance();

        qDebug() << "begin ogout  1";
        //在最后发送一个下线请求
        Protocol packReq;
        packReq.setType(Protocol::logout);

        QString userName = instance->getUser().getName();
        QString userPassword = instance->getUser().getPassword();
        qDebug() << userName;
        qDebug() << userPassword;
        packReq["userName"] = userName;
        packReq["userPassword"] = userPassword;

        instance->getTcpSocket()->write(packReq.pack());
    }else{
        qDebug() << "begin ogout  2";
        event->ignore();
        return;
    }
}
