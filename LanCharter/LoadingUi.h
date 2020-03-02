#ifndef LOADINGUI_H
#define LOADINGUI_H

#include <QWidget>
#include <QDebug>
#include <QMovie>

namespace Ui {
class LoadingUi;
}

class LoadingUi : public QWidget
{
    Q_OBJECT

public:
    explicit LoadingUi(QWidget *parent = nullptr);
    ~LoadingUi();

private:
    Ui::LoadingUi *ui;
};

#endif // LOADINGUI_H
