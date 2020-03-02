#ifndef MODIFYPASSWORDDIALOG_H
#define MODIFYPASSWORDDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include "DominantUser.h"
#include "protocol.h"

namespace Ui {
class ModifyPasswordDialog;
}

class ModifyPasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModifyPasswordDialog(QWidget *parent = nullptr);
    ~ModifyPasswordDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::ModifyPasswordDialog *ui;
private:
    bool isLineEditCheck();
};

#endif // MODIFYPASSWORDDIALOG_H
