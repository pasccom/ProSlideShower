#include "subdisplayhandler.h"
#include "projdisplay.h"
#include "presmodel.h"

#if QT_VERSION > 0x050000
#   include <QtWidgets>
#else
#   include <QtGui>
#endif

SubDisplayHandler::SubDisplayHandler(PresModel* model, QObject* parent):
    QObject(parent), QVector<ProjDisplay *>(), mModel(model), mParent(NULL)
{
    if (mModel != NULL)
        connect(mModel, SIGNAL(virtualScreenNumberChanged()),
                this, SLOT(updateDisplayActions()));
}

SubDisplayHandler::~SubDisplayHandler(void)
{
    for (int d = 0; d < size(); d++)
        delete at(d);
}

void SubDisplayHandler::setModel(PresModel* model)
{
    if (mModel != NULL) {
        disconnect(mModel, SIGNAL(virtualScreenNumberChanged()),
                this, SLOT(updateDisplayActions()));
    }
    mModel = model;
    if (mModel != NULL)
        connect(mModel, SIGNAL(virtualScreenNumberChanged()),
                this, SLOT(updateDisplayActions()));

    for (int d = 0; d < size(); d++)
        if (at(d) != NULL)
            at(d)->setModel(model);
    updateDisplayActions();
}

void SubDisplayHandler::load(const QString& file, int h, int v)
{
    if (!model()->load(file))
        return;
    setModel(model());

    if (h <= 0)
        handleVirtualScreens();
    else
        model()->setVirtualScreens(h, v);
}

void SubDisplayHandler::goToNextPage()
{
    for (int d = 0; d < size(); d++)
        if ((at(d) != NULL) && (at(d)->model() != NULL) && (at(d)->model() != model()))
            at(d)->model()->goToNextPage();
}

void SubDisplayHandler::goToPrevPage()
{
    for (int d = 0; d < size(); d++)
        if ((at(d) != NULL) && (at(d)->model() != NULL) && (at(d)->model() != model()))
            at(d)->model()->goToPrevPage();
}

void SubDisplayHandler::load(void)
{
    QSettings settings(this);
    if (settings.status() != QSettings::NoError)
        qWarning() << "Could not open settings. Status:" << settings.status();

    QString openPath = settings.value("openPath").toString();
    if (!QFile::exists(openPath))
        openPath = QString::null;

    QFileDialog fileDialog(mParent, tr("Choose a PDF file"), openPath, tr("PDF files (*.pdf)"));
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setWindowModality(Qt::ApplicationModal);

    if ((fileDialog.exec() == QDialog::Rejected) || fileDialog.selectedFiles().isEmpty())
        return;

    QDir openDir(fileDialog.selectedFiles().first());
    if (openDir.cdUp())
        settings.setValue("openPath", openDir.absolutePath());

    bool ok = false;
    ProjDisplay *senderDisplay = NULL;
    QAction *senderAction = qobject_cast<QAction *>(sender());
    if (senderAction != NULL)
        senderDisplay = reinterpret_cast<ProjDisplay *>(senderAction->data().toULongLong(&ok));
    if (!ok)
        senderDisplay = NULL;

    if (senderDisplay != NULL) {
        senderDisplay->setModel(fileDialog.selectedFiles().first());
        senderDisplay->model()->setCurrentPageNumber(model()->getCurrentPageNumber());
        handleVirtualScreens();
    } else {
        load(fileDialog.selectedFiles().first());
    }
}

void SubDisplayHandler::handleVirtualScreens(void)
{
    QInputDialog virtualScreenDialog(mParent);
    virtualScreenDialog.setWindowTitle(tr("Number of virtual screens"));
    virtualScreenDialog.setLabelText(tr("Enter number of virtual screens in the presentation"));
    virtualScreenDialog.setInputMode(QInputDialog::IntInput);
    virtualScreenDialog.setIntMinimum(1);
    virtualScreenDialog.setIntMaximum(8);
    virtualScreenDialog.setIntStep(1);
    virtualScreenDialog.setIntValue(1);
    if (virtualScreenDialog.exec() == QDialog::Rejected)
        return;

    bool ok = false;
    ProjDisplay *senderDisplay = NULL;
    QAction *senderAction = qobject_cast<QAction *>(sender());
    if (senderAction != NULL)
        senderDisplay = reinterpret_cast<ProjDisplay *>(senderAction->data().toULongLong(&ok));
    if (!ok)
        senderDisplay = NULL;

    if (senderDisplay != NULL)
        senderDisplay->model()->setVirtualScreens(virtualScreenDialog.intValue());
    else
        model()->setVirtualScreens(virtualScreenDialog.intValue());
    updateDisplayActions();
}

