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
signals:

public slots:
    void load(const QString& file, int h = 0, int v = 1);
protected:
    bool eventFilter(QObject* watched, QEvent* event);
    void keyReleaseEvent(QKeyEvent* ke);
    void mouseMoveEvent(QMouseEvent *me);
private:

    ProjController* mController;
};

#endif // PROJMANAGER_H
