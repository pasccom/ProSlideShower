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

    mController = new ProjController(model(), NULL);
    mController->setGeometry(desktop->screenGeometry(desktop->primaryScreen()));
    mController->show();
    mController->setTotalTime(QTime(0, 45, 0));

    setParentWidget(mController);

    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), mController, SLOT(start()), 0, Qt::ApplicationShortcut);
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_P), mController, SLOT(pause()), 0, Qt::ApplicationShortcut);
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_X), mController, SLOT(stop()), 0, Qt::ApplicationShortcut);

    resize(desktop->screenCount());

    qDebug() << desktop->screenCount() << desktop->primaryScreen();
    bool first = true;
    for (int d = 0; d < size(); d++) {
        qDebug() << desktop->screenGeometry(d);
        if (first && (d != desktop->primaryScreen())) {
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
    delete mController;
}


void ProjManager::load(const QString& file)
{
    SubDisplayHandler::load(file);
    mController->setModel(model());
}

bool ProjManager::eventFilter(QObject* watched, QEvent* event)
{
    switch(event->type()) {
    case QEvent::KeyRelease:
        keyReleaseEvent(static_cast<QKeyEvent *>(event));
        return event->isAccepted();
    case QEvent::Close:
        qDebug() << watched->metaObject()->className();
        if (qobject_cast<QMenu *>(watched) != NULL)
            return false;
        qApp->quit();
        return true;
    default:
        break;
    }

    return false;
}

void ProjManager::keyReleaseEvent(QKeyEvent *ke)
{
    //qDebug() << "key release event" << ke->key() << ke->modifiers();

    switch (ke->key()) {
    case Qt::Key_Left:
    case Qt::Key_Up:
    case Qt::Key_PageUp:
        ke->accept();
        if (model() != NULL)
            if (!model()->goToPrevPage())
                break;
        goToPrevPage();
        mController->goToPrevPage();
        break;
    case Qt::Key_Right:
    case Qt::Key_Down:
    case Qt::Key_PageDown:
        ke->accept();
        if (model() != NULL)
            if (!model()->goToNextPage())
                break;
        goToNextPage();
        mController->goToNextPage();
        break;
    }
}
