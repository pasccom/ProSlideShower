/* Copyright 2016 Pascal COMBES <pascom@orange.fr>
 * 
 * This file is part of ProSlideShower.
 * 
 * ProSlideShower is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ProSlideShower is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with ProSlideShower. If not, see <http://www.gnu.org/licenses/>
 */

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
