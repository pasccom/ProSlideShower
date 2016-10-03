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
