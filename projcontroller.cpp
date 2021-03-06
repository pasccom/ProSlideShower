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

#include "projcontroller.h"
#include "projcontrollerpane.h"
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
    QSettings settings(this);
    if (settings.status() != QSettings::NoError)
        qWarning() << "Could not open settings. Status:" << settings.status();

    mDisplays->setParentWidget(this);

    // Presentation timing:
    QVariant totalTime = settings.value("totalTime", QVariant(QTime(0, 45, 0)));
    mTotalTime = totalTime.canConvert(QVariant::Time) ? totalTime.toTime() : QTime(0, 45, 0);
    mTime = QTime(0, 0, 0);
    mFormat = "hh:mm:ss";

    mTimer = new QTimer(this);
    mTimer->setInterval(1000);
    mTimer->setSingleShot(false);

    // Palette and font override:
    QPalette widgetPalette = palette();
    widgetPalette.setColor(QPalette::Window, Qt::black);
    widgetPalette.setColor(QPalette::WindowText, Qt::white);
    widgetPalette.setColor(QPalette::Base, QColor(32, 32, 32));
    widgetPalette.setColor(QPalette::Button, QColor(32, 32, 32));
    widgetPalette.setColor(QPalette::ButtonText, Qt::white);
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

    QAction *configAction = new QAction(QIcon(":/icons/configure.png"), tr("Configure"), this);

    // Header:
    mPane = new ProjControllerPane(this);
    mPane->setModel(model);
    mPane->hide();

    // Footer:
    mTimeLabel = new QLabel("00:00:00", this);
    mTimeLabel->setFont(widgetFont);
    mTimeLabel->addAction(configAction);
    mTimeLabel->setContextMenuPolicy(Qt::ActionsContextMenu);
    mSlideLabel = new QLabel("0/0", this);
    mSlideLabel->setFont(widgetFont);

    mTimeProgress = new QProgressBar(this);
    mTimeProgress->setRange(0, 0);
    mTimeProgress->setValue(0);

    mSlideProgress = new QProgressBar(this);
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
    mainLayout->addWidget(mPane);
    mainLayout->addWidget(mSplitter, 1);
    mainLayout->addLayout(bottomLayout);

    setLayout(mainLayout);

    connect(mTimer, SIGNAL(timeout()),
            this, SLOT(handleSecondStep()));
    connect(mPane, SIGNAL(openDocumentRequest()),
            mDisplays, SLOT(load()));
    connect(mPane, SIGNAL(configureRequest()),
            this, SLOT(configure()));
    connect(configAction, SIGNAL(triggered()),
            this, SLOT(configure()));

    if (model != NULL) {
        connect(model, SIGNAL(documentChanged()),
                this, SLOT(handleDocumentChange()));
        connect(model, SIGNAL(currentFrameChanged()),
                this, SLOT(handleFrameChange()));
    }
}

ProjController::~ProjController(void)
{
    QSettings settings(this);
    if (settings.status() != QSettings::NoError)
        qWarning() << "Could not open settings. Status:" << settings.status();
    settings.setValue("totalTime", mTotalTime);
}

void ProjController::setModel(PresModel* model)
{
    if (this->model() != NULL) {
        disconnect(this->model(), SIGNAL(documentChanged()),
                this, SLOT(handleDocumentChange()));
        disconnect(this->model(), SIGNAL(currentFrameChanged()),
                this, SLOT(handleFrameChange()));
    }

    mDisplays->setModel(model);
    mPane->setModel(model);

    if (model != NULL) {
        connect(model, SIGNAL(documentChanged()),
                this, SLOT(handleDocumentChange()));
        connect(model, SIGNAL(currentFrameChanged()),
                this, SLOT(handleFrameChange()));
        updateFrame();
    }
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

    if (mDisplays->model()->getFrameCount() != 0)
        mSlideLabel->setText(QString("%1/%2").arg(mDisplays->model()->getCurrentFrameNumber()).arg(mDisplays->model()->getFrameCount()));
    else
        mSlideLabel->setText("");

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

void ProjController::configure(void)
{
    QDialog configDialog(this);
    configDialog.setModal(true);
    configDialog.setWindowTitle(tr("Presentation configuration"));

    QTimeEdit totalTimeEdit(mTotalTime, &configDialog);
    QLabel totalTimeLabel(tr("Presentation duration:"), &configDialog);
    totalTimeLabel.setBuddy(&totalTimeEdit);

    QDialogButtonBox configButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &configDialog);
    connect(&configButtons, SIGNAL(accepted()), &configDialog, SLOT(accept()));
    connect(&configButtons, SIGNAL(rejected()), &configDialog, SLOT(reject()));

    QGridLayout configLayout;
    configLayout.setContentsMargins(2, 2, 2, 2);
    configLayout.setSpacing(2);
    configLayout.addWidget(&totalTimeLabel, 0, 0, Qt::AlignLeft);
    configLayout.addWidget(&totalTimeEdit, 0, 1);
    configLayout.addWidget(&configButtons, 1, 0, 1, 2, Qt::AlignRight);
    configDialog.setLayout(&configLayout);

    int res = configDialog.exec();

    if (res == QDialog::Rejected)
        return;

    mTotalTime = totalTimeEdit.time();
    updateTime();
}
