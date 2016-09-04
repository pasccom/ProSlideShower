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
