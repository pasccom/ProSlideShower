#ifndef PROJMANAGER_H
#define PROJMANAGER_H

#include <QObject>
#include <QVector>
#include "presmodel.h"

class PresModel;
class ProjController;
class ProjDisplay;
class QKeyEvent;

class ProjManager : public QObject
{
    Q_OBJECT
public:
    ProjManager(QObject *parent = NULL);
    ~ProjManager(void);
signals:

public slots:
    inline void load(const QString& file) {mModel->load(file);}
protected:
    bool eventFilter(QObject* watched, QEvent* event);
    void keyReleaseEvent(QKeyEvent *ke);
private slots:
    void handleLoadFile(void);
private:
    PresModel* mModel;
    ProjController* mController;
    QVector<ProjDisplay*> mDisplays;
};

#endif // PROJMANAGER_H
