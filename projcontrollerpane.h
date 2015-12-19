#ifndef PROJCONTROLLERPANE_H
#define PROJCONTROLLERPANE_H

#include <QWidget>
#include "galleryview.h"

class QToolButton;

class ProjControllerPane : public QWidget
{
    Q_OBJECT
public:
    ProjControllerPane(QWidget* parent = NULL);

    inline void setModel(PreviewModel *model) {mSlidesView->setModel(model);}
    inline QAbstractItemModel* model(void) const {return mSlidesView->model();}
signals:
    void openDocumentRequest(void);
protected:
    bool eventFilter(QObject* watched, QEvent* event);
private:
    GalleryView* mSlidesView;
    QToolButton* mOpenButton;
    QToolButton* mConfigButton;
    QToolButton* mCloseButton;
};

#endif // PROJCONTROLLERPANE_H
