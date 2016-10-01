#include "projdisplay.h"
#include "presmodel.h"

#include <QtGui>
#if QT_VERSION > 0x050000
#   include <QtWidgets>
#endif

ProjDisplay::ProjDisplay(PresModel* model, QWidget* parent) :
    QWidget(parent, Qt::FramelessWindowHint)
{
    setContextMenuPolicy(Qt::ActionsContextMenu);

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
    setContextMenuPolicy(Qt::ActionsContextMenu);

    mModel = new PresModel(path, this);
    mModelOwned = true;

    mOffset = 0;
    mHVirtualScreen = 1;
    mVVirtualScreen = 1;

    connectToModel();
}

ProjDisplay::~ProjDisplay(void)
{
    if (mModelOwned && (mModel != NULL))
        delete mModel;
}

void ProjDisplay::setModel(PresModel* model)
{
    disconnectFromModel();

    if (mModelOwned)
        delete mModel;

    mModel = model;
    mModelOwned = false;
    repaint();

    connectToModel();
}

void ProjDisplay::setModel(const QString& path)
{
    disconnectFromModel();

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

void ProjDisplay::disconnectFromModel(void)
{
    if (mModel == NULL)
        return;

    disconnect(mModel, SIGNAL(documentChanged()),
            this, SLOT(update()));
    disconnect(mModel, SIGNAL(virtualScreenNumberChanged()),
            this, SLOT(update()));
    disconnect(mModel, SIGNAL(currentPageChanged()),
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

    QImage pageImg = mModel->getCurrentPage(rect(), mHVirtualScreen, mVVirtualScreen, mOffset);

    QRectF imgRect((rect().width() - pageImg.width()) / 2., (rect().height() - pageImg.height()) / 2., pageImg.width(), pageImg.height());
    p.drawImage(imgRect, pageImg);
}
