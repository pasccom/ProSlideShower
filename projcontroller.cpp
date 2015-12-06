#include "projcontroller.h"
#include "presmodel.h"
#include "presstyle.h"
#include "projdisplay.h"

#if QT_VERSION >= 0x050000
#   include <QtWidgets>
#else
#   include <QtGui>
#endif

ProjController::ProjController(PresModel *model, QWidget *parent) :
    QWidget(parent, Qt::FramelessWindowHint), mDisplays(new SubDisplayHandler(model, parent))
{
    mDisplays->setParentWidget(this);

    // Presentation timing:
    mTime = QTime(0, 0, 0);
    mFormat = "hh:mm:ss";

    mTimer = new QTimer(this);
    mTimer->setInterval(1000);
    mTimer->setSingleShot(false);

    // Style, palette and font override:
    mStyle = new PresStyle();

    QPalette widgetPalette = palette();
    widgetPalette.setColor(QPalette::Window, Qt::black);
    widgetPalette.setColor(QPalette::WindowText, Qt::white);
    widgetPalette.setColor(QPalette::NoRole, Qt::white);
    setPalette(widgetPalette);

    QFont widgetFont = font();
    widgetFont.setPointSize(32);

    // Main display:
    mSplitter = new QSplitter(Qt::Horizontal, this);

    mDisplays->resize(3);

    for (int d = 0; d < mDisplays->size(); d++) {
        mDisplays->replace(d, new ProjDisplay(model, this));
        mDisplays->at(d)->setOffset(d - 1);
        mSplitter->addWidget(mDisplays->at(d));
    }

    // Footer:
    mTimeLabel = new QLabel("00:00:00", this);
    mTimeLabel->setFont(widgetFont);
    mSlideLabel = new QLabel("0/0", this);
    mSlideLabel->setFont(widgetFont);

    mTimeProgress = new QProgressBar(this);
    mTimeProgress->setStyle(mStyle);
    mTimeProgress->setRange(0, 0);
    mTimeProgress->setValue(0);

    mSlideProgress = new QProgressBar(this);
    mSlideProgress->setStyle(mStyle);
    mSlideProgress->setRange(0, 0);
    mSlideProgress->setValue(0);

    // Layout:
    QBoxLayout* progressLayout = new QVBoxLayout;
    progressLayout->addWidget(mSlideProgress, 1, Qt::AlignVCenter);
    progressLayout->addWidget(mTimeProgress, 1, Qt::AlignVCenter);

    QBoxLayout* bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(mTimeLabel);
    bottomLayout->addLayout(progressLayout, 1);
    bottomLayout->addWidget(mSlideLabel);

    QBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(mSplitter, 1);
    mainLayout->addLayout(bottomLayout);

    setLayout(mainLayout);

    handleFrameChange();
    mDisplays->updateDisplayActions();

    connect(mTimer, SIGNAL(timeout()),
            this, SLOT(handleSecondStep()));
    connect(model, SIGNAL(documentChanged()),
            this, SLOT(handleDocumentChange()));
    connect(model, SIGNAL(currentFrameChanged()),
            this, SLOT(handleFrameChange()));
}

void ProjController::stop(void)
{
    if (mTimer->isActive())
        mTimer->stop();

    mTime = QTime(0, 0, 0);
    updateTime();
}


void ProjController::handleDocumentChange(void)
{
    updateFrame();
    //updateDisplays();
}

void ProjController::handleFrameChange(void)
{
    updateFrame();
    //updateDisplays();
}

void ProjController::updateDisplays(void)
{
    for (int d = 0; d < mDisplays->size(); d++)
        mDisplays->at(d)->repaint();
}


void ProjController::updateTime(void)
{
    mTimeProgress->setRange(0, -mTotalTime.secsTo(QTime(0, 0, 0)));
    mTimeProgress->setValue(-mTime.secsTo(QTime(0, 0, 0)));

    if (mFormat.contains('s', Qt::CaseSensitive) || (mTime.second() == 0))
        mTimeLabel->setText(mTime.toString(mFormat));

    updateColor();
}

void ProjController::updateFrame(void)
{
    mSlideProgress->setRange(1, mDisplays->model()->getFrameCount());
    mSlideProgress->setValue(mDisplays->model()->getCurrentFrameNumber());

    mSlideLabel->setText(QString("%1/%2").arg(mDisplays->model()->getCurrentFrameNumber()).arg(mDisplays->model()->getFrameCount()));

    updateColor();
}

void ProjController::updateColor(void)
{
    qreal timePercent = 100. * qMax(0, mTimeProgress->value() - mTimeProgress->minimum()) / qMax(1, mTimeProgress->maximum() - mTimeProgress->minimum());
    qreal slidePercent = 100. * qMax(0, mSlideProgress->value() - mSlideProgress->minimum()) / qMax(1, mSlideProgress->maximum() - mSlideProgress->minimum());
    qreal delta = timePercent - slidePercent;

    QPalette widgetPalette = palette();

    if (delta >= 25)
        widgetPalette.setColor(QPalette::NoRole, Qt::red);
    else if (delta <= -25)
        widgetPalette.setColor(QPalette::NoRole, Qt::blue);
    else if (delta >= 15)
        widgetPalette.setColor(QPalette::NoRole, QColor::fromHsvF((25. - delta)/60., 1., 1.));
    else if (delta <= -15)
        widgetPalette.setColor(QPalette::NoRole, QColor::fromHsvF((-delta - 5.)/30., 1., 1.));
    else if (delta >= 5)
        widgetPalette.setColor(QPalette::NoRole, QColor::fromHsvF(1./6., (delta - 5.)/10., 1.));
    else if (delta <= -5)
        widgetPalette.setColor(QPalette::NoRole, QColor::fromHsvF(1./3., (-delta - 5.)/10., 1.));
    else
        widgetPalette.setColor(QPalette::NoRole, Qt::white);

    setPalette(widgetPalette);
    mTimeProgress->repaint();
    mSlideProgress->repaint();
}