void SubDisplayHandler::handleOffset(void)
{
    bool ok = false;
    ProjDisplay *senderDisplay = NULL;
    QAction *senderAction = qobject_cast<QAction *>(sender());
    if (senderAction != NULL)
        senderDisplay = reinterpret_cast<ProjDisplay *>(senderAction->data().toULongLong(&ok));
    if (!ok)
        senderDisplay = NULL;
    if (senderDisplay == NULL)
        return;

    QInputDialog virtualScreenDialog(mParent);
    virtualScreenDialog.setWindowTitle(tr("Page offset"));
    virtualScreenDialog.setLabelText(tr("Enter the offset of the displayed page"));
    virtualScreenDialog.setInputMode(QInputDialog::IntInput);
    virtualScreenDialog.setIntMinimum(INT_MIN);
    virtualScreenDialog.setIntMaximum(INT_MAX);
    virtualScreenDialog.setIntStep(1);
    virtualScreenDialog.setIntValue(senderDisplay->offset());
    if (virtualScreenDialog.exec() == QDialog::Rejected)
        return;

    senderDisplay->setOffset(virtualScreenDialog.intValue());
    senderDisplay->update();
}

void SubDisplayHandler::updateDisplayActions(void)
{
    QSet<QAction *> actionsToDelete;
    for (int d = 0; d < size(); d++) {
        if (at(d) == NULL)
            continue;
        actionsToDelete.unite(at(d)->actions().toSet());
        updateDisplayActions(at(d));
    }

    foreach (QAction* action, actionsToDelete)
        action->deleteLater();
}

void SubDisplayHandler::updateDisplayActions(ProjDisplay* display)
{
    QAction* loadMasterAction = new QAction(QIcon(":/icons/open.png"), tr("Load &Master"), this);
    loadMasterAction->setData(QVariant((quint64) 0));
    QAction* loadSlaveAction = new QAction(QIcon(":/icons/open.png"), tr("Load &Slave"), this);
    loadSlaveAction->setData(QVariant(reinterpret_cast<qulonglong>(display)));
    QAction* virtualScreenAction = new QAction(tr("&Virtual screen number"), this);
    virtualScreenAction->setData(QVariant(reinterpret_cast<qulonglong>(display)));
    QAction* offsetAction = new QAction(tr("&Display offset"), this);
    offsetAction->setData(QVariant(reinterpret_cast<qulonglong>(display)));

    connect(loadMasterAction, SIGNAL(triggered()),
            this, SLOT(load()));
    connect(loadSlaveAction, SIGNAL(triggered(bool)),
            this, SLOT(load()));
    connect(virtualScreenAction, SIGNAL(triggered()),
            this, SLOT(handleVirtualScreens()));
    connect(offsetAction, SIGNAL(triggered()),
            this, SLOT(handleOffset()));

    QList<QAction *> actions = display->actions();
    foreach (QAction *action, actions) {
        display->removeAction(action);
    }

    display->addAction(loadMasterAction);
    display->addAction(loadSlaveAction);
    if ((display->model() != NULL) && display->model()->isOK())
        display->addAction(virtualScreenAction);
    display->addAction(offsetAction);
    if ((display->model() != NULL) && display->model()->isOK() && (display->model()->horizontalVirtualScreens() > 1)) {
        for (int s = 0; s < display->model()->horizontalVirtualScreens(); s++) {
            QAction* useVirtualScreenAction = new QAction(tr("Use virtual screen &%1").arg(s + 1), this);
            useVirtualScreenAction->setData(QVariant(s + 1));
            connect(useVirtualScreenAction, SIGNAL(triggered()),
                    display, SLOT(setHorizontalVirtualScreen()));
            display->addAction(useVirtualScreenAction);
        }
    }
}
