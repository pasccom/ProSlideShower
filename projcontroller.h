#ifndef PROJCONTROLLER_H
#define PROJCONTROLLER_H

#include <QWidget>
#include "subdisplayhandler.h"
#include "presstyle.h"
#include <QTimer>
#include <QTime>

class PresModel;
class ProjDisplay;
class QSplitter;
class QLabel;
class QProgressBar;

class ProjController : public QWidget
{
    Q_OBJECT
public:
    ProjController(PresModel *model, QWidget *parent = NULL);
    ~ProjController(void) {delete mStyle;}

    inline void setModel(PresModel* model) {mDisplays->setModel(model);}
    inline PresModel* model() const {return mDisplays->model();}

    inline void setTimeFormat(const QString& format) {mFormat = format; updateTime();}
    inline QString timeFormat(void) const {return mFormat;}

    inline void setTotalTime(const QTime& length) {mTotalTime = length; updateTime();}
    inline QTime totalTime(void) const {return mTotalTime;}
signals:

public slots:
    void goToNextPage() {mDisplays->goToNextPage();}
    void goToPrevPage() {mDisplays->goToPrevPage();}

    void handleDocumentChange(void);
    void handleFrameChange(void);

    inline void start(void) {if (!mTimer->isActive()) mTimer->start();}
    inline void pause(void) {if (mTimer->isActive()) mTimer->stop();}
    void stop(void);
private slots:
    inline void handleSecondStep(void) {mTime = mTime.addSecs(1); updateTime();}
private:
    void updateDisplays(void);
    void updateTime(void);
    void updateFrame(void);
    void updateColor(void);

    SubDisplayHandler *mDisplays;
    PresStyle* mStyle;
    QSplitter* mSplitter;
    QLabel* mTimeLabel;
    QLabel* mSlideLabel;
    QProgressBar* mSlideProgress;
    QProgressBar* mTimeProgress;

    QTimer* mTimer;
    QTime mTime;
    QTime mTotalTime;
    QString mFormat;
};

#endif // PROJCONTROLLER_H
