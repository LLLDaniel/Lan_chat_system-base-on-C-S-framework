#include "VideoLabel.h"

VideoLabel::VideoLabel(QWidget *parent) : QLabel(parent)
{

}

void VideoLabel::makeDanMu(QString text)
{
    qDebug() << "弹幕制造" << text;
   //获取桌面设备
    QRect screenRect = this->geometry();

    Danmu* danmu3 = new Danmu(this,text,"Red",1,screenRect);

    danmu3->show();

    QPropertyAnimation *animation = new QPropertyAnimation(danmu3, "pos");

    animation->setDuration(5000);

    animation->setStartValue(QPoint(800,danmu3->getPosY()));
    animation->setEndValue(QPoint(screenRect.left()-this->width()-100,danmu3->getPosY()));

    animation->start();
    connect(animation,SIGNAL(finished()),danmu3,SLOT(deleteLater()));

}

void VideoLabel::getGeo()
{
    QRect screenRect = this->geometry();
    qDebug() << screenRect << "pppp";
}
