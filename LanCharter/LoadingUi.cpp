#include "LoadingUi.h"
#include "ui_LoadingUi.h"

LoadingUi::LoadingUi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoadingUi)
{
    ui->setupUi(this);
    qDebug() << "show loading ";
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);

    QMovie* move = new QMovie(":/image/loading.gif");
    ui->label->setMovie(move);
    move->start();
}

LoadingUi::~LoadingUi()
{
    delete ui;
}
