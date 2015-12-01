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

    mModel = new PresModel(QString::null, this);

    mController = new ProjController(mModel, NULL);
    mController->setGeometry(desktop->screenGeometry(desktop->primaryScreen()));
    mController->show();
    mController->setTotalTime(QTime(0, 45, 0));

    mDisplays.resize(desktop->screenCount());

    qDebug() << desktop->screenCount() << desktop->primaryScreen();
    bool first = true;
    for (int d = 0; d < mDisplays.size(); d++) {
        qDebug() << desktop->screenGeometry(d);
        if (first && (d != desktop->primaryScreen())) {
            mDisplays[d] = new ProjDisplay(mModel, NULL);
            mDisplays[d]->setGeometry(desktop->screenGeometry(d));
            mDisplays[d]->show();

            connect(mModel, SIGNAL(currentPageChanged()),
                    mDisplays[d], SLOT(update()));
        } else {
            mDisplays[d] = NULL;
        }
    }

    updateDisplayActions();

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
    QFileDialog fileDialog(mController, tr("Choose a PDF file"), "", "*.pdf");
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setWindowModality(Qt::ApplicationModal);

    if ((fileDialog.exec() == QDialog::Rejected) || fileDialog.selectedFiles().isEmpty())
        return;

    mModel->load(fileDialog.selectedFiles().first());
    handleVirtualScreens();
}

void ProjManager::handleVirtualScreens(void)
{
    QInputDialog virtualScreenDialog(mController);
    virtualScreenDialog.setWindowTitle(tr("Number of virtual screens"));
    virtualScreenDialog.setLabelText(tr("Enter number of virtual screens in the presentation"));
    virtualScreenDialog.setInputMode(QInputDialog::IntInput);
    virtualScreenDialog.setIntMinimum(1);
    virtualScreenDialog.setIntMaximum(8);
    virtualScreenDialog.setIntStep(1);
    virtualScreenDialog.setIntValue(1);
    if (virtualScreenDialog.exec() == QDialog::Rejected)
        return;

    mModel->setVirtualScreens(virtualScreenDialog.intValue());
    updateDisplayActions();
}

void ProjManager::updateDisplayActions(void)
{
    QAction* loadFileAction = new QAction(QIcon(":/icons/open.png"), tr("&Open"), this);
    QAction* virtualScreenAction = new QAction(tr("&Virtual screen number"), this);

    connect(loadFileAction, SIGNAL(triggered()),
            this, SLOT(handleLoadFile()));
    connect(virtualScreenAction, SIGNAL(triggered()),
            this, SLOT(handleVirtualScreens()));

    QSet<QAction *> actionsToDelete;
    for (int d = 0; d < mDisplays.size(); d++) {
        if (mDisplays[d] == NULL)
            continue;

        QList<QAction *> actions = mDisplays[d]->actions();
        foreach (QAction *action, actions) {
            mDisplays[d]->removeAction(action);
            actionsToDelete.insert(action);
        }

        mDisplays[d]->addAction(loadFileAction);
        if (mModel->isOK())
            mDisplays[d]->addAction(virtualScreenAction);
        if (mModel->isOK() && (mModel->horizontalVirtualScreens() > 1)) {
            for (int s = 0; s < mModel->horizontalVirtualScreens(); s++) {
                QAction* useVirtualScreenAction = new QAction(tr("Use virtual screen &%1").arg(s + 1), this);
                useVirtualScreenAction->setData(QVariant(s + 1));
                connect(useVirtualScreenAction, SIGNAL(triggered()),
                        mDisplays[d], SLOT(setHorizontalVirtualScreen()));
                mDisplays[d]->addAction(useVirtualScreenAction);
            }
        }
    }
    qDeleteAll(actionsToDelete);
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
    case Qt::Key_Up:
    case Qt::Key_PageUp:
        ke->accept();
        mModel->goToPrevPage();
        break;
    case Qt::Key_Right:
    case Qt::Key_Down:
    case Qt::Key_PageDown:
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
