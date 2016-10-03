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

#include "projmanager.h"
#include "presmodel.h"
#include "projdisplay.h"

#include "tests/desktopsimulatorwidget.h"

#include <QtCore>
#include <QtGui>
#if QT_VERSION >= 0x050000
#   include <QtWidgets>
#endif

ProjManager::ProjManager(QObject *parent) :
    SubDisplayHandler(NULL, parent)
{
    qApp->installEventFilter(this);

#ifndef SIMULATING_DESKTOPS
    QDesktopWidget* desktop = qApp->desktop();
#else
    DesktopSimulatorWidget *desktop = new DesktopSimulatorWidget(SIMULATING_H_DESKTOPS, SIMULATING_V_DESKTOPS);
#endif

    if (desktop->screenCount() > 1) {
        mController = new ProjController(NULL, NULL);
        mController->setGeometry(desktop->screenGeometry(desktop->primaryScreen()));
        mController->show();
        mController->setPaneHeight(200);

        setParentWidget(mController);

        new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), mController, SLOT(start()), 0, Qt::ApplicationShortcut);
        new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_P), mController, SLOT(pause()), 0, Qt::ApplicationShortcut);
        new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_X), mController, SLOT(stop()), 0, Qt::ApplicationShortcut);
        new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_C), mController, SLOT(configure()), 0, Qt::ApplicationShortcut);
    } else {
        mController = NULL;
    }

    setModel(new PresModel(QString::null, this));

    resize(desktop->screenCount());

    qDebug() << desktop->screenCount() << desktop->primaryScreen();
    bool first = true;
    for (int d = 0; d < size(); d++) {
        qDebug() << desktop->screenGeometry(d);
        if ((desktop->screenCount() == 1) || (first && (d != desktop->primaryScreen()))) {
            replace(d, new ProjDisplay(model(), NULL));
            at(d)->setGeometry(desktop->screenGeometry(d));
            at(d)->show();
        } else {
            replace(d, NULL);
        }
    }

    updateDisplayActions();

#ifdef SIMULATING_DESKTOPS
    delete desktop;
#endif
}

ProjManager::~ProjManager(void)
{
    if (mController != NULL)
        delete mController;
}

void ProjManager::setModel(PresModel* model)
{
    if (this->model() != NULL)
        this->model()->deleteLater();
    SubDisplayHandler::setModel(model);
    if (mController != NULL)
        mController->setModel(model);
}

bool ProjManager::eventFilter(QObject* watched, QEvent* event)
{
    switch(event->type()) {
    case QEvent::MouseMove:
        //qDebug() << "Mouse move event" << watched->metaObject()->className();
        mouseMoveEvent(static_cast<QMouseEvent *>(event));
        return event->isAccepted();
    case QEvent::KeyPress:
        keyPressEvent(static_cast<QKeyEvent *>(event));
        return event->isAccepted();
    case QEvent::KeyRelease:
        keyReleaseEvent(static_cast<QKeyEvent *>(event));
        return event->isAccepted();
    case QEvent::Close:
        qDebug() << "Close event" << watched->metaObject()->className();
        if ((qobject_cast<ProjDisplay *>(watched) == NULL)
         && (qobject_cast<ProjController *>(watched) == NULL))
            return false;
        qApp->quit();
        return true;
    default:
        break;
    }

    return false;
}

void ProjManager::keyPressEvent(QKeyEvent *ke)
{
    ke->ignore();

    switch (ke->key()) {
    case Qt::Key_Left:
    case Qt::Key_Up:
    case Qt::Key_PageUp:
        ke->accept();
        break;
    case Qt::Key_Right:
    case Qt::Key_Down:
    case Qt::Key_PageDown:
        ke->accept();
        break;
    }
}

void ProjManager::keyReleaseEvent(QKeyEvent *ke)
{
    //qDebug() << "key release event" << ke->key() << ke->modifiers();
    ke->ignore();

    switch (ke->key()) {
    case Qt::Key_Left:
    case Qt::Key_Up:
    case Qt::Key_PageUp:
        if (model() != NULL)
            if (!model()->goToPrevPage())
                break;
        ke->accept();
        goToPrevPage();
        if (mController != NULL)
            mController->goToPrevPage();
        break;
    case Qt::Key_Right:
    case Qt::Key_Down:
    case Qt::Key_PageDown:
        if (model() != NULL)
            if (!model()->goToNextPage())
                break;
        ke->accept();
        goToNextPage();
        if (mController != NULL)
            mController->goToNextPage();
        break;
    }
}

void ProjManager::mouseMoveEvent(QMouseEvent *me)
{
#ifndef SIMULATING_DESKTOPS
    QDesktopWidget* desktop = qApp->desktop();
#else
    DesktopSimulatorWidget *desktop = new DesktopSimulatorWidget(SIMULATING_H_DESKTOPS, SIMULATING_V_DESKTOPS);
#endif

    me->ignore();
    if (mController == NULL)
        return;

    QRect showRect(desktop->screenGeometry(desktop->primaryScreen()).topLeft(),
                   QPoint(desktop->screenGeometry(desktop->primaryScreen()).right(),
                          desktop->screenGeometry(desktop->primaryScreen()).top() + 1));
    QRect hideRect(desktop->screenGeometry(desktop->primaryScreen()).topLeft(),
                   QPoint(desktop->screenGeometry(desktop->primaryScreen()).right(),
                          desktop->screenGeometry(desktop->primaryScreen()).top() + mController->paneHeight()));

    /*qDebug() << showRect << me->globalPos() << showRect.contains(me->globalPos());
    qDebug() << hideRect << me->globalPos() << hideRect.contains(me->globalPos());*/

    if (showRect.contains(me->globalPos())) {
        mController->showControlPane();
        me->accept();
    }
    if (!hideRect.contains(me->globalPos()) && (me->buttons() == Qt::NoButton))
        mController->hideControlPane();

#ifdef SIMULATING_DESKTOPS
    delete desktop;
#endif
}
