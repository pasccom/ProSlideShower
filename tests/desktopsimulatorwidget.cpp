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

#include "desktopsimulatorwidget.h"

#if QT_VERSION > 0x050000
#   include <QtWidgets>
#else
#   include <QtGui>
#endif

DesktopSimulatorWidget::DesktopSimulatorWidget(int h, int v, QWidget *parent):
    QWidget(parent)
{
    mDesktop = qApp->desktop();

    mHDesktops = h;
    mVDesktops = v;

    for (int s = 0; s < mDesktop->screenCount(); s++) {
        if (s != mDesktop->primaryScreen()) {
            mAuxScreenRect = mDesktop->screenGeometry(s);
            break;
        }
    }

    if (mAuxScreenRect.isNull())
        mAuxScreenRect = mDesktop->screenGeometry(mDesktop->primaryScreen());
}

int DesktopSimulatorWidget::primaryScreen(void) const
{
    if ((mHDesktops == 0) || (mVDesktops == 0))
        return mDesktop->primaryScreen();
    return 0;

}

int DesktopSimulatorWidget::screenCount(void) const
{
    if ((mHDesktops == 0) && (mVDesktops == 0))
        return 1;
    if ((mHDesktops == 0) || (mVDesktops == 0))
        return mDesktop->screenCount();
    qDebug() << mAuxScreenRect << mDesktop->screenGeometry(mDesktop->primaryScreen())
             << (mAuxScreenRect == mDesktop->screenGeometry(mDesktop->primaryScreen()));
    return 1*((int) (mAuxScreenRect != mDesktop->screenGeometry(mDesktop->primaryScreen()))) + mHDesktops*mVDesktops;
}

QRect DesktopSimulatorWidget::screenGeometry(int screen) const
{
    if ((mHDesktops == 0) || (mVDesktops == 0))
        return mDesktop->screenGeometry(screen);
    if (screen == -1)
        screen = 0;

    if (mAuxScreenRect == mDesktop->screenGeometry(mDesktop->primaryScreen())) {
        return computeScreen(screen + 1);
    } else {
        if (screen == 0)
            return mDesktop->screenGeometry(mDesktop->primaryScreen());
        return computeScreen(screen);
    }
}

QRect DesktopSimulatorWidget::computeScreen(int screen) const
{
    if (screen > mHDesktops*mVDesktops)
        return QRect();

    int x = (screen - 1) % mHDesktops;
    int y = (screen - 1) / mHDesktops;
    int w = mAuxScreenRect.width() / mHDesktops;
    int h = mAuxScreenRect.height() / mVDesktops;

    return QRect(mAuxScreenRect.x() + x*w, mAuxScreenRect.y() + y*h, w, h);
}
