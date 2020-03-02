#ifndef VIDEOLABEL_H
#define VIDEOLABEL_H

#include <QWidget>
#include <QLabel>
#include "Danmu.h"

class VideoLabel : public QLabel
{
    Q_OBJECT
public:
    explicit VideoLabel(QWidget *parent = nullptr);

signals:

public slots:
    void makeDanMu(QString text);
private:
       void getGeo();
};

#endif // VIDEOLABEL_H
