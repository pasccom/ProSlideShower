#include "projdisplay.h"
#include "presmodel.h"

#include <QtGui>


void ProjDisplay::paintEvent(QPaintEvent *pe)
{
    QPainter p(this);
    p.fillRect(pe->rect(), QBrush(Qt::black, Qt::SolidPattern));
    if (mModel == NULL)
        return;

    QImage pageImg;
    switch(mOffset) {
    case 0:
        pageImg = mModel->getCurrentPage(rect(), mHVirtualScreen, mVVirtualScreen);
        break;
    case 1:
        pageImg = mModel->getNextPage(rect(), mHVirtualScreen, mVVirtualScreen);
        break;
    case -1:
        pageImg = mModel->getPrevPage(rect(), mHVirtualScreen, mVVirtualScreen);
        break;
    default:
        Q_ASSERT(false);
    }

    QRectF imgRect((rect().width() - pageImg.width()) / 2., (rect().height() - pageImg.height()) / 2., pageImg.width(), pageImg.height());
    p.drawImage(imgRect, pageImg);
}
