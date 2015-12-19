#include "projcontrollerpane.h"
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
   mSlidesView->setTopMargin(1, 2);
   mSlidesView->setBottomMargin(1, 2);
   mSlidesView->setLeftMargin(5);
   mSlidesView->setRightMargin(5);
   mSlidesView->setSpacing(5);

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
