#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHostAddress>
#include <QDebug>
#include <QMessageBox>
#include "protocol.h"
#include "IdChooseUi.h"
#include "User.h"
#include "DominantUser.h"
#include "LoadingUi.h"
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>


namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_btn_login_clicked();

    void on_btn_register_clicked();
    void readReadySlot();
    void connectToHostSuccessSlot();
    void connectErrorSlot(QAbstractSocket::SocketError);

    void slotTimeOut();

    void on_toolButton_min_clicked();

    void on_toolButton_close_clicked();

signals:

private:
    Ui::Login *ui;
    IdChooseUi* idChoose;
    QTcpSocket* client_socket;
    LoadingUi* loadingUI;
    User user;
    QTimer m_Timer;

    QPoint last;


private:
    void creatIdChooseUi();
    void initUi();

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);


};

#endif // LOGIN_H
