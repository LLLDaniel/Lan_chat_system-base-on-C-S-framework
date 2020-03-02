#ifndef USERUI_H
#define USERUI_H

#include <QWidget>
#include "User.h"
#include "UserUi.h"
#include "RoomUi.h"
#include "DominantUser.h"
#include <QTcpSocket>
#include "protocol.h"
#include <QDebug>
#include <QMessageBox>
#include <QObject>
#include <QListWidgetItem>
#include <QCloseEvent>
#include <QTimer>
#include "UserInfoDialog.h"
#include "ModifyPasswordDialog.h"
#include "UserChargeMoneyDialog.h"

namespace Ui {
class UserUi;
}

class UserUi : public QWidget
{
    Q_OBJECT

public:
    explicit UserUi(QWidget *parent = nullptr);
    explicit UserUi(const User& user, QWidget *parent = nullptr);
    ~UserUi();

private slots:
    void on_btn_exit_clicked();
    void on_btn_makeroom_clicked();
    void on_btn_flashroom_clicked();

    void doubleClickSlot(QListWidgetItem*);

    void readReadySlot();

    void slotTimeOut();


    void on_btn_user_info_clicked();

    void on_btn_modify_password_clicked();

    void on_btn_charge_clicked();

signals:
    void userUiExit();  //用户退出当前界面

private:
    Ui::UserUi *ui;
    RoomUi* roomUi;
    UserInfoDialog* userinfodialog;
    ModifyPasswordDialog* modifypassworddialog;
    UserChargeMoneyDialog* userchargemoneydialoy;
    User user;
    QTcpSocket* client_socket;
    QTimer m_timer;

private:
    void creatliveroomsucceed(int roomId ,QString roomName,
                                          int groupPort, QString audioIp, QString videoIp);
    void enterliveroomsucceed(int roomId , QString roomName,
                                          int groupPort, QString audioIp, QString videoIp);
    void exitliveroomsucceed();

    void destroyliveroomsucceed();
    void  getRoomInfos(Protocol& packRet);
    void  getUserPersonInfos(Protocol& packRet);
    void  modifyUserPassword(Protocol& packRet);

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // USERUI_H





