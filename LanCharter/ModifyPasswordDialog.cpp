#include "ModifyPasswordDialog.h"
#include "ui_ModifyPasswordDialog.h"

ModifyPasswordDialog::ModifyPasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModifyPasswordDialog)
{
    ui->setupUi(this);

    setWindowTitle("modify your infos");
    this->setFixedSize(QSize(564,308));
}

ModifyPasswordDialog::~ModifyPasswordDialog()
{
    delete ui;
}

void ModifyPasswordDialog::on_buttonBox_accepted()
{
    qDebug() << "button accept";
    bool ret = isLineEditCheck();
    if( !ret )
        return;
    //发送密码给服务器
    DominantUser* instance = DominantUser::getInstance();

    QTcpSocket* clientSocket = instance->getTcpSocket();
    User user = instance->getUser();

    Protocol packReq;
    packReq.setType(Protocol::modifypassword);
    packReq["userName"] = user.getName();
    packReq["userOldPassword"] = this->ui->lineEdit_old_password->text();
    packReq["userNewPassword"] = this->ui->lineEdit_new_password->text();

    qDebug() << "userName " << user.getName();
    qDebug() << "userOldPassword " << this->ui->lineEdit_old_password->text();
    qDebug() << "userNewPassword " << this->ui->lineEdit_new_password->text();

    //清理dialog上的残留文字
    this->ui->lineEdit_old_password->clear();
    this->ui->lineEdit_new_password->clear();
    this->ui->lineEdit_new_password_again->clear();

    clientSocket->write(packReq.pack());

}


bool ModifyPasswordDialog::isLineEditCheck()
{
    if(ui->lineEdit_old_password->text().isEmpty()){
        QMessageBox::warning(this, "信息提示", "用户名不能为空");
        return false;
    }

    if(ui->lineEdit_new_password->text().isEmpty()){
        QMessageBox::warning(this, "信息提示", "新密码不能为空");
        return false;
    }
    if(ui->lineEdit_new_password_again->text().isEmpty()){
        QMessageBox::warning(this, "信息提示", "新密码不能为空");
        return false;
    }

    DominantUser* instance = DominantUser::getInstance();

    if( ui->lineEdit_old_password->text() != instance->getUser().getPassword() )
    {
        QMessageBox::warning(this, "信息提示", "旧密码不正确");
        return false;
    }
    if( ui->lineEdit_new_password->text() != ui->lineEdit_new_password_again->text() )
    {
        QMessageBox::warning(this, "信息提示", "两次输入的新密码不一致");
        return false;
    }

    if( ui->lineEdit_new_password->text() == ui->lineEdit_old_password->text() )
    {
        QMessageBox::warning(this, "信息提示", "新旧密码不能一致");
        return false;
    }

    return true;
}

void ModifyPasswordDialog::on_buttonBox_rejected()
{
    //清理dialog上的残留文字
    this->ui->lineEdit_old_password->clear();
    this->ui->lineEdit_new_password->clear();
    this->ui->lineEdit_new_password_again->clear();
}
