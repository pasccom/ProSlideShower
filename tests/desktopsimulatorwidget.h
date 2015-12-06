#ifndef DESKTOPSIMULATORWIDGET_H
#define DESKTOPSIMULATORWIDGET_H

#include <QWidget>

class QDesktopWidget;

class DesktopSimulatorWidget : public QWidget
{
    Q_OBJECT
public:
    DesktopSimulatorWidget(int h = 0, int v = 0, QWidget *parent = NULL);

    int primaryScreen(void) const;
    int screenCount(void) const;
    QRect screenGeometry(int screen) const;
signals:

public slots:

private:
    QRect computeScreen(int screen) const;

    QDesktopWidget *mDesktop;
    QRect mAuxScreenRect;
    int mHDesktops;
    int mVDesktops;
};

#endif // DESKTOPSIMULATORWIDGET_H
