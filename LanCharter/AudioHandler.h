#ifndef AUDIOHANDLER_H
#define AUDIOHANDLER_H

#include <QObject>
#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QUdpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QIODevice>
#include "package.h"

class AudioHandler : public QObject
{
    Q_OBJECT
public:
    explicit AudioHandler(QObject *parent = nullptr);
    explicit AudioHandler(const QHostAddress& audioGroupAddress,int groupPort,QObject *parent = nullptr);
//    ~AudioHandler();

signals:

public slots:
    void captureDataFromDevice();
    void onReadyReadAudio();

    void anchorStartAudio();
    void anchorEndAudio();

    void touristStartAudio();
    void touristEndAudio();

private:
    QAudioInput* audioInput;
    QAudioOutput* audioOutput;
    QUdpSocket* socketAudioRead;
    QUdpSocket* socketAudioWrite;
    QIODevice* audioInputIODevice;
    QIODevice* audioOutputIODevice;
    QHostAddress groupAddress;
    QAudioFormat format;
    bool onAudio;
    int groupPort;
};

#endif // AUDIOHANDLER_H
