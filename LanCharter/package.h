#ifndef PACKAGE_H
#define PACKAGE_H

#define SIZE 20


//包头
enum dateType{
    TYPE_REGIST,
    TYPE_REGIST_FAILED,
    TYPE_REGIST_SUCCESS,

    TYPE_LGOIN
};

//自定义协议包
typedef struct pack{
    dateType     type;
    char        name[SIZE];
    char        pwd[SIZE];
    //...
}pack_t;

//视频协议包
typedef struct {
    int width;
    int height;
    int bytesPerline;
    bool isLastPack;
    int packTaken;
    char data[4096];
}ImagePackage;

//音频协议包
struct AudioPackage
{
    char data[1024];
    int dataLen;
};

#endif // PACKAGE_H
