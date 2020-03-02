#include "UserChargeMoneyDialog.h"
#include "ui_UserChargeMoneyDialog.h"

UserChargeMoneyDialog::UserChargeMoneyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserChargeMoneyDialog)
{
    ui->setupUi(this);
    setWindowTitle("charge money");
    this->setFixedSize(QSize(402,300));
}

UserChargeMoneyDialog::~UserChargeMoneyDialog()
{
    delete ui;
}

void UserChargeMoneyDialog::on_buttonBox_accepted()
{
    QString smoney = this->ui->lineEdit->text();
    float money = 0;
    money = smoney.toFloat();
    if( money <= 0)
    {
       QMessageBox::warning(this, "警告", "充值金额错误");
       ui->lineEdit->clear();
       return;
    }
    qDebug() << money;
    //发送金额给服务器
    DominantUser* instance = DominantUser::getInstance();

    QTcpSocket* clientSocket = instance->getTcpSocket();
    User user = instance->getUser();

    Protocol packReq;
    packReq.setType(Protocol::chargemoney);
    packReq["userName"] = user.getName();
    packReq["userPassword"] = user.getPassword();
    packReq["chargeMoney"] = smoney;

    clientSocket->write(packReq.pack());
    ui->lineEdit->clear();

}

void UserChargeMoneyDialog::on_buttonBox_rejected()
{
    ui->lineEdit->clear();
}
