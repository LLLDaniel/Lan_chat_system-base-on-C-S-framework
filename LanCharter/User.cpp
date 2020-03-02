#include "User.h"

User::User()
{

}

User::User(const QString &name, const QString &password,
           float money, int integral, bool online)
        :name(name), password(password), money(money),
          integral(integral),online(online)
{

}
