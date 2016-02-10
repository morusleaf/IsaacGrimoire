#include "qimagebox.h"
#include <QDebug>
#include <QLayout>

QImageBox::QImageBox(Item *item, QWidget *parent)
    : QLabel(parent),
      item(item)
{
    //item = _item;
    setAutoFillBackground(true);
    setAttribute(Qt::WA_Hover, true);
    setFixedSize(32, 32);
    setAlignment(Qt::AlignCenter);
}

void QImageBox::setImage(QImage *image, bool fromPlayer) {
    if (!fromPlayer) {
        QPalette pal = palette();
        pal.setColor(QPalette::Window, QColor(212,212,212));
        setPalette(pal);
    }
    setPixmap(QPixmap::fromImage(*image));
}

void QImageBox::mousePressEvent(QMouseEvent*) {
    setChosen();
}

void QImageBox::setChosen() {
    // reset every ItemBox's style
    // TODO: use callback
//    QFrame *ItemPane = (QFrame*)this->parentWidget();
//    int layoutsize = ItemPane->layout()->count();
//    for (int i=0; i<layoutsize; i++) {
//        ((QImageBox*)ItemPane->layout()->itemAt(i)->widget())->setFrameStyle(QFrame::Plain);
//    }
    // update InfoPane
    emit chooseImageBox(item);
    // set curent ItemBox's style
    setFrameStyle(QFrame::Sunken | QFrame::Panel);
}
