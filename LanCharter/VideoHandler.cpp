#include "VideoHandler.h"

VideoHandler::VideoHandler(QObject *parent) : QObject(parent)
{

}

VideoHandler::VideoHandler(const QHostAddress& videoGroupAddress, int groupPort, QObject *parent) : QObject(parent)
{
    qDebug() << "video handler start1";
    sender = new QUdpSocket(this);
    receiver = nullptr;
    this->camera = nullptr;
    this->m_Timer = nullptr;
    qDebug() << "video handler start2";
    groupAddress.setAddress(videoGroupAddress.toString());
    this->groupPort = groupPort;

    onVideo = false;
}


void VideoHandler::anchorStartVideo()
{
    if( onVideo )
    {
        return;
    }else{
        onVideo = true;
    }
    //准备摄像机
    //获取计算机的默认摄像头
    QCameraInfo info = QCameraInfo::defaultCamera();
//    QCameraInfo info;
    //如果没有获取到摄像头信息，说明没有摄像头，此时获取桌面
    if( !info.isNull() )
    {
        //如果分享桌面的计时器开着，则关闭
        if( this->m_Timer != nullptr )
        {
            this->m_Timer->stop();
        }

        qDebug() << "camera is not null";
        this->camera = new QCamera(info, this);
        vs = new VideoSurface(this);

        //设置将多媒体文件显示到QVideoWidget窗口上
        this->camera->setViewfinder(vs);
        qDebug() << "video handler start3";
        //对没一帧图片的处理
        connect(vs, SIGNAL(frameChanged(QVideoFrame)),
                this, SLOT(frameChangedSlot(QVideoFrame)));
        qDebug() << this->camera->isAvailable() << "ee";
        this->camera->start();
    }else{
        qDebug() << "camera is null";
        //获取桌面图像并且发送，利用定时器实现
        this->m_Timer = new QTimer();
        connect(m_Timer, SIGNAL(timeout()), this, SLOT(shareWindowScreen()));
        this->m_Timer->start(10);
    }
}

void VideoHandler::anchorEndVideo()
{
    if( !onVideo )
    {
        return;
    }else{
        onVideo = false;
    }
    //如果有摄像头，关闭它，否则关闭桌面分享
    if( this->camera != nullptr )
    {
        this->camera->stop();
    }else{
        this->m_Timer->stop();
    }

}

void VideoHandler::touristStartVideo()
{
    if( onVideo )
    {
        return;
    }else{
        onVideo = true;
    }
    qDebug() << "tourist video totototo";
    qDebug() << this->groupPort;
    qDebug() << this->groupAddress.toString();
    if( receiver != nullptr )
        return ;
    receiver = new QUdpSocket(this);
    //读端绑定指定端口
    receiver->bind(QHostAddress::AnyIPv4, (quint16)this->groupPort,
                        QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    receiver->joinMulticastGroup(groupAddress);
    qDebug() << this->receiver->isValid();
    connect(receiver, SIGNAL(readyRead()),
                    this, SLOT(readReadSlot()));
}

void VideoHandler::touristEndVideo()
{
    if( !onVideo )
    {
        return;
    }else{
        onVideo = false;
    }
    if( receiver == nullptr )
        return;
    disconnect(receiver, SIGNAL(readyRead()),
                    this, SLOT(readReadSlot()));
    delete receiver;
    receiver = nullptr;
}

void VideoHandler::frameChangedSlot(QVideoFrame frame)
{
    //QImage----->表示图片【网络间传输】
    //QVideoFrame--->QImage
    //映射图片的数据到虚拟内存中
    int unitBytes  = 4096;
    static int i = 0;
    i++;
    frame.map(QAbstractVideoBuffer::ReadOnly);

    /*
     * QImage(
     * const uchar *data,           //图片数据的存储地址
     * int width,                   //图片的宽度
     * int height,                  //图片的高度
     * int bytesPerLine,            //每行的字节数
     * Format format,               //图片格式
     * );
    */
    QImage img(
        frame.bits(),           //字节地址
        frame.width(),          //宽度
        frame.height(),         //高度
        frame.bytesPerLine(),   //行中字节数
        QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat())
        );
    //QImage--->QPixmap
    QPixmap pixmap = QPixmap::fromImage(img);

    //旋转图片180度
    QMatrix matrix;
    matrix.rotate(180);

    pixmap = pixmap.transformed(matrix);
    //发送图片给本地
    emit sendVideoToUi(pixmap);
    //向服务器发送图片信息---由于是局域网，直接往组播地址发就行了

//    if( i %4 != 0)
//        return;
    //将图片信息以广播组的形式发送出去
    QByteArray byte;
    QBuffer buffer(&byte);           //缓存
    img.save(&buffer, "JPEG");

//    qDebug() << byte.size();
    //将图片信息写入到制定端口
    this->sender->writeDatagram(byte,
          this->groupAddress, (quint16)this->groupPort);
//    qDebug() << "write....";

}

