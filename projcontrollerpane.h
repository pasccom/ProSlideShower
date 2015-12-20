#ifndef PROJCONTROLLERPANE_H
#define PROJCONTROLLERPANE_H

#include <QWidget>
#include "galleryview.h"

class QToolButton;
class PresModel;

class ProjControllerPane : public QWidget
{
    Q_OBJECT
public:
    ProjControllerPane(QWidget* parent = NULL);

    void setModel(PresModel *model);
    inline QAbstractItemModel* model(void) const {return mSlidesView->model();}
signals:
    void openDocumentRequest(void);
protected:
    bool eventFilter(QObject* watched, QEvent* event);
private slots:
    void handlePageChange(void);
private:
    GalleryView* mSlidesView;
    QToolButton* mOpenButton;
    QToolButton* mConfigButton;
    QToolButton* mCloseButton;
};

#endif // PROJCONTROLLERPANE_H
