#include "projmanager.h"
#include "presmodel.h"
#include "projcontroller.h"
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

    setModel(new PresModel(QString::null, this));

    if (desktop->screenCount() > 1) {
        mController = new ProjController(model(), NULL);
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