void VideoHandler::readReadSlot()
{
    qDebug() << "接收upd视频数据";

    qint64 size = this->receiver->pendingDatagramSize();
    //直接读取异步监听时接收到的数据报
    QByteArray buff;
    // 字节矩阵，大小用qint64描述，适合操作1大量数据2字符串数组3二进制01数据，节省内存，更加安全
    //一般用const char*初始化
    // 默认采用深拷贝，可指定为浅拷贝
    buff.resize((int)size);
    //为缓冲区规定一个maxsize（没必要用那么大，刚好装下图像就够了）
    this->receiver->readDatagram(buff.data(),buff.size());
    // 数据报只支持一维操作
    QBuffer buffer(&buff);

    //提供用Qbytearray读写内存中IO缓存区的接口，若无传参内部默认自动创建一个Qbytearray；

    //对IO缓存区读写操作等效于像IO设备读写数据

    //操作QBuffer像在操作文件（其实原理都差不多，各自都是内存中一块特殊区域嘛）
    QImageReader reader(&buffer,"JPEG");
    //可读入磁盘文件、设备文件中的图像、以及其他图像数据如pixmap和image，相比较更加专业。
    //buffer属于设备文件一类，
    //专业地读取设备文件的图像数据。
    QImage image = reader.read();
    //read()方法用来读取设备图像，也可读取视频，读取成功返回QImage*，否则返回NULL
    //格式转换
    QPixmap pix = QPixmap::fromImage(image);
    //旋转图片180度
    QMatrix matrix;
    matrix.rotate(180);

    pix = pix.transformed(matrix);
    emit sendVideoToUi(pix);
}

void VideoHandler::shareWindowScreen()
{
    //1. 获取桌面图像,利用定时器实现不断传输
    QScreen *screen = QApplication::primaryScreen(); //获取当前程序的主窗口对象
    QPixmap pixmap = screen->grabWindow(QApplication::desktop()->winId()); //调用主窗口对象的捕捉窗口图像，并传递桌面窗口的id号

    emit sendVideoToUi(pixmap);
    //向服务器发送图片信息---由于是局域网，直接往组播地址发就行了

    QImage img = pixmap.toImage();
    /*
     * QImage(
     * const uchar *data,           //图片数据的存储地址
     * int width,                   //图片的宽度
     * int height,                  //图片的高度
     * int bytesPerLine,            //每行的字节数
     * Format format,               //图片格式
     * );
    */

    //将图片信息以广播组的形式发送出去
    QByteArray byte;
    QBuffer buffer(&byte);           //缓存
    img.save(&buffer, "JPEG",10);

    qDebug() << byte.size();
    //将图片信息写入到制定端口
    this->sender->writeDatagram(byte,
          this->groupAddress, (quint16)this->groupPort);
    qDebug() << "write...." << this->groupPort << groupAddress.toString();

}


