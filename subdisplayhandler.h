#ifndef SUBDISPLAYHANDLER_H
#define SUBDISPLAYHANDLER_H

#include <QObject>
#include <QVector>

class PresModel;
class ProjDisplay;

class SubDisplayHandler : public QObject, public QVector<ProjDisplay *>
{
    Q_OBJECT
public:
    inline SubDisplayHandler(PresModel* model = NULL, QObject* parent = NULL):
        QObject(parent), QVector<ProjDisplay *>(), mModel(model), mParent(NULL) {}
    virtual ~SubDisplayHandler(void);

    inline void setParentWidget(QWidget* widget) {mParent = widget;}
    inline QWidget* parentWidget(void) const {return mParent;}

    void setModel(PresModel* model);
    inline PresModel* model() const {return mModel;}

    void goToNextPage();
    void goToPrevPage();

    virtual void updateDisplayActions(void);
public slots:
    virtual void load(const QString& file, int h = 0, int v = 1);
private slots:
    void handleLoadFile(void);
    void handleVirtualScreens(void);
private:
    void updateDisplayActions(ProjDisplay* display);

    PresModel* mModel;
    QWidget* mParent;
};

#endif // SUBDISPLAYHANDLER_H
