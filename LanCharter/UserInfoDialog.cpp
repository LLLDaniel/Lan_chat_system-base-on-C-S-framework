#include "UserInfoDialog.h"
#include "ui_UserInfoDialog.h"

UserInfoDialog::UserInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserInfoDialog)
{
    ui->setupUi(this);

    setWindowTitle("living room");
    this->setFixedSize(QSize(501,277));
    this->ui->label_name->setFont(QFont("SimHei",10,100));
    this->ui->label_name_act->setFont(QFont("SimHei",10,100));
    this->ui->label_password->setFont(QFont("SimHei",10,100));
    this->ui->label_password_act->setFont(QFont("SimHei",10,100));
    this->ui->label_money->setFont(QFont("SimHei",10,100));
    this->ui->label_money_act->setFont(QFont("SimHei",10,100));

//    QPalette pe;
//    pe.setColor(QPalette::WindowText,Qt::green);
//    this->ui->label_name->setPalette(pe);

}

UserInfoDialog::~UserInfoDialog()
{
    delete ui;
}

void UserInfoDialog::getUserInfo(const QString &userName, const QString &userPassword,const QString &userMoney)
{
    this->ui->label_name_act->setText(userName);
    this->ui->label_password_act->setText(userPassword);
    this->ui->label_money_act->setText(userMoney);

}
