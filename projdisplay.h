#ifndef PROJDISPLAY_H
#define PROJDISPLAY_H

#include <QWidget>

class PresModel;

class ProjDisplay : public QWidget
{
public:
    ProjDisplay(PresModel* model, QWidget* parent = NULL);

    inline void setOffset(int offset) {Q_ASSERT((offset >= -1) && (offset <= 1)); mOffset = offset;}
    inline int offset(void) const {return mOffset;}

    inline void setHorizontalVirtualScreen(int horizontal) {Q_ASSERT(horizontal > 0); mHVirtualScreen = horizontal;}
    inline void setVericalVirtualScreen(int vertical) {Q_ASSERT(vertical > 0); mVVirtualScreen = vertical;}
    inline int horizontalVirtualScreen(void) const {return mHVirtualScreen;}
    inline int verticalVirtualScreen(void) const {return mVVirtualScreen;}
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
