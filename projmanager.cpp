#include "projmanager.h"
#include "presmodel.h"
#include "projcontroller.h"
#include "projdisplay.h"

#include <QtCore>
#include <QtGui>
#if QT_VERSION >= 0x050000
#   include <QtWidgets>
#endif

ProjManager::ProjManager(QObject *parent) :
    QObject(parent)
{
    qApp->installEventFilter(this);

    QDesktopWidget* desktop = qApp->desktop();
    QAction *loadFileAction = new QAction(QIcon(":/icons/open.png"), tr("&Open"), this);

    mModel = new PresModel(QString::null, this);
    mModel->setVirtualScreens(2);

    mController = new ProjController(mModel, NULL);
    mController->setGeometry(desktop->screenGeometry(desktop->primaryScreen()));
    mController->show();
    mController->setTotalTime(QTime(0, 1, 0));

    mDisplays.resize(desktop->screenCount());

    qDebug() << desktop->screenCount() << desktop->primaryScreen();
    bool first = true;
    for (int d = 0; d < mDisplays.size(); d++) {
        qDebug() << desktop->screenGeometry(d);
        if (first && (d != desktop->primaryScreen())) {
            mDisplays[d] = new ProjDisplay(mModel, NULL);
            mDisplays[d]->setGeometry(desktop->screenGeometry(d));
            mDisplays[d]->show();
            mDisplays[d]->addAction(loadFileAction);

            connect(mModel, SIGNAL(currentPageChanged()),
                    mDisplays[d], SLOT(update()));
        } else {
            mDisplays[d] = NULL;
        }
    }

    connect(loadFileAction, SIGNAL(triggered()),
            this, SLOT(handleLoadFile()));

    connect(mModel, SIGNAL(currentPageChanged()),
            mController, SLOT(handleSlideChange()));
}

ProjManager::~ProjManager(void)
{
    qDeleteAll(mDisplays);
    delete mController;
}

void ProjManager::handleLoadFile(void)
{
    QFileDialog *fileDialog = new QFileDialog(mController, tr("Choose a PDF file"), "", "*.pdf");
    fileDialog->setFileMode(QFileDialog::ExistingFile);
    fileDialog->setWindowModality(Qt::ApplicationModal);

    if ((fileDialog->exec() == QDialog::Rejected) || fileDialog->selectedFiles().isEmpty())
        return;
    mModel->load(fileDialog->selectedFiles().first());
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
    switch (ke->key()) {
    case Qt::Key_Left:
        ke->accept();
        mModel->goToPrevPage();
        break;
    case Qt::Key_Right:
        ke->accept();
        mModel->goToNextPage();
        break;
    case Qt::Key_S:
        if (ke->modifiers() == Qt::CTRL) {
            ke->accept();
            mController->start();
        }
        break;
    case Qt::Key_P:
        if (ke->modifiers() == Qt::CTRL) {
            ke->accept();
            mController->pause();
        }
        break;
    case Qt::Key_X:
        if (ke->modifiers() == Qt::CTRL) {
            ke->accept();
            mController->stop();
        }
        break;
    }
}
