#ifndef PROJCONTROLLER_H
#define PROJCONTROLLER_H

#include <QWidget>
#include "presstyle.h"
#include <QVector>
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

    inline void setTimeFormat(const QString& format) {mFormat = format; updateTime();}
    inline QString timeFormat(void) const {return mFormat;}

    inline void setTotalTime(const QTime& length) {mTotalTime = length; updateTime();}
    inline QTime totalTime(void) const {return mTotalTime;}
signals:

public slots:
    void handleSlideChange(void);
    inline void start(void) {if (!mTimer->isActive()) mTimer->start();}
    inline void pause(void) {if (mTimer->isActive()) mTimer->stop();}
    void stop(void);
private slots:
    inline void handleSecondStep(void) {mTime = mTime.addSecs(1); updateTime();}
private:
    void updateTime(void);
    void updateSlide(void);
    void updateColor(void);

    PresModel* mModel;
    PresStyle* mStyle;
    QSplitter* mSplitter;
    QLabel* mTimeLabel;
    QLabel* mSlideLabel;
    QProgressBar* mSlideProgress;
    QProgressBar* mTimeProgress;
    QVector<ProjDisplay*> mDisplays;

    QTimer* mTimer;
    QTime mTime;
    QTime mTotalTime;
    QString mFormat;
};

#endif // PROJCONTROLLER_H
