#ifndef ROOMUI_H
#define ROOMUI_H

#include <QWidget>
#include <QCamera>
#include <QThread>
#include "DominantUser.h"
#include "protocol.h"
#include <QMessageBox>
#include "protocol.h"
#include "VideoSurface.h"
#include "VideoHandler.h"
#include "AudioHandler.h"
#include <QMovie>
#include <QTimer>
#include <QCloseEvent>
#include <QPaintEvent>
#include <QPainter>
#include "Danmu.h"

namespace Ui {
class RoomUi;
}

class RoomUi : public QWidget
{
    Q_OBJECT

public:
    explicit RoomUi(QWidget *parent = nullptr);
    ~RoomUi();

private slots:
    void on_btn_flash_clicked();
    void readReadySlot();
    void on_btn_send_clicked();

    void on_btn_camera_open_clicked();
    void frameChangedSlot(QPixmap pixmap);

    void on_btn_camera_close_clicked();

    void on_btn_make_open_clicked();

    void on_btn_make_close_clicked();

    void on_btn_danmu_clicked();

    void initWiget();

signals:
    void RoomUiClose();
    void anchorStartVideo();
    void touristStartVideo();

    void anchorEndVideo();
    void touristEndVideo();

    void anchorStartAudio();
    void touristStartAudio();

    void anchorEndAudio();
    void touristEndAudio();

    void makeDanMu(QString text);

private:
    Ui::RoomUi *ui;
    QTcpSocket* client_socket;
    VideoHandler* videohandler;
    AudioHandler* audiohandler;
    QThread* videoThread;
    QThread* audioThread;
    QMovie* move;

private:
    void getRoomMateInfos(Protocol& packRet);
    void getChatText(Protocol& pack);
    void getRoomMateExitMessage(Protocol& pack);
    void getRoomMateEnterMessage(Protocol& pack);
    void handleAnyoneGiveGiftToAnchor(Protocol& pack);
    void getRoomOverDueMessage(Protocol& pack);
    void setWindowStyle();

    void sendStopVideoToViewers();
    void handleVideoStop(Protocol& pack);

    int getGiftMoney();
    bool getPermitToGiveGift();
    void toGiveGift();

    void initGiftCheckBox();

protected:
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *);

};

#endif // ROOMUI_H
