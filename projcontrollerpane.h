#ifndef PROJCONTROLLERPANE_H
#define PROJCONTROLLERPANE_H

#include <QWidget>

class QListWidget;
class QToolButton;

class ProjControllerPane : public QWidget
{
    Q_OBJECT
public:
    ProjControllerPane(QWidget* parent = NULL);
signals:
    void openDocumentRequest(void);
protected:
    bool eventFilter(QObject* watched, QEvent* event);
private:
    QListWidget* mSlidesView;
    QToolButton* mOpenButton;
    QToolButton* mConfigButton;
    QToolButton* mCloseButton;
};

#endif // PROJCONTROLLERPANE_H
