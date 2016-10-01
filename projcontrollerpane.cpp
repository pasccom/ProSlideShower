#include "projcontrollerpane.h"
#include "presmodel.h"
#include "galleryview.h"

#if QT_VERSION > 0x050000
#   include <QtWidgets>
#else
#   include <QtGui>
#endif

ProjControllerPane::ProjControllerPane(QWidget *parent) :
    QWidget(parent)
{
   mSlidesView = new GalleryView(Qt::Horizontal, this);
   mSlidesView->setTopPadding(1, 2);
   mSlidesView->setBottomPadding(1, 2);
   mSlidesView->setLeftPadding(1, 2);
   mSlidesView->setRightPadding(1, 2);
   mSlidesView->setTopMargin(1, 2);
   mSlidesView->setBottomMargin(1, 2);
   mSlidesView->setLeftMargin(5);
   mSlidesView->setRightMargin(5);
   mSlidesView->setSpacing(5);
   mSlidesView->setSelectionMode(QAbstractItemView::SingleSelection);

   mOpenButton = new QToolButton(this);
   mOpenButton->setIcon(QIcon(":/icons/open.png"));
   mOpenButton->setToolTip(tr("Open master"));
   mOpenButton->installEventFilter(this);

   mConfigButton = new QToolButton(this);
   mConfigButton->setIcon(QIcon(":/icons/configure.png"));
   mConfigButton->setToolTip(tr("Configure"));
   mConfigButton->installEventFilter(this);

   mCloseButton = new QToolButton(this);
   mCloseButton->setIcon(QIcon(":/icons/close.png"));
   mCloseButton->setToolTip(tr("Quit"));
   mCloseButton->installEventFilter(this);

   QBoxLayout *leftButtonLayout = new QVBoxLayout;
   leftButtonLayout->setContentsMargins(0, 0, 0, 0);
   leftButtonLayout->addWidget(mOpenButton);
   leftButtonLayout->addStretch(1);

   QBoxLayout *rightButtonLayout = new QVBoxLayout;
   rightButtonLayout->setContentsMargins(0, 0, 0, 0);
   rightButtonLayout->addWidget(mCloseButton);
   rightButtonLayout->addWidget(mConfigButton);
   rightButtonLayout->addStretch(1);

   QBoxLayout *mainLayout = new QHBoxLayout;
   mainLayout->setContentsMargins(6, 6, 6, 0);
   mainLayout->addLayout(leftButtonLayout);
   mainLayout->addWidget(mSlidesView, 1);
   mainLayout->addLayout(rightButtonLayout);

   setLayout(mainLayout);

   connect(mCloseButton, SIGNAL(clicked(bool)),
           qApp, SLOT(quit()));
   connect(mOpenButton, SIGNAL(clicked(bool)),
           this, SIGNAL(openDocumentRequest()));
   connect(mConfigButton, SIGNAL(clicked(bool)),
           this, SIGNAL(configureRequest()));
}

void ProjControllerPane::setModel(PresModel *model)
{
    if (mSlidesView->model() != NULL) {
        disconnect(mSlidesView, SIGNAL(doubleClicked(const QModelIndex&)),
                   mSlidesView->model(), SLOT(setCurrentPage(const QModelIndex&)));
        disconnect(mSlidesView->model(), SIGNAL(currentPageChanged()),
                   this, SIGNAL(handlePageChange()));
        disconnect(mSlidesView->model(), SIGNAL(modelReset()),
                   this, SLOT(handlePageChange()));
    }

    mSlidesView->setModel(model);
    if (model == NULL)
        return;

    mSlidesView->setCurrentIndex(model->getCurentIndex());
    mSlidesView->center(model->getCurentIndex());

    connect(mSlidesView, SIGNAL(doubleClicked(const QModelIndex&)),
            model, SLOT(setCurrentPage(const QModelIndex&)));
    connect(model, SIGNAL(currentPageChanged()),
            this, SLOT(handlePageChange()));
    connect(model, SIGNAL(modelReset()),
            this, SLOT(handlePageChange()));
}

void ProjControllerPane::handlePageChange(void)
{
    PresModel *presModel = qobject_cast<PresModel *>(mSlidesView->model());

    if (presModel != NULL) {
        mSlidesView->setCurrentIndex(presModel->getCurentIndex());
        mSlidesView->center(presModel->getCurentIndex());
    }
}

bool ProjControllerPane::eventFilter(QObject* watched, QEvent* event)
{
    QWidget *watchedWidget = qobject_cast<QWidget*>(watched);
    if (watched == NULL)
        return false;

    if ((event->type() == QEvent::HoverEnter)
     || (event->type() == QEvent::HoverMove)) {
        QHoverEvent *he = static_cast<QHoverEvent*>(event);
        QToolTip::showText(watchedWidget->mapToGlobal(he->pos()), watchedWidget->toolTip(), watchedWidget, watchedWidget->rect());
    }

    return false;
}
