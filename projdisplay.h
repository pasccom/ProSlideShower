#ifndef PROJDISPLAY_H
#define PROJDISPLAY_H

#include <QWidget>

class PresModel;

class ProjDisplay : public QWidget
{
    Q_OBJECT
public:
    ProjDisplay(PresModel* model, QWidget* parent = NULL);

    inline void setOffset(int offset) {Q_ASSERT((offset >= -1) && (offset <= 1)); mOffset = offset;}
    inline int offset(void) const {return mOffset;}

    inline int horizontalVirtualScreen(void) const {return mHVirtualScreen;}
    inline int verticalVirtualScreen(void) const {return mVVirtualScreen;}
public slots:
    void setHorizontalVirtualScreen(int horizontal = -1);
    void setVericalVirtualScreen(int vertical = -1);
protected:
    void paintEvent(QPaintEvent *pe);
    void contextMenuEvent(QContextMenuEvent *cme);
private:
    PresModel* mModel;
    int mOffset;
    int mHVirtualScreen;
    int mVVirtualScreen;
};

#endif // PROJDISPLAY_H
