#ifndef PROJMANAGER_H
#define PROJMANAGER_H

#include "subdisplayhandler.h"

class ProjController;
class QKeyEvent;

class ProjManager : public SubDisplayHandler
{
    Q_OBJECT
public:
    ProjManager(QObject *parent = NULL);
    ~ProjManager(void);
signals:

public slots:
    void load(const QString& file);
protected:
    bool eventFilter(QObject* watched, QEvent* event);
    void keyReleaseEvent(QKeyEvent* ke);
private:

    ProjController* mController;
};

#endif // PROJMANAGER_H
