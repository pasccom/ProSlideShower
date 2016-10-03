/* Copyright 2016 Pascal COMBES <pascom@orange.fr>
 * 
 * This file is part of ProSlideShower.
 * 
 * ProSlideShower is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ProSlideShower is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with ProSlideShower. If not, see <http://www.gnu.org/licenses/>
 */

#ifndef GALLERYVIEW_H
#define GALLERYVIEW_H

//#include <QAbstractScrollArea>
#include <QAbstractItemView>
#include <QModelIndex>
#include <QScrollBar>
#include <QPixmap>
#include <QVector>

class PreviewModel;
class QItemSelectionModel;

class Dimension
{
public:
    inline Dimension(void) :
        mPixels(0), mPercents(0) {}
    enum Unit {PERCENT, PIXEL};
    inline Dimension(int pixels, int percents = 0) :
        mPixels(pixels), mPercents(percents) {}
    inline Dimension(int dim, Unit unit) :
        mPixels(unit == PIXEL ? dim : 0), mPercents(unit == PERCENT ? dim : 0) {}
    inline Dimension(const Dimension &other) :
        mPixels(other.mPixels), mPercents(other.mPercents) {}

    inline Dimension &operator=(const Dimension &other)
    {
        mPixels = other.mPixels;
        mPercents = other.mPercents;
        return *this;
    }

    inline Dimension &operator+=(const Dimension &other)
    {
        mPixels+=other.mPixels;
        mPercents+=other.mPercents;
        return *this;
    }

    inline Dimension &operator-=(const Dimension &other)
    {
        mPixels-=other.mPixels;
        mPercents-=other.mPercents;
        return *this;
    }

    inline Dimension &operator*=(int factor)
    {
        mPixels*=factor;
        mPercents*=factor;
        return *this;
    }

    inline bool isEmpty(void) const {return (mPixels == 0) || (mPercents == 0);}
    inline bool isNull(void) const {return (mPixels == 0) && (mPercents == 0);}
    inline bool isValid(void) const {return (mPixels >= 0) && (mPercents >= 0);}

    inline int pixels(void) const {return mPixels;}
    inline unsigned int upixels(void) const {return qMax(0, mPixels);}
    inline int percents(void) const {return mPercents;}
    inline unsigned int upercents(void) const {return qMax(0, mPercents);}
    inline int dimension(Unit unit) const {return unit == PERCENT ? mPercents : mPixels;}
    inline int toPixels(int dim) const
    {
        return mPixels + (dim * mPercents) / 100.;
    }
    inline int fromPixels(int total) const
    {
        return (total - mPixels)*100. / (100. + mPercents);
    }
private:
    int mPixels;
    int mPercents;

    friend const Dimension operator+(const Dimension &dim1, const Dimension &dim2);
    friend const Dimension operator-(const Dimension &dim1, const Dimension &dim2);
    friend const Dimension operator*(const Dimension &dim, int factor);
    friend const Dimension operator*(int factor, const Dimension &dim);
    friend QDebug operator<<(QDebug debug, const Dimension &dim);
};


inline const Dimension operator+(const Dimension &dim1, const Dimension &dim2)
{
    return Dimension(dim1.mPixels + dim2.mPixels, dim2.mPercents + dim1.mPercents);
}

inline const Dimension operator-(const Dimension &dim1, const Dimension &dim2)
{
    return Dimension(dim1.mPixels - dim2.mPixels, dim2.mPercents - dim1.mPercents);
}

inline const Dimension operator*(const Dimension &dim, int factor)
{
    return Dimension(dim.mPixels * factor, dim.mPercents * factor);
}

inline const Dimension operator*(int factor, const Dimension &dim)
{
    return Dimension(dim.mPixels * factor, dim.mPercents * factor);
}

QDebug &operator<<(QDebug &debug, const Dimension &dim);

//class GalleryView : public QAbstractScrollArea
class GalleryView : public QAbstractItemView
{
    Q_OBJECT
public:
    //enum SelectionMode {NoSelection, SingleSelection, MultiSelection, ExtendedSelection, ContiguousSelection};

    GalleryView(Qt::Orientation orientation = Qt::Horizontal, QWidget *parent = 0);
    
    void setOrientation(Qt::Orientation orientation);
    inline Qt::Orientation orientation(void) const {return mOrientation;}

    inline void setAlignement(Qt::Alignment align)
    {
        mAlignment = align;
        update();
    }
    inline Qt::Alignment alignement(void) const {return mAlignment;}

    void setModel(QAbstractItemModel *model);
    void setModel(PreviewModel *model);
    inline QAbstractItemModel *model(void) const {return (QAbstractItemModel *) mModel;}

    //inline void setSelectionModel(QItemSelectionModel *model) {mSelectionModel = model;}
    //inline QItemSelectionModel *selectionModel(void) const {return mSelectionModel;}

