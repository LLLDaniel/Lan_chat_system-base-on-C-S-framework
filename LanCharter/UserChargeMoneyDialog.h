#ifndef USERCHARGEMONEYDIALOG_H
#define USERCHARGEMONEYDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QMessageBox>
#include "DominantUser.h"
#include "protocol.h"

namespace Ui {
class UserChargeMoneyDialog;
}

class UserChargeMoneyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserChargeMoneyDialog(QWidget *parent = nullptr);
    ~UserChargeMoneyDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::UserChargeMoneyDialog *ui;
};

#endif // USERCHARGEMONEYDIALOG_H
