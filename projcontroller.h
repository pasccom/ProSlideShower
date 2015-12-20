#ifndef PROJCONTROLLER_H
#define PROJCONTROLLER_H

#include <QWidget>
#include "subdisplayhandler.h"
#include "presstyle.h"
#include "projcontrollerpane.h"
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

    inline void setPaneHeight(int height) {mPane->setFixedHeight(height);}
    inline int paneHeight(void) const {return mPane->height();}

    inline bool isPaneShown(void) const {return mPane->isVisible();}
signals:
    void documentOpened(const QString& file);
public slots:
    void goToNextPage() {mDisplays->goToNextPage();}
    void goToPrevPage() {mDisplays->goToPrevPage();}

    inline void start(void) {if (!mTimer->isActive()) mTimer->start();}
    inline void pause(void) {if (mTimer->isActive()) mTimer->stop();}
    void stop(void);

    void showControlPane(void) {mPane->show();}
    void hideControlPane(void) {mPane->hide();}
private slots:
    inline void handleSecondStep(void) {mTime = mTime.addSecs(1); updateTime();}
    void handleLoadFile(void);
    void handleDocumentChange(void);
    void handleFrameChange(void);
private:
    void updateDisplays(void);
    void updateTime(void);
    void updateFrame(void);
    void updateColor(void);

    SubDisplayHandler *mDisplays;
    PresStyle* mStyle;
    QSplitter* mSplitter;
    ProjControllerPane* mPane;
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
