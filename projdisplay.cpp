#include "projdisplay.h"
#include "presmodel.h"

#include <QtGui>
#if QT_VERSION > 0x050000
#   include <QtWidgets>
#endif

ProjDisplay::ProjDisplay(PresModel* model, QWidget* parent) :
    QWidget(parent, Qt::FramelessWindowHint)
{
    mModel = model;
    mModelOwned = false;

    mOffset = 0;
    mHVirtualScreen = 1;
    mVVirtualScreen = 1;

    connectToModel();
}

ProjDisplay::ProjDisplay(const QString& path, QWidget* parent) :
    QWidget(parent, Qt::FramelessWindowHint)
{
    mModel = new PresModel(path, this);
    mModelOwned = true;

    mOffset = 0;
    mHVirtualScreen = 1;
    mVVirtualScreen = 1;

    connectToModel();
}

void ProjDisplay::setModel(PresModel* model)
{
    if (mModelOwned)
        delete mModel;

    mModel = model;
    mModelOwned = false;
    repaint();

    connectToModel();
}

void ProjDisplay::setModel(const QString& path)
{
    if (mModelOwned)
        delete mModel;

    mModel = new PresModel(path, this);
    mModelOwned = true;
    repaint();

    connectToModel();
}

void ProjDisplay::connectToModel(void)
{
    if (mModel == NULL)
        return;

    connect(mModel, SIGNAL(documentChanged()),
            this, SLOT(update()));
    connect(mModel, SIGNAL(virtualScreenNumberChanged()),
            this, SLOT(update()));
    connect(mModel, SIGNAL(currentPageChanged()),
            this, SLOT(update()));
}

void ProjDisplay::setHorizontalVirtualScreen(int horizontal)
{
    int old = mHVirtualScreen;

    if(horizontal > 0) {
        mHVirtualScreen = horizontal;
    } else {
        QAction* senderAction = qobject_cast<QAction *>(sender());
        if ((senderAction != NULL) && (senderAction->data().type() == QVariant::Int) && (senderAction->data().toInt() > 0))
            mHVirtualScreen = senderAction->data().toInt();
    }

    if (mHVirtualScreen != old)
        repaint();
}

void ProjDisplay::setVericalVirtualScreen(int vertical)
{
    int old = mVVirtualScreen;

    if(vertical > 0) {
        mVVirtualScreen = vertical;
    } else {
        QAction* senderAction = qobject_cast<QAction *>(sender());
        if ((senderAction != NULL) && (senderAction->data().type() == QVariant::Int) && (senderAction->data().toInt() > 0))
            mVVirtualScreen = senderAction->data().toInt();
    }

    if (mVVirtualScreen != old)
        repaint();
}

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

void ProjDisplay::contextMenuEvent(QContextMenuEvent *cme)
{
    QMenu *menu = new QMenu(this);
    foreach(QAction* action, actions())
        menu->addAction(action);
    if (menu->actions().count() > 0) {
        cme->accept();
        menu->popup(cme->globalPos());

        connect(menu, SIGNAL(aboutToHide()),
                menu, SLOT(deleteLater()));
    } else {
        cme->ignore();
        delete menu;
    }
}
