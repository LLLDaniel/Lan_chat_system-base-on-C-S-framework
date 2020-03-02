#include "AudioHandler.h"

AudioHandler::AudioHandler(QObject *parent) : QObject(parent)
{

}

AudioHandler::AudioHandler(const QHostAddress& audioGroupAddress, int groupPort, QObject *parent) : QObject(parent)
{
    socketAudioRead = nullptr;
    socketAudioWrite = new QUdpSocket(this);
    audioInput = nullptr;
    audioOutput = nullptr;
    audioInputIODevice = nullptr;
    audioOutputIODevice = nullptr;

    //设置音频格式
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    groupAddress.setAddress(audioGroupAddress.toString());

    onAudio = false;
    this->groupPort = groupPort;
}

//AudioHandler::~AudioHandler()
//{
//    if( audioInput != nullptr )
//    {
//        delete audioInput;
//    }
//    if( audioOutput != nullptr )
//    {
//        delete audioOutput;
//    }
//    if( socketAudioRead != nullptr )
//    {
//        delete socketAudioRead;
//    }
//    if( socketAudioWrite != nullptr )
//    {
//        delete socketAudioWrite;
//    }
//    if( audioInputIODevice != nullptr )
//    {
//        delete audioInputIODevice;
//    }
//    if( audioOutputIODevice != nullptr )
//    {
//        delete audioOutputIODevice;
//    }
//}

void AudioHandler::captureDataFromDevice()
{
    qDebug() << "hehe 3";
    AudioPackage pack;
    memset(&pack, 0, sizeof(AudioPackage));
    qDebug() << "hehe 4";
    pack.dataLen = audioInputIODevice->read(pack.data, 1024);
    qDebug() << "hehe 5";
    socketAudioWrite->writeDatagram((char*)&pack,
                                    sizeof(AudioPackage),
                                    this->groupAddress,
                                    (quint16)this->groupPort);
    qDebug() << "hehe 6";
//    audioOutputIODevice->write(pack.data, pack.dataLen);
}

void AudioHandler::onReadyReadAudio()
{
    qDebug()<<"tourist start audio 3";
    AudioPackage pack;
    memset(&pack, 0 , sizeof(AudioPackage));
    socketAudioRead->readDatagram((char*)&pack, sizeof(AudioPackage));
     qDebug()<<"tourist start audio 4";
    audioOutputIODevice->write(pack.data, pack.dataLen);
     qDebug()<<"tourist start audio 5";
}

void AudioHandler::anchorStartAudio()
{
    if( onAudio )
    {
        return;
    }else{
        onAudio = true;
    }

    qDebug() << "hehe 1";

    audioInput = new QAudioInput(format);
    audioOutput = new QAudioOutput(format);

    qDebug() << "hehe 2";
    audioInputIODevice = audioInput->start();

    audioOutputIODevice = audioOutput->start();
    connect(audioInputIODevice, SIGNAL(readyRead()),
                this, SLOT(captureDataFromDevice()));
}

void AudioHandler::anchorEndAudio()
{
    qDebug() << "audio stop";

    if( !onAudio )
    {
        return;
    }else{
        onAudio = false;
    }
    audioInput->stop();
}

void AudioHandler::touristStartAudio()
{
    qDebug()<<"tourist start audio 1";
    if( socketAudioRead != nullptr )
        return;

    socketAudioRead = new QUdpSocket(this);
    socketAudioRead->bind(QHostAddress::AnyIPv4, (quint16)this->groupPort,
                          QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    socketAudioRead->joinMulticastGroup(groupAddress);
    connect(socketAudioRead, SIGNAL(readyRead()),
            this, SLOT(onReadyReadAudio()));

    audioOutput = new QAudioOutput(format);
    audioOutputIODevice = audioOutput->start();
    qDebug()<<"tourist start audio 2";
}

void AudioHandler::touristEndAudio()
{
    if( socketAudioRead == nullptr )
        return;
    disconnect(socketAudioRead, SIGNAL(readyRead()),
            this, SLOT(onReadyReadAudio()));
    delete socketAudioRead;
    socketAudioRead = nullptr;
}
