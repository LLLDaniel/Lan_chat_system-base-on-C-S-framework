#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

#if 0
    QJsonArray          json数组
    QJsonObject         对象
    QJsonDocument       文档

#endif
class Protocol{
public:
    enum dateType{
        none,               //空   0
        regist,             //注册
        login,              //登录
        chat,                //聊天
        video,              //视频
        audio,              //音频

        registfailed,             //注册
        registsucceed,             //注册失败
        loginfailed,              //登录
        loginsucceed,              //登录成功

        creatliveroom,      //创建房间
        destroyliveroom,    //销毁房间
        exitliveroom,       //退出房间
        enterliveroom,      //进入房间

        creatliveroomsucceed,      //创建房间成功
        creatliveroomfailed,      //创建房间失败
        destroyliveroomsucceed,      //创建房间成功
        destroyliveroomfailed,      //创建房间失败
        enterliveroomsucceed,       //进入房间成功
        enterliveroomfailed,        //进入房间失败
        exitliveroomsucceed,       //离开房间成功
        exitliveroomfailed,        //离开房间失败

        getRoomInfos,       //获取房间信息
        getRoomMateInfos,   //获取房间在线用户信息

        logout,     //主动退出
        anyoneexitliveroom, //任何人离开房间
        anyoneenterliveroom, //任何人进入房间

        getpersoninfo,
        getpersoninfofailed,
        getpersoninfosucceed,

        modifypassword, //修改密码
        modifypasswordfaild, //修改失败
        modifypasswordsucceed, //修改成功

        roomoverdue, //主播退出房间房间过期
        videosharestop, //主播停止发送直播视频

        chargemoney, //充值金钱
        chargemoneyfailed,
        chargemoneysucceed,

        givinggift, //送礼物
        givinggiftfailed,
        givinggiftsucceed,

        anyonegivegifttoanchor, //有人送礼
    };

    enum giftType{
        flower,
        diamond,
        runcar,
    };

    Protocol(dateType type = Protocol::none) :type(type){ }
    void setType(dateType type = Protocol::none) { this->type = type; }
    dateType getType() const { return this->type; }

    //提供对键值对的访问
    QJsonValue operator[](const QString &key) const
                       { return this->body[key]; }
    QJsonValueRef operator[](const QString &key)
                       { return this->body[key]; }


    //封包
    QByteArray pack()
    {
        QByteArray head(8, 0);          //包体大小  + 包头

        //将QJsonObject对象转化为QByteArray
        QByteArray qBody  = QJsonDocument(this->body).toJson();

        *((int*)head.data()) = qBody.size();   //json字符串的大小
        *(dateType*)(head.data() + 4) = type;      //数据包的头


        return head + qBody;
    }

    //解包
    int unpack(QByteArray byte)
    {
        if(byte.size() < 8)     return 0;

        int len  = *((int*)byte.data());


        this->type = *(dateType*)(byte.data() + 4);      //包头

        //解析包体
        QByteArray byteBody = byte.mid(8, len);

        if(byteBody.size() != len) return 0;

        this->body = QJsonDocument::fromJson(byteBody).object();
        return 8 + len;
    }

private:
    dateType    type;          //包头
    QJsonObject body;          //包体         键值对
};


#endif // PROTOCOL_H
