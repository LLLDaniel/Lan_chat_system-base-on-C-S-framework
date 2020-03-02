#ifndef USER_H
#define USER_H
#include <QString>


class User
{

public:
    enum indentity{
        TOURIST,  //用户
        ANCHOR,  //主播
    };
    User();
    User(const QString& name, const QString& password,
         float money = 0, int integral = 0, bool online = false);

    const QString& getName() const { return this->name; }
    void setName(const QString name) { this->name = name; }

    const QString& getPassword() const { return  this->password; }
    void setPassword(const QString password) { this->password = password;}

    float getMoney() const { return this->money; }
    void setMoney(float money) { this->money = money; }

    int getIntegral() const { return this->integral; }
    void setIntegral( int integral) { this->integral = integral; }

    bool getOnline() const { return this->online; }
    void setOnline(bool online) { this->online = online; }

    indentity getIndentity() { return this->status; }
    void setIndentity( indentity type ) { this->status =type; }
private:
    QString name;
    QString password;
    float money;
    int integral;
    bool online;

    indentity status;
};

#endif // USER_H