    bool isPartiallyVisible(const QModelIndex &index, bool includePadding = false) const;
    bool isTotallyVisible(const QModelIndex &index, bool includePadding = false) const;

    void ensureTotallyVisible(const QModelIndex &index,
                              bool includePadding = false,
                              ScrollHint hint = EnsureVisible);
    inline void center(const QModelIndex &index)
    {
        ensureTotallyVisible(index, false, PositionAtCenter);
    }
    inline void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible)
    {
        ensureTotallyVisible(index, true, hint);
    }

    inline QModelIndex indexAt(const QPoint &screenPoint) const {return indexAt(screenPoint, true);}
    QModelIndex indexAt(const QPoint& screenPoint, bool includePadding) const;

    QRect visualRect(const QModelIndex &index, bool includePadding) const;
    inline QRect visualRect(const QModelIndex &index) const {return visualRect(index, true);}

    inline void setBottomPadding(unsigned int pixels, unsigned int percents = 0)
    {
        mBottomPadding = Dimension(pixels, percents);
    }
    inline void setTopPadding(unsigned int pixels, unsigned int percents = 0)
    {
        mTopPadding = Dimension(pixels, percents);
    }
    inline void setLeftPadding(unsigned int pixels, unsigned int percents = 0)
    {
        mLeftPadding = Dimension(pixels, percents);
    }
    inline void setRightPadding(unsigned int pixels, unsigned int percents = 0)
    {
        mRightPadding = Dimension(pixels, percents);
    }

    inline void setBottomMargin(unsigned int pixels, unsigned int percents = 0)
    {
        mBottomMargin = Dimension(pixels, percents);
    }
    inline void setTopMargin(unsigned int pixels, unsigned int percents = 0)
    {
        mTopMargin = Dimension(pixels, percents);
    }
    inline void setLeftMargin(unsigned int pixels, unsigned int percents = 0)
    {
        mLeftMargin = Dimension(pixels, percents);
    }
    inline void setRightMargin(unsigned int pixels, unsigned int percents = 0)
    {
        mRightMargin = Dimension(pixels, percents);
    }
    inline void setSpacing(unsigned int pixels, unsigned int percents = 0)
    {
        mSpacing = Dimension(pixels, percents);
    }

signals:
    
public slots:
    void reset(void);
protected:
    int horizontalOffset(void) const;
    int verticalOffset(void) const;

    QModelIndex moveCursor(CursorAction cursorAction,
                                   Qt::KeyboardModifiers modifiers);

    inline bool isIndexHidden(const QModelIndex &index, bool includePadding) const
    {
        return !isPartiallyVisible(index, includePadding);
    }
    inline bool isIndexHidden(const QModelIndex &index) const
    {
        return !isPartiallyVisible(index, false);
    }

    void setSelection(const QRect &screenRect,
                      bool includePadding,
                      QItemSelectionModel::SelectionFlags command);
    inline void setSelection(const QRect &screenRect, QItemSelectionModel::SelectionFlags command)
    {
        setSelection(screenRect, true, command);
    }


    QRegion visualRegionForSelection(const QItemSelection &selection, bool includePadding) const;
    inline QRegion visualRegionForSelection(const QItemSelection &selection) const
    {
        return visualRegionForSelection(selection, true);
    }

    void paintEvent(QPaintEvent *pe);
    void resizeEvent(QResizeEvent *re);
    void keyPressEvent(QKeyEvent *ke);
    /*void mousePressEvent(QMouseEvent *me);
    void mouseMoveEvent(QMouseEvent *me);
    void mouseReleaseEvent(QMouseEvent *me);*/
private slots:
    void scrollAlong(int action);
