#ifndef IDCHOOSEUI_H
#define IDCHOOSEUI_H

#include <QDialog>
#include "User.h"
#include "UserUi.h"
#include "DominantUser.h"

namespace Ui {
class IdChooseUi;
}

class IdChooseUi : public QDialog
{
    Q_OBJECT

public:
    explicit IdChooseUi(QWidget *parent = nullptr);

    ~IdChooseUi();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_btn_anchor_clicked();

    void on_btn_tourist_clicked();

private:
    Ui::IdChooseUi *ui;
    UserUi* userUi;

private:
    void setUserId(User::indentity type);
};

#endif // IDCHOOSEUI_H
