#ifndef QIMAGEBOX_H
#define QIMAGEBOX_H

#include <QLabel>
#include <QLineEdit>
#include <QTextBrowser>
#include "Core/Item.h"

class QImageBox : public QLabel
{
    Q_OBJECT
private:
    Item *item;
public:
    explicit QImageBox(Item *item, QWidget *parent = 0);
    void setImage(QImage *iamage, bool fromPlayer = false);
    void setChosen();
signals:
    void chooseImageBox(Item*);
protected:
    void mousePressEvent(QMouseEvent *event);
};

#endif // QIMAGEBOX_H
