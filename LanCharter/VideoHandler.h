#ifndef VIDEOHANDLER_H
#define VIDEOHANDLER_H

#include <QObject>
#include <QDebug>
#include <QCamera>
#include <QCameraInfo>
#include <QPixmap>
#include <QUdpSocket>
#include <QBuffer>
#include <QImageReader>
#include <QScreen>
#include <QTimer>
#include <QApplication>
#include <QDesktopWidget>
#include "VideoSurface.h"
#include "protocol.h"
#include "package.h"

class VideoHandler : public QObject
{
    Q_OBJECT
public:
    explicit VideoHandler(QObject *parent = nullptr);
    explicit VideoHandler( const QHostAddress& videoGroupAddress ,
                                          int groupPort, QObject *parent = nullptr);

signals:
    void sendVideoToUi(QPixmap pixmap);

public slots:
    void anchorStartVideo();
    void anchorEndVideo();

    void touristStartVideo();
    void touristEndVideo();

    void frameChangedSlot(QVideoFrame frame);
    void readReadSlot();

    void shareWindowScreen();
private:
    VideoSurface* vs;
    QCamera* camera;
    QUdpSocket* sender;
    QUdpSocket* receiver;
    QHostAddress groupAddress;
    int groupPort;
    bool onVideo;
    QTimer* m_Timer;


};

#endif // VIDEOHANDLER_H