private:
    QRect fromViewportCoodinates(const QRect& rect) const;
    QPoint fromViewportCoodinates(const QPoint& point) const;
    QRect toViewportCoodinates(const QRect& rect) const;
    QPoint toViewportCoodinates(const QPoint& point) const;

    /*enum Direction {POSITIVE = 1, NEGATIVE = -1};*/

    void ensureFirstIndexVisible(void);
    void ensureLastIndexVisible(void);

    /*void activeItemTowards(Direction dir);
    void expandSelectionTowards(Direction dir, bool add = false);*/

    void updateAspectRatioList(void);
    inline void resetScrollBars (void)
    {
        alongScrollBar()->setMinimum(0);
        alongScrollBar()->setMaximum(0);
        disconnect(alongScrollBar(), SIGNAL(actionTriggered(int)));
    }
    void updateScrollBars(void);

    unsigned int computeConstraint(bool scrollBar) const;
    qreal computeTotal(unsigned int constraint) const;
    unsigned int computeAlernative(void) const;

    QSize viewportSize(bool scrolBar);

    inline unsigned int getPosition(void) const
    {
        return cTotal <= (unsigned int) alongDimension(maximumViewportSize()) ?
                    0 :
                    alongScrollBar()->value();
        /*return qMin(cTotal - alongDimension(maximumViewportSize()),
                    (unsigned int) alongScrollBar()->value() * SLIDER_SINGLE_STEP);*/
    }
    inline QScrollBar *alongScrollBar(void) const
    {
        return mOrientation == Qt::Vertical ? verticalScrollBar() : horizontalScrollBar();
    }
    inline QScrollBar *acrossScrollBar(void) const
    {
        return mOrientation == Qt::Vertical ? horizontalScrollBar() : verticalScrollBar();
    }

    inline Dimension firstAlongPadding(void) const
    {
        return mOrientation == Qt::Vertical ? mTopPadding : mLeftPadding;
    }
    inline Dimension secondAlongPadding(void) const
    {
        return mOrientation == Qt::Vertical ? mBottomPadding : mRightPadding;
    }
    inline Dimension firstAcrossPadding(void) const
    {
        return mOrientation == Qt::Vertical ? mLeftPadding : mTopPadding;
    }
    inline Dimension secondAcrossPadding(void) const
    {
        return mOrientation == Qt::Vertical ? mRightPadding : mBottomPadding;
    }

    inline Dimension firstAlongMargin(void) const
    {
        return mOrientation == Qt::Vertical ? mTopMargin : mLeftMargin;
    }
    inline Dimension secondAlongMargin(void) const
    {
        return mOrientation == Qt::Vertical ? mBottomMargin : mRightMargin;
    }
    inline Dimension firstAcrossMargin(void) const
    {
        return mOrientation == Qt::Vertical ? mLeftMargin : mTopMargin;
    }
    inline Dimension secondAcrossMargin(void) const
    {
        return mOrientation == Qt::Vertical ? mRightMargin : mBottomMargin;
    }

    inline int accrossDimension(const QSize &size) const
    {
        return mOrientation == Qt::Vertical ? size.width() : size.height();
    }
    inline int accrossDimension(const QPoint &point) const
    {
        return mOrientation == Qt::Vertical ? point.x() : point.y();
    }
    inline int alongDimension(const QSize &size) const
    {
        return mOrientation == Qt::Vertical ? size.height() : size.width();
    }
    inline int alongDimension(const QPoint &point) const
    {
        return mOrientation == Qt::Vertical ? point.y() : point.x();
    }
    inline int &raccrossDimension(QSize &size) const
    {
        return mOrientation == Qt::Vertical ? size.rwidth() : size.rheight();
    }
    inline int &raccrossDimension(QPoint &point) const
    {
        return mOrientation == Qt::Vertical ? point.rx() : point.ry();
    }
    inline int &ralongDimension(QSize &size) const
    {
        return mOrientation == Qt::Vertical ? size.rheight() : size.rwidth();
    }
    inline int &ralongDimension(QPoint &point) const
    {
        return mOrientation == Qt::Vertical ? point.ry() : point.rx();
    }

    inline int alongAddKey(void) const
    {
        return mOrientation == Qt::Vertical ? Qt::Key_Down : Qt::Key_Right;
    }
    inline int alongSubKey(void) const
    {
        return mOrientation == Qt::Vertical ? Qt::Key_Up : Qt::Key_Left;
    }
    inline int accrossAddKey(void) const
    {
        return mOrientation == Qt::Vertical ? Qt::Key_Right : Qt::Key_Down;
    }
    inline int accrossSubKey(void) const
    {
        return mOrientation == Qt::Vertical ? Qt::Key_Left : Qt::Key_Up;
    }

    inline int alongAddDirection(void) const
    {
        return mOrientation == Qt::Vertical ?
                    QAbstractItemView::MoveDown :
                    QAbstractItemView::MoveRight;
    }
    inline int alongSubDirection(void) const
    {
        return mOrientation == Qt::Vertical ?
                    QAbstractItemView::MoveUp :
                    QAbstractItemView::MoveLeft;
    }
    inline int accrossAddDirection(void) const
    {
        return mOrientation == Qt::Vertical ?
                    QAbstractItemView::MoveRight :
                    QAbstractItemView::MoveDown;
    }
    inline int accrossSubDirection(void) const
    {
        return mOrientation == Qt::Vertical ?
                    QAbstractItemView::MoveLeft :
                    QAbstractItemView::MoveUp;
    }


    PreviewModel *mModel;
    //QItemSelectionModel *mSelectionModel;
    Qt::Alignment mAlignment;
    Qt::Orientation mOrientation;

    QVector<qreal> mAspectRatioList;
    QVector<QImage> cPreviews;
    unsigned int cConstraint;
    unsigned int cAcross;
    unsigned int cTotal;

    Dimension mBottomPadding;
    Dimension mTopPadding;
    Dimension mLeftPadding;
    Dimension mRightPadding;

    Dimension mBottomMargin;
    Dimension mTopMargin;
    Dimension mLeftMargin;
    Dimension mRightMargin;

    Dimension mSpacing;

    static const unsigned int SLIDER_SINGLE_STEP = 10;
};

#endif // GALLERYVIEW_H
