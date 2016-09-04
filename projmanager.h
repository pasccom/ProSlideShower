#ifndef PROJMANAGER_H
#define PROJMANAGER_H

#include "subdisplayhandler.h"

class ProjController;
class QKeyEvent;
class QMouseEvent;

class ProjManager : public SubDisplayHandler
{
    Q_OBJECT
public:
    ProjManager(QObject *parent = NULL);
    ~ProjManager(void);
protected:
    bool eventFilter(QObject* watched, QEvent* event);
    void keyPressEvent(QKeyEvent *ke);
    void keyReleaseEvent(QKeyEvent* ke);
    void mouseMoveEvent(QMouseEvent *me);
private:

    ProjController* mController;
};

#endif // PROJMANAGER_H
