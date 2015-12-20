#include "galleryview.h"
#include "previewmodel.h"

#include <QtCore>
#if (QT_VERSION < 0x050000)
#   include <QtGui>
#else
#   include <QtWidgets>
#endif
#include <QtDebug>

QDebug operator<<(QDebug debug, const Dimension &dim)
{
    debug.nospace() << "Dimension(" << dim.mPixels << "px, " << dim.mPercents << "%)";
    return debug.space();
}


GalleryView::GalleryView(Qt::Orientation orientation, QWidget *parent) :
    QAbstractItemView(parent),
    mModel(NULL),
    //mSelectionModel(NULL),
    mAlignment(Qt::AlignHCenter | Qt::AlignVCenter),
    mOrientation(orientation)
{
    //setSelectionModel(NULL);

    mAspectRatioList.clear();
    cPreviews.clear();

    verticalScrollBar()->setSingleStep(SLIDER_SINGLE_STEP);
    horizontalScrollBar()->setSingleStep(SLIDER_SINGLE_STEP);

    connect(alongScrollBar(), SIGNAL(actionTriggered(int)), this, SLOT(scrollAlong(int)));
}

void GalleryView::setModel(QAbstractItemModel *model)
{
    PreviewModel *previewModel = qobject_cast<PreviewModel *>(model);
    Q_ASSERT(previewModel != NULL);
    setModel(previewModel);
}

void GalleryView::setModel(PreviewModel *model)
{
    if (mModel != NULL)
        disconnect(mModel, SIGNAL(modelReset()),
                   this, SLOT(reset()));
    mModel = model;
    QAbstractItemView::setModel(model);
    connect(mModel, SIGNAL(modelReset()),
            this, SLOT(reset()));

    mAspectRatioList.clear();
    viewport()->update();
}

void GalleryView::setOrientation(Qt::Orientation orientation)
{
    resetScrollBars();
    mOrientation = orientation;
    mAspectRatioList.clear();
    viewport()->update();

    connect(alongScrollBar(), SIGNAL(actionTriggered(int)), this, SLOT(scrollAlong(int)));
}

void GalleryView::reset(void)
{
    qDebug() << "View resetted.";

    mAspectRatioList.clear();

    QAbstractItemView::reset();
}

QRect GalleryView::fromViewportCoodinates(const QRect& rect) const
{
    /* Along alignment */
    unsigned int alongAlignementOffset = 0;
    if (mAlignment & Qt::AlignHCenter)
        alongAlignementOffset = qMax(0, (int) (alongDimension(maximumViewportSize()) - cTotal)) / 2;
    else if (mAlignment & Qt::AlignRight)
        alongAlignementOffset = qMax(0, (int) (alongDimension(maximumViewportSize()) - cTotal));

    /* Accross alignment */
    unsigned int accrossAlignementOffset = 0;
    if (mAlignment & Qt::AlignVCenter)
        accrossAlignementOffset = qMax((unsigned int) 0, cAcross - cConstraint -
                (firstAcrossPadding() + secondAcrossPadding() +
                 firstAcrossMargin() + secondAcrossMargin()).toPixels(cConstraint)) / 2;
    else if (mAlignment & Qt::AlignBottom)
        accrossAlignementOffset += qMax((unsigned int) 0, cAcross - cConstraint -
                (firstAcrossPadding() + secondAcrossPadding() +
                 firstAcrossMargin() + secondAcrossMargin()).toPixels(cConstraint));

    if (mOrientation == Qt::Vertical)
        return rect.translated(QPoint(accrossAlignementOffset,
                                      alongAlignementOffset - getPosition()));
    else
        return rect.translated(QPoint(alongAlignementOffset - getPosition(),
                                      accrossAlignementOffset));
}

QPoint GalleryView::fromViewportCoodinates(const QPoint& point) const
{
    /* Along alignment */
    unsigned int alongAlignementOffset = 0;
    if (mAlignment & Qt::AlignHCenter)
        alongAlignementOffset = qMax(0, (int) (alongDimension(maximumViewportSize()) - cTotal)) / 2;
    else if (mAlignment & Qt::AlignRight)
        alongAlignementOffset = qMax(0, (int) (alongDimension(maximumViewportSize()) - cTotal));

    /* Accross alignment */
    unsigned int accrossAlignementOffset = 0;
    if (mAlignment & Qt::AlignVCenter)
        accrossAlignementOffset = qMax((unsigned int) 0, cAcross - cConstraint -
                (firstAcrossPadding() + secondAcrossPadding() +
                 firstAcrossMargin() + secondAcrossMargin()).toPixels(cConstraint)) / 2;
    else if (mAlignment & Qt::AlignBottom)
        accrossAlignementOffset += qMax((unsigned int) 0, cAcross - cConstraint -
                (firstAcrossPadding() + secondAcrossPadding() +
                 firstAcrossMargin() + secondAcrossMargin()).toPixels(cConstraint));

    if (mOrientation == Qt::Vertical)
        return point + QPoint(accrossAlignementOffset,
                              alongAlignementOffset - getPosition());
    else
        return point + QPoint(alongAlignementOffset - getPosition(),
                              accrossAlignementOffset);
}

QRect GalleryView::toViewportCoodinates(const QRect& rect) const
{
    /* Along alignment */
    unsigned int alongAlignementOffset = 0;
    if (mAlignment & Qt::AlignHCenter)
        alongAlignementOffset = qMax(0, (int) (alongDimension(maximumViewportSize()) - cTotal)) / 2;
    else if (mAlignment & Qt::AlignRight)
        alongAlignementOffset = qMax(0, (int) (alongDimension(maximumViewportSize()) - cTotal));

    /* Accross alignment */
    unsigned int accrossAlignementOffset = 0;
    if (mAlignment & Qt::AlignVCenter)
        accrossAlignementOffset = qMax((unsigned int) 0, cAcross - cConstraint -
                (firstAcrossPadding() + secondAcrossPadding() +
                 firstAcrossMargin() + secondAcrossMargin()).toPixels(cConstraint)) / 2;
    else if (mAlignment & Qt::AlignBottom)
        accrossAlignementOffset += qMax((unsigned int) 0, cAcross - cConstraint -
                (firstAcrossPadding() + secondAcrossPadding() +
                 firstAcrossMargin() + secondAcrossMargin()).toPixels(cConstraint));

    if (mOrientation == Qt::Vertical)
        return rect.translated(QPoint(-accrossAlignementOffset,
                                      -alongAlignementOffset + getPosition()));
    else
        return rect.translated(QPoint(-alongAlignementOffset + getPosition(),
                                      -accrossAlignementOffset));
}

QPoint GalleryView::toViewportCoodinates(const QPoint& point) const
{
    /* Along alignment */
    unsigned int alongAlignementOffset = 0;
    if (mAlignment & Qt::AlignHCenter)
        alongAlignementOffset = qMax(0, (int) (alongDimension(maximumViewportSize()) - cTotal)) / 2;
    else if (mAlignment & Qt::AlignRight)
        alongAlignementOffset = qMax(0, (int) (alongDimension(maximumViewportSize()) - cTotal));

    /* Accross alignment */
    unsigned int accrossAlignementOffset = 0;
    if (mAlignment & Qt::AlignVCenter)
        accrossAlignementOffset = qMax((unsigned int) 0, cAcross - cConstraint -
                (firstAcrossPadding() + secondAcrossPadding() +
                 firstAcrossMargin() + secondAcrossMargin()).toPixels(cConstraint)) / 2;
    else if (mAlignment & Qt::AlignBottom)
        accrossAlignementOffset += qMax((unsigned int) 0, cAcross - cConstraint -
                (firstAcrossPadding() + secondAcrossPadding() +
                 firstAcrossMargin() + secondAcrossMargin()).toPixels(cConstraint));

    if (mOrientation == Qt::Vertical)
        return point + QPoint(-accrossAlignementOffset,
                              -alongAlignementOffset + getPosition());
    else
        return point + QPoint(-alongAlignementOffset + getPosition(),
                              -accrossAlignementOffset);
}

bool GalleryView::isPartiallyVisible(const QModelIndex &index, bool includePadding) const
{
    if (!index.isValid())
        return false;
    if (cTotal < (unsigned int) alongDimension(maximumViewportSize()))
        return true; /* This means everything is displayed */

    unsigned int viewPixelMin = getPosition(),
                 viewPixelMax = getPosition() + alongDimension((maximumViewportSize()));

    unsigned int indexPixelMin = (firstAlongMargin()
            + (includePadding ? index.row() : index.row() + 1) * firstAlongPadding()
            + index.row() * secondAlongPadding()
            + index.row() * mSpacing).toPixels(cConstraint)
            + mAspectRatioList.at(mModel->rowCount() - index.row()) * cConstraint;
    unsigned int indexPixelMax = (firstAlongMargin()
            + (index.row() + 1) * firstAlongPadding()
            + (includePadding ? index.row() + 1 : index.row()) *secondAlongPadding()
            + index.row() * mSpacing).toPixels(cConstraint)
            + mAspectRatioList.at(mModel->rowCount() - index.row() - 1) * cConstraint;

    return ((viewPixelMin <= indexPixelMin) && (viewPixelMax >= indexPixelMin))
        || ((viewPixelMin <= indexPixelMax) && (viewPixelMax >= indexPixelMax));
}


bool GalleryView::isTotallyVisible(const QModelIndex &index, bool includePadding) const
{
    if (!index.isValid())
        return false;
    if (cTotal < (unsigned int) alongDimension(maximumViewportSize()))
        return true; /* This means everythig is displayed */

    unsigned int viewPixelMin = getPosition(),
                 viewPixelMax = getPosition() + alongDimension((maximumViewportSize()));

    unsigned int indexPixelMin = (firstAlongMargin()
            + (includePadding ? index.row() : index.row() + 1) * firstAlongPadding()
            + index.row() * secondAlongPadding()
            + index.row() * mSpacing).toPixels(cConstraint)
            + mAspectRatioList.at(mModel->rowCount() - index.row()) * cConstraint;
    unsigned int indexPixelMax = (firstAlongMargin()
            + (index.row() + 1) * firstAlongPadding()
            + (includePadding ? index.row() + 1 : index.row()) *secondAlongPadding()
            + index.row() * mSpacing).toPixels(cConstraint)
            + mAspectRatioList.at(mModel->rowCount() - index.row() - 1) * cConstraint;

    qDebug() << "Is totally visible?"
             << viewPixelMin << viewPixelMax
             << indexPixelMin << indexPixelMax;

    return (viewPixelMin <= indexPixelMin) && (viewPixelMax >= indexPixelMax);
}

void GalleryView::ensureTotallyVisible(const QModelIndex &index, bool includePadding, ScrollHint hint)
{
    if (!index.isValid())
        return;
    if (cTotal < (unsigned int) alongDimension(maximumViewportSize()))
        return; /* This means everything is displayed */

    unsigned int indexPixelMin = (firstAlongMargin()
            + (includePadding ? index.row() : index.row() + 1) * firstAlongPadding()
            + index.row() * secondAlongPadding()
            + index.row() * mSpacing).toPixels(cConstraint)
            + mAspectRatioList.at(mModel->rowCount() - index.row()) * cConstraint;
    unsigned int indexPixelMax = (firstAlongMargin()
            + (index.row() + 1) * firstAlongPadding()
            + (includePadding ? index.row() + 1 : index.row()) *secondAlongPadding()
            + index.row() * mSpacing).toPixels(cConstraint)
            + mAspectRatioList.at(mModel->rowCount() - index.row() - 1) * cConstraint;

    unsigned int firstOffset = index.row() == 0 ?
                firstAlongMargin().toPixels(cConstraint) :
                mSpacing.toPixels(cConstraint) / 2;
    unsigned int lastOffset = index.row() == mModel->rowCount() - 1 ?
                secondAlongMargin().toPixels(cConstraint) :
                mSpacing.toPixels(cConstraint) / 2;
    if(!includePadding) {
        firstOffset = 0;
        lastOffset = 0;
    }

    qDebug() << "ensureTotallyVisible" << indexPixelMin << indexPixelMax << firstOffset << lastOffset;
    qDebug() << ((int) indexPixelMin + indexPixelMax - alongDimension(maximumViewportSize())) / 2;

    /*
     * Six cases:
     *  1.2.3. Use hint.
     *  4.The preview is too large to be fully visible.
     *  5.The preview is before the first visible preview.
     *  6.The preview is after the last visible preview.
     */
    if (hint == QAbstractItemView::PositionAtTop)
        alongScrollBar()->setValue(indexPixelMin - firstOffset);
    else if (hint == QAbstractItemView::PositionAtBottom)
        alongScrollBar()->setValue(indexPixelMax + lastOffset - alongDimension(maximumViewportSize()));
    else if (hint == QAbstractItemView::PositionAtCenter)
        alongScrollBar()->setValue(qMax(0, qMin(alongScrollBar()->maximum(),
                                                ((int) (indexPixelMin + indexPixelMax) - alongDimension(maximumViewportSize())) / 2)));
    else if (indexPixelMax - indexPixelMin > (unsigned int) alongDimension(maximumViewportSize()))
        alongScrollBar()->setValue((indexPixelMin + indexPixelMax - alongDimension(maximumViewportSize())) / 2);
    else if (getPosition() > indexPixelMin)
        alongScrollBar()->setValue(indexPixelMin - firstOffset);
    else if (getPosition() + alongDimension(maximumViewportSize()) < indexPixelMax)
        alongScrollBar()->setValue(indexPixelMax + lastOffset - alongDimension(maximumViewportSize()));
}

/*void GalleryView::center(const QModelIndex &index)
{
    if(!index.isValid())
        return;
    if(cTotal < (unsigned int) alongDimension(maximumViewportSize()))
        return; // This means everythig is displayed

    unsigned int indexPixelCenter = (firstAlongMargin()
            + (index.row() + 1) * firstAlongPadding()
            + index.row() * secondAlongPadding()
            + index.row() * mSpacing).toPixels(cConstraint)
            + (mAspectRatioList.at(mModel->rowCount() - index.row() - 1)
            + mAspectRatioList.at(mModel->rowCount() - index.row())) / 2. * cConstraint;

    alongScrollBar()->setValue(
            qRound(qMax((unsigned int) 0,
                        qMin((unsigned int) alongScrollBar()->maximum(),
                             indexPixelCenter - alongDimension(maximumViewportSize()) / 2))));
} */

QModelIndex GalleryView::indexAt(const QPoint& screenPoint, bool includePadding) const
{
    QPoint point = toViewportCoodinates(screenPoint);
    qDebug() << __func__ << point << screenPoint;

    unsigned int accrossMin = firstAcrossMargin().toPixels(cConstraint);
    unsigned int accrossMax = cConstraint
            + (firstAcrossMargin() + firstAcrossPadding()).toPixels(cConstraint);

    if (!includePadding)
        accrossMin += firstAcrossPadding().toPixels(cConstraint);
    else
        accrossMax += secondAcrossPadding().toPixels(cConstraint);

    /* Point in accross margins */
    if ((accrossDimension(point) < 0 )
     || ((unsigned int) accrossDimension(point) < accrossMin)
     || ((unsigned int) accrossDimension(point) > accrossMax))
        return QModelIndex();

    if (alongDimension(point) < 0)
        return QModelIndex();
    int n = mModel->rowCount();

    Dimension spacesCumulatedMin = firstAlongMargin();
    Dimension spacesCumulatedMax = firstAlongMargin() + firstAlongPadding();
    if(!includePadding)
        spacesCumulatedMin += firstAlongPadding();
    else
        spacesCumulatedMax += secondAlongPadding();

    for (int i = 0; i < n; i++) {
        unsigned int pixelMin = mAspectRatioList.at(n - i) * cConstraint
                + spacesCumulatedMin.toPixels(cConstraint);
        unsigned int pixelMax = mAspectRatioList.at(n - i - 1) * cConstraint
                + spacesCumulatedMax.toPixels(cConstraint);

        /* Means position is in non-index space */
        if ((unsigned int) alongDimension(point) < pixelMin)
            return QModelIndex();

        /* Means position is in the current item */
        if ((unsigned int) alongDimension(point) <= pixelMax)
            return mModel->index(i);

        spacesCumulatedMin += (firstAlongPadding() + mSpacing + secondAlongPadding());
        spacesCumulatedMax += (firstAlongPadding() + mSpacing + secondAlongPadding());
    }

    return QModelIndex();
}

QRect GalleryView::visualRect(const QModelIndex &index, bool includePadding) const
{
    if (!index.isValid())
        return QRect();
    /*if (cTotal < (unsigned int) alongDimension(maximumViewportSize()))
        return; /* This means everythig is displayed */

    unsigned int indexPixelMin = (firstAlongMargin()
            + (includePadding ? index.row() : index.row() + 1) * firstAlongPadding()
            + index.row() * secondAlongPadding()
            + index.row() * mSpacing).toPixels(cConstraint)
            + mAspectRatioList.at(mModel->rowCount() - index.row()) * cConstraint;
    unsigned int indexPixelMax = (firstAlongMargin()
            + (index.row() + 1) * firstAlongPadding()
            + (includePadding ? index.row() + 1 : index.row()) *secondAlongPadding()
            + index.row() * mSpacing).toPixels(cConstraint)
            + mAspectRatioList.at(mModel->rowCount() - index.row() - 1) * cConstraint;
    unsigned int accrossPixelMin = (firstAcrossMargin()
            + (includePadding ? 0 : 1) * firstAcrossPadding()).toPixels(cConstraint);
    unsigned int accrossLength = cConstraint
            + (includePadding ? 1 : 0) * (firstAcrossPadding()
                                          + secondAcrossPadding()).toPixels(cConstraint);


    if (mOrientation == Qt::Vertical)
        return fromViewportCoodinates(QRect(accrossPixelMin,
                                            indexPixelMin,
                                            accrossLength,
                                            indexPixelMax - indexPixelMin));
    else
        return fromViewportCoodinates(QRect(indexPixelMin,
                                      accrossPixelMin,
                                      indexPixelMax - indexPixelMin,
                                      accrossLength));
}

int GalleryView::horizontalOffset(void) const
{
    if (mOrientation == Qt::Vertical)
        return 0;
    else
        return getPosition();
}

int GalleryView::verticalOffset(void) const
{
    if (mOrientation == Qt::Vertical)
        return getPosition();
    else
        return 0;
}

QModelIndex GalleryView::moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
    Q_UNUSED(modifiers);

    QModelIndex index =  currentIndex();

    if ((cursorAction == alongAddDirection())
     || (cursorAction == QAbstractItemView::MoveNext)) {
        if (!index.isValid())
            return mModel->index(0);
        else if (index.row() < mModel->rowCount() - 1)
            return index.sibling(index.row() + 1, 0);
    } else if ((cursorAction == alongSubDirection())
           || (cursorAction == QAbstractItemView::MovePrevious)){
        if (!index.isValid())
            return mModel->index(mModel->rowCount() - 1);
        else if (index.row() > 0)
            return index.sibling(index.row() - 1, 0);
    } else if (cursorAction == QAbstractItemView::MoveHome) {
        return mModel->index(0);
    } else if (cursorAction == QAbstractItemView::MoveEnd) {
        return mModel->index(mModel->rowCount() - 1);
    }

    return index;
}

void GalleryView::setSelection(const QRect &screenRect,
                               bool includePadding,
                               QItemSelectionModel::SelectionFlags command)
{
    QRect rect = toViewportCoodinates(screenRect.normalized());

    qDebug() << "setSelection" << rect << command << rect.topLeft() << rect.bottomRight();

    Dimension accrossMin = firstAcrossMargin();
    Dimension accrossMax = firstAcrossMargin() + firstAcrossPadding();
    if(includePadding)
        accrossMax += secondAcrossPadding();
    else
        accrossMin += firstAcrossPadding();

    if(accrossDimension(rect.bottomRight()) < (int) accrossMin.toPixels(cConstraint))
        return; /* The selection is in the first accross margin */
    if(accrossDimension(rect.topLeft()) > (int) cConstraint + accrossMax.toPixels(cConstraint))
        return; /* The selection is in the second accross margin */

    if(alongDimension(rect.bottomRight()) < 0)
        return; /* The selection is in the first along margin */
    if(alongDimension(rect.topLeft()) > (int) cTotal)
        return; /* The selection is in the second along margin */

    int n = mModel->rowCount();
    int i = 0;
    Dimension spacesCumulatedMin = firstAlongMargin();
    Dimension spacesCumulatedMax = firstAlongMargin() + firstAlongPadding();
    if (includePadding)
        spacesCumulatedMax += secondAlongPadding();
    else
        spacesCumulatedMin += firstAlongPadding();

    while ((i < n)
        && (spacesCumulatedMax.toPixels(cConstraint)
           + qCeil(mAspectRatioList.at(n - i - 1)*cConstraint)
           < alongDimension(rect.topLeft()))) {
        i++;
        spacesCumulatedMin += (secondAlongPadding() + mSpacing + firstAlongPadding());
        spacesCumulatedMax += (firstAlongPadding() + mSpacing + secondAlongPadding());
    }

    if(i == n)
        return; /* Rect is in second along margin. */

    qDebug() << spacesCumulatedMax.toPixels(cConstraint) + qFloor(mAspectRatioList.at(n - i - 1)*cConstraint)
             << alongDimension(rect.topLeft());

    if (alongDimension(rect.bottomRight())
        < spacesCumulatedMin.toPixels(cConstraint)
        + qFloor(mAspectRatioList.at(n - i)*cConstraint))
    return;

    QModelIndex firstIndex = mModel->index(i++);
    spacesCumulatedMin += (secondAlongPadding() + mSpacing + firstAlongPadding());
    spacesCumulatedMax += (firstAlongPadding() + mSpacing + secondAlongPadding());

    while ((i < n)
        && (spacesCumulatedMin.toPixels(cConstraint)
           + qFloor(mAspectRatioList.at(n - i)*cConstraint)
           < alongDimension(rect.bottomRight()))) {
        qDebug() << spacesCumulatedMin.toPixels(cConstraint) + qFloor(mAspectRatioList.at(n - i)*cConstraint);
        i++;
        spacesCumulatedMin += (secondAlongPadding() + mSpacing + firstAlongPadding());
        spacesCumulatedMax += (firstAlongPadding() + mSpacing + secondAlongPadding());
    }

    qDebug() << spacesCumulatedMin.toPixels(cConstraint) + qFloor(mAspectRatioList.at(n - i)*cConstraint)
             << alongDimension(rect.bottomRight());

    QModelIndex lastIndex = mModel->index(i - 1);

    selectionModel()->select(QItemSelection(firstIndex, lastIndex), command);
}

QRegion GalleryView::visualRegionForSelection(const QItemSelection &selection,
                                              bool includePadding) const
{
    QRegion region;

    foreach(QItemSelectionRange range, selection)
        region+=(visualRect(range.topLeft(), includePadding)
                 | visualRect(range.bottomRight(), includePadding));

    return region;
}

void GalleryView::paintEvent(QPaintEvent *pe)
{
    QItemSelectionModel *mSelectionModel = selectionModel();

    if (mAspectRatioList.size() == 0) {
        updateAspectRatioList();
        updateScrollBars();
    }

    QPainter p(viewport());

    Dimension currentAlong = firstAlongPadding() + firstAlongMargin();
    unsigned int pixelMin = getPosition(),
                 pixelMax = getPosition() + alongDimension((maximumViewportSize()));
    unsigned int n = mAspectRatioList.size() - 1;
    unsigned int originAcross = (firstAcrossPadding() + firstAcrossMargin()).toPixels(cConstraint);
    unsigned int lengthAccross = cConstraint;

    qDebug() << "Repainting:" << pe->rect() << pixelMin << pixelMax;// << mAspectRatioList;

    /* Along alignment */
    if(mAlignment & Qt::AlignHCenter)
        currentAlong += Dimension(qMax(0,
                                       (int) (alongDimension(maximumViewportSize()) - cTotal)) / 2,
                                  Dimension::PIXEL);
    else if(mAlignment & Qt::AlignRight)
        currentAlong += Dimension(qMax(0,
                                       (int) (alongDimension(maximumViewportSize()) - cTotal)),
                                  Dimension::PIXEL);
    /* Accross alignment */
    if(mAlignment & Qt::AlignVCenter)
        originAcross += qMax((unsigned int) 0, cAcross - cConstraint -
                            (firstAcrossPadding() + secondAcrossPadding()).toPixels(cConstraint) -
                            (firstAcrossMargin() + secondAcrossMargin()).toPixels(cConstraint)) / 2;
    else if(mAlignment & Qt::AlignBottom)
        originAcross += qMax((unsigned int) 0, cAcross - cConstraint -
                             (firstAcrossPadding() + secondAcrossPadding()).toPixels(cConstraint) -
                             (firstAcrossMargin() + secondAcrossMargin()).toPixels(cConstraint));

    for(int i = n - 1; i >= 0; i--) {
        unsigned int originAlong = currentAlong.toPixels(cConstraint)
                                 + mAspectRatioList.at(i + 1) * cConstraint;
        unsigned int endAlong = currentAlong.toPixels(cConstraint)
                              + mAspectRatioList.at(i) * cConstraint;

        if (originAlong > pixelMax)
            break;

        if (endAlong >= pixelMin) {
            // For selection/activation:
            if (mSelectionModel && mSelectionModel->isSelected(mModel->index(n - 1 - i))) {
                p.setPen(Qt::NoPen);
                p.setBrush(QBrush(palette().highlight()));
                if(mOrientation == Qt::Vertical)
                    p.drawRect(originAcross - firstAcrossPadding().toPixels(cConstraint),
                               originAlong - firstAlongPadding().toPixels(cConstraint) - pixelMin,
                               lengthAccross + (firstAcrossPadding()
                                             + secondAcrossPadding()).toPixels(cConstraint),
                               endAlong - originAlong + (firstAlongPadding()
                                                      + secondAlongPadding()).toPixels(cConstraint));
                else
                    p.drawRect(originAlong - firstAlongPadding().toPixels(cConstraint) - pixelMin,
                               originAcross - firstAcrossPadding().toPixels(cConstraint),
                               endAlong - originAlong  + (firstAlongPadding()
                                                       + secondAlongPadding()).toPixels(cConstraint),
                               lengthAccross  + (firstAcrossPadding()
                                              + secondAcrossPadding()).toPixels(cConstraint));
            }
            if (currentIndex().isValid()
            && ((unsigned int) currentIndex().row() == n - 1 - i)) {
                p.setPen(QPen(QBrush(palette().windowText()),
                              0.,
                              Qt::DotLine,
                              Qt::SquareCap,
                              Qt::BevelJoin));
                p.setBrush(Qt::NoBrush);
                if (mOrientation == Qt::Vertical)
                    p.drawRect(originAcross - firstAcrossPadding().toPixels(cConstraint),
                               originAlong - firstAlongPadding().toPixels(cConstraint) - pixelMin,
                               lengthAccross - 1 + (firstAcrossPadding()
                                                 + secondAcrossPadding()).toPixels(cConstraint),
                               endAlong - originAlong - 1 + (firstAlongPadding()
                                                          + secondAlongPadding()).toPixels(cConstraint));
                else
                    p.drawRect(originAlong - firstAlongPadding().toPixels(cConstraint) - pixelMin,
                               originAcross - firstAcrossPadding().toPixels(cConstraint),
                               endAlong - originAlong - 1 + (firstAlongPadding()
                                                          + secondAlongPadding()).toPixels(cConstraint),
                               lengthAccross - 1 + (firstAcrossPadding()
                                                 + secondAcrossPadding()).toPixels(cConstraint));
            }

            p.setPen(QPen(QBrush(palette().windowText()), 0., Qt::SolidLine));
            p.setBrush(QBrush(palette().base()));
            // Check cached pixmaps list:
            QImage image = cPreviews.at(i);
            if(image.isNull()) {
                image = mModel->data(mModel->index(n - 1 -i), PreviewModel::PreviewRole)
                                .value<QImage>();
                cPreviews.replace(i, image);
            }
            // Draws the image:
            if (mOrientation == Qt::Vertical) {
                if(!image.isNull())
                    /*p.drawImage(originAcross, originAlong - pixelMin,
                                lengthAccross, endAlong - originAlong, image);*/
                    p.drawImage(originAcross, originAlong - pixelMin, image);
                else
                    p.drawRect(originAcross, originAlong - pixelMin,
                               lengthAccross - 1, endAlong - originAlong - 1);
            } else {
                if(!image.isNull())
                    /*p.drawImage(originAlong - pixelMin, originAcross,
                                 endAlong - originAlong, lengthAccross, image);*/
                    p.drawImage(originAlong - pixelMin, originAcross, image);
                else
                    p.drawRect(originAlong - pixelMin, originAcross,
                               endAlong - originAlong - 1, lengthAccross - 1);
            }
        }

        currentAlong+=(secondAlongPadding() + mSpacing + firstAlongPadding());
    }

    // TODO To be deleted:
    /*for(int i = 0; i < mModel->rowCount(); i++)
    {
        qDebug() << "Item" << i
                 << (isPartiallyVisible(mModel->index(i)) ? "is partially" : "is not")
                 << (isTotallyVisible(mModel->index(i)) ? "and totally visible" : "visible");
    }*/
}

void GalleryView::resizeEvent(QResizeEvent *re)
{
    Q_UNUSED(re);

    updateScrollBars();
    viewport()->update();
}

/*void GalleryView::keyPressEvent(QKeyEvent *ke)
{
    if (ke->key() == alongAddKey()) {
        if ((ke->modifiers() & Qt::ShiftModifier) && (ke->modifiers() & Qt::ControlModifier))
            expandSelectionTowards(GalleryView::POSITIVE, true);
        else if (ke->modifiers() & Qt::ShiftModifier)
            expandSelectionTowards(GalleryView::POSITIVE);
        else if (ke->modifiers() & Qt::AltModifier)
            activeItemTowards(GalleryView::POSITIVE);
        else if (ke->modifiers() & Qt::ControlModifier)
            alongScrollBar()->triggerAction(QAbstractSlider::SliderToMaximum);
        else
            alongScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepAdd);
        ke->accept();
    } else if (ke->key() == alongSubKey()) {
        if ((ke->modifiers() & Qt::ShiftModifier) && (ke->modifiers() & Qt::ControlModifier))
            expandSelectionTowards(GalleryView::NEGATIVE, true);
        else if (ke->modifiers() & Qt::ShiftModifier)
            expandSelectionTowards(GalleryView::NEGATIVE);
        else if (ke->modifiers() & Qt::AltModifier)
            activeItemTowards(GalleryView::NEGATIVE);
        else if(ke->modifiers() & Qt::ControlModifier)
            alongScrollBar()->triggerAction(QAbstractSlider::SliderToMinimum);
        else
            alongScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepSub);
        ke->accept();
    } else if (ke->key() == Qt::Key_PageUp) {
        alongScrollBar()->triggerAction(QAbstractSlider::SliderPageStepSub);
        ke->accept();
    } else  if (ke->key() == Qt::Key_PageDown) {
        alongScrollBar()->triggerAction(QAbstractSlider::SliderPageStepAdd);
        ke->accept();
    } else {
        ke->ignore();
    }
}*/

void GalleryView::keyPressEvent(QKeyEvent *ke)
{
    if (ke->key() == alongAddKey()) {
        if (ke->modifiers() & Qt::AltModifier) {
            alongScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepAdd);
            ke->accept();
            return;
        }
    } else if (ke->key() == alongSubKey()) {
        if (ke->modifiers() & Qt::AltModifier) {
            alongScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepSub);
            ke->accept();
            return;
        }
    } else if (ke->key() == Qt::Key_PageUp) {
        if (ke->modifiers() & Qt::ControlModifier) {
            alongScrollBar()->triggerAction(QAbstractSlider::SliderToMinimum);
            ke->accept();
        } else {
            alongScrollBar()->triggerAction(QAbstractSlider::SliderPageStepSub);
            ke->accept();
        }
    } else  if (ke->key() == Qt::Key_PageDown) {
        if (ke->modifiers() & Qt::ControlModifier) {
            alongScrollBar()->triggerAction(QAbstractSlider::SliderToMaximum);
            ke->accept();
        } else {
            alongScrollBar()->triggerAction(QAbstractSlider::SliderPageStepAdd);
            ke->accept();
        }
    }

    QAbstractItemView::keyPressEvent(ke);
}


/*void GalleryView::mousePressEvent(QMouseEvent *me)
{
    // TODO to be deleted :
    QModelIndex indexWith = indexAt(me->pos(), true);
    QModelIndex indexWithout = indexAt(me->pos(), false);

    qDebug() << "Avec padding:" << (indexWith.isValid() ? QString::number(indexWith.row()) : "No index");
    qDebug() << "Sans padding:" << (indexWithout.isValid() ? QString::number(indexWithout.row()) : "No index");

    QModelIndex index = indexAt(me->pos(), false);

    if(!index.isValid()) {
        me->ignore();
        return;
    }
    me->accept();

    if(me->modifiers() & Qt::ShiftModifier) {
        QItemSelection selection = mSelectionModel->selection();
        if ((me->modifiers() & Qt::ControlModifier)
        && !mSelectionModel->isSelected(mSelectionModel->currentIndex())) {
            if (index.row() < mSelectionModel->currentIndex().row())
                selection.append(QItemSelectionRange(index, mSelectionModel->currentIndex()));
            else if (index.row() > mSelectionModel->currentIndex().row())
                selection.append(QItemSelectionRange(mSelectionModel->currentIndex(), index));
            else
                selection.append(QItemSelectionRange(index));
        } else if (mSelectionModel->isSelected(mSelectionModel->currentIndex())) {
            if (index.row() < mSelectionModel->currentIndex().row())
                selection.replace(selection.size() - 1, QItemSelectionRange(index, mSelectionModel->currentIndex()));
            else if (index.row() > mSelectionModel->currentIndex().row())
                selection.replace(selection.size() - 1, QItemSelectionRange(mSelectionModel->currentIndex(), index));
            else
                selection.replace(selection.size() - 1, QItemSelectionRange(index));
        } else {
            if (index.row() < mSelectionModel->currentIndex().row())
                selection = QItemSelection(index, mSelectionModel->currentIndex());
            else if (index.row() > mSelectionModel->currentIndex().row())
                selection = QItemSelection(mSelectionModel->currentIndex(), index);
            else
                selection = QItemSelection(index, index);
        }
        mSelectionModel->select(selection, QItemSelectionModel::ClearAndSelect);
    } else if (me->modifiers() & Qt::ControlModifier) {
        mSelectionModel->setCurrentIndex(index, QItemSelectionModel::Toggle);
    } else {
        if(!mSelectionModel->isSelected(index))
            mSelectionModel->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
        else
            ;// TODO begin drag
    }

    viewport()->update();
}*/


/*void GalleryView::mouseMoveEvent(QMouseEvent *me)
{
    QModelIndex index = indexAt(me->pos(), false);

    if(!index.isValid()) {
        me->ignore();
        return;
    }
    me->accept();

    if(!mSelectionModel->isSelected(mSelectionModel->currentIndex()))
        mSelectionModel->select(mSelectionModel->currentIndex(), QItemSelectionModel::SelectCurrent);

    //if(mSelectionModel->currentIndex() == index)
    //    return;

    //mSelectionModel->setCurrentIndex(index, QItemSelectionModel::NoUpdate);

    QItemSelection selection = mSelectionModel->selection();
    if (index.row() < mSelectionModel->currentIndex().row())
        selection.replace(selection.size() - 1, QItemSelectionRange(index, selection.last().bottomRight()));
    else if (index.row() > mSelectionModel->currentIndex().row())
        selection.replace(selection.size() - 1, QItemSelectionRange(selection.last().topLeft(), index));
    else
        selection.replace(selection.size() - 1, QItemSelectionRange(index));
    mSelectionModel->select(selection, QItemSelectionModel::ClearAndSelect);

    viewport()->update();
}*/

/*void GalleryView::mouseReleaseEvent(QMouseEvent *me)
{
    QModelIndex index = indexAt(me->pos(), false);

    me->accept();

    if(!index.isValid())
        return;

    if (!(me->modifiers() & Qt::ControlModifier)) {
        QItemSelection selection;
        selection.append(mSelectionModel->selection().last());
        mSelectionModel->select(selection, QItemSelectionModel::ClearAndSelect);
    }
    if (!(me->modifiers() & Qt::ShiftModifier))
        mSelectionModel->setCurrentIndex(index, QItemSelectionModel::NoUpdate);

    viewport()->update();
}*/

void GalleryView::ensureFirstIndexVisible(void)
{
    if(alongScrollBar()->value() == alongScrollBar()->minimum())
        return; /* We cannot scroll further in negative direction */

    int i = 0;
    int n = mModel->rowCount();
    unsigned int offset = mSpacing.toPixels(cConstraint) / 2;
    Dimension spacesCumulated = firstAlongMargin() + firstAlongPadding() + secondAlongPadding();

    while (i < n) {
        qDebug() << qFloor(mAspectRatioList.at(n - i - 1) * cConstraint) + spacesCumulated.toPixels(cConstraint) + offset
                 << getPosition();
        if (qFloor(mAspectRatioList.at(n - i - 1) * cConstraint)
            + spacesCumulated.toPixels(cConstraint) + offset
            >= getPosition())
            break;
        spacesCumulated += (firstAlongPadding() + mSpacing + secondAlongPadding());
        i++;
    }

    qDebug() << "Index to be displayed at left:" << i;

    /* Ensure preview i is visible */
    unsigned int indexPixelMin = (firstAlongMargin()
            + i * (firstAlongPadding() + secondAlongPadding())
            + (i - 1) * mSpacing).toPixels(cConstraint)
            + mSpacing.toPixels(cConstraint) / 2
            + qFloor(mAspectRatioList.at(n - i) * cConstraint);

    qDebug() << indexPixelMin << (unsigned int) alongScrollBar()->maximum()
             << qMin((unsigned int) alongScrollBar()->maximum(), indexPixelMin);

    if (i == 0)
        alongScrollBar()->setValue(0);
    else
        alongScrollBar()->setValue(qMin((unsigned int) alongScrollBar()->maximum(), indexPixelMin));
}

void GalleryView::ensureLastIndexVisible(void)
{
    if(alongScrollBar()->value() == alongScrollBar()->maximum())
        return; /* We cannot scroll further in positive direction */

    int i = 0;
    int n = mModel->rowCount();
    unsigned int offset = mSpacing.toPixels(cConstraint) / 2;
    Dimension spacesCumulated = firstAlongMargin() + firstAlongPadding() + secondAlongPadding();

    while (i < n) {
        qDebug() << qCeil(mAspectRatioList.at(n - i - 1) * cConstraint) + spacesCumulated.toPixels(cConstraint) + offset
                 << getPosition() + alongDimension(maximumViewportSize());
        if (qCeil(mAspectRatioList.at(n - i - 1) * cConstraint)
            + spacesCumulated.toPixels(cConstraint) + offset
            > getPosition() + alongDimension(maximumViewportSize()))
            break;
        spacesCumulated += (firstAlongPadding() + mSpacing + secondAlongPadding());
        i++;
    }

    qDebug() << "Index to be displayed at right:" << i;

    /* Ensure preview i is visible */
    unsigned int indexPixelMax = (firstAlongMargin()
            + (i + 1) * (firstAlongPadding() + secondAlongPadding())
            + i * mSpacing).toPixels(cConstraint)
            + mSpacing.toPixels(cConstraint) / 2
            + qCeil(mAspectRatioList.at(n - i - 1) * cConstraint);

    qDebug() << indexPixelMax - alongDimension(maximumViewportSize()) << alongScrollBar()->maximum()
             << qMax((unsigned int) 0, indexPixelMax - alongDimension(maximumViewportSize()));

    if (i == n - 1)
        alongScrollBar()->setValue(alongScrollBar()->maximum());
    else
        alongScrollBar()->setValue(qMax((unsigned int) 0,
                                        indexPixelMax - alongDimension(maximumViewportSize())));
}

/*void GalleryView::activeItemTowards(Direction dir)
{
    QModelIndex currentIndex = mSelectionModel->currentIndex();

    switch(dir)
    {
    case GalleryView::POSITIVE :
        qDebug() << "activeItemToRight()"
                 << (currentIndex.isValid() ? currentIndex.row() : -1);
        if (!currentIndex.isValid())
            mSelectionModel->setCurrentIndex(mModel->index(0), QItemSelectionModel::NoUpdate);
        else if (currentIndex.row() != mModel->rowCount() - 1)
            mSelectionModel->setCurrentIndex(currentIndex.sibling(currentIndex.row() + 1, 0),
                                             QItemSelectionModel::NoUpdate);

        qDebug() << "activeItemToRight()" << mSelectionModel->currentIndex().row();
        break;
    case GalleryView::NEGATIVE :
        qDebug() << "activeItemToLeft()"
                 << (currentIndex.isValid() ? currentIndex.row() : -1);
        if (!currentIndex.isValid())
            mSelectionModel->setCurrentIndex(mModel->index(mModel->rowCount() - 1),
                                             QItemSelectionModel::NoUpdate);
        else if (currentIndex.row() != 0)
            mSelectionModel->setCurrentIndex(currentIndex.sibling(currentIndex.row() - 1, 0),
                                             QItemSelectionModel::NoUpdate);

        qDebug() << "activeItemToLeft()" << mSelectionModel->currentIndex().row();
        break;
    default:
        return;
    }

    viewport()->update(); // TODO Optimisation: Update only modified index(es)
}*/

/*void GalleryView::expandSelectionTowards(Direction dir, bool add)
{
    QModelIndex currentIndex = mSelectionModel->currentIndex();

    qDebug() << "Selection" << mSelectionModel->selection();

    switch(dir)
    {
    case GalleryView::POSITIVE:
        if (!currentIndex.isValid()) {
            mSelectionModel->setCurrentIndex(mModel->index(0), QItemSelectionModel::SelectCurrent);
        } else if (currentIndex.row() != mModel->rowCount() - 1) {
            QModelIndex nextIndex = currentIndex.sibling(currentIndex.row() + 1, 0);
            QItemSelection selection = mSelectionModel->selection();

            if (currentIndex == selection.last().bottomRight()) {
                selection.replace(selection.size() - 1, QItemSelectionRange(selection.last().topLeft(), nextIndex));
            } else if (currentIndex == selection.last().topLeft()) {
                selection.replace(selection.size() - 1, QItemSelectionRange(nextIndex, selection.last().bottomRight()));
            } else {
                if(add)
                    selection.append(QItemSelectionRange(currentIndex, nextIndex));
                else
                    selection = QItemSelection(currentIndex, nextIndex);
            }

            mSelectionModel->select(selection, QItemSelectionModel::ClearAndSelect);
            mSelectionModel->setCurrentIndex(nextIndex, QItemSelectionModel::NoUpdate);
        }
        break;
    case GalleryView::NEGATIVE:
        if (!currentIndex.isValid()) {
            mSelectionModel->setCurrentIndex(mModel->index(mModel->rowCount() - 1),
                                            QItemSelectionModel::SelectCurrent);
        } else if (currentIndex.row() != 0) {
            QModelIndex prevIndex = currentIndex.sibling(currentIndex.row() - 1, 0);
            QItemSelection selection = mSelectionModel->selection();

            if (currentIndex == selection.last().topLeft()) {
                selection.replace(selection.size() - 1, QItemSelectionRange(prevIndex, selection.last().bottomRight()));
            } else if (currentIndex == selection.last().bottomRight()) {
                selection.replace(selection.size() - 1, QItemSelectionRange(selection.last().topLeft(), prevIndex));
            } else {
                if(add)
                    selection.append(QItemSelectionRange(prevIndex, currentIndex));
                else
                    selection = QItemSelection(prevIndex, currentIndex);
            }

            mSelectionModel->select(selection, QItemSelectionModel::ClearAndSelect);
            mSelectionModel->setCurrentIndex(prevIndex, QItemSelectionModel::NoUpdate);
        }
        break;
    default:
        return;
    }

    viewport()->update(); // TODO Optimisation: Update only modified index(es)
}*/

void GalleryView::scrollAlong(int action)
{
    qDebug() << "Scroll along:" << action;
    switch (action) {
    case QAbstractSlider::SliderMove:
    case QAbstractSlider::SliderSingleStepAdd:
    case QAbstractSlider::SliderSingleStepSub:
        alongScrollBar()->setValue(alongScrollBar()->sliderPosition());
        break;
    case QAbstractSlider::SliderPageStepAdd:
        // Set position so that last image is in middle of spacing or end of margin.
        ensureLastIndexVisible();
        break;
    case QAbstractSlider::SliderPageStepSub:
        // Set position so that first image is in middle of spacing or begin of margin.
        ensureFirstIndexVisible();
        break;
    case QAbstractSlider::SliderToMaximum:
        alongScrollBar()->setValue(alongScrollBar()->maximum());
        break;
    case QAbstractSlider::SliderToMinimum:
        alongScrollBar()->setValue(0);
        break;
    default:
        break;
    }
}

void GalleryView::updateAspectRatioList(void)
{
    if (!mModel)
        return;

    double prev = 0.;
    int n = mModel->rowCount();
    mAspectRatioList.resize(n + 1);
    mAspectRatioList.replace(n, prev);
    cPreviews.clear();
    cPreviews.resize(n);

    for (int i = 0; i < n; i++) {
        QSize itemSize = mModel->data(mModel->index(i), Qt::SizeHintRole).toSize();

        if(!itemSize.isValid())
            ;
        else if(mOrientation == Qt::Vertical)
            prev+=((double) itemSize.height()) / ((double) itemSize.width());
        else
            prev+=((double) itemSize.width()) / ((double) itemSize.height());

        mAspectRatioList.replace(n - 1 - i, prev);
    }

    qDebug() << mAspectRatioList;
}

unsigned int GalleryView::computeConstraint(bool scrollBar) const
{
    unsigned int constraint;

    QStyleOption opt;
    opt.init(this);
    const int extra = style()->pixelMetric(QStyle::PM_ScrollView_ScrollBarSpacing, &opt, this)
                    + style()->pixelMetric(QStyle::PM_ScrollBarExtent, &opt, this);

    constraint = accrossDimension(maximumViewportSize());
    if(scrollBar)
        constraint -= extra;
    constraint = (firstAcrossPadding() + secondAcrossPadding() +
                  firstAcrossMargin() + secondAcrossMargin()).fromPixels(constraint);

    return constraint;
}

unsigned int GalleryView::computeAlernative(void) const
{
    qreal factor = mAspectRatioList.at(0);
    unsigned int constraint = alongDimension(maximumViewportSize());
    int n = mAspectRatioList.size() - 1;

    factor += (firstAlongPadding().percents() + secondAlongPadding().percents()) * n / 100.;
    factor += (firstAlongMargin().percents() + secondAlongMargin().percents()) / 100.;
    factor += mSpacing.percents() * (n - 1) / 100.;
    constraint -= (firstAlongPadding().pixels() + secondAlongPadding().pixels()) * n;
    constraint -= (firstAlongMargin().pixels() + secondAlongMargin().pixels());
    constraint -= mSpacing.pixels() * (n - 1);

    return (unsigned int) qFloor(constraint/factor);
}

qreal GalleryView::computeTotal(unsigned int constraint) const
{
    if (mAspectRatioList.isEmpty())
        return 0.;

    qreal total = mAspectRatioList.at(0)*constraint;
    int n = mAspectRatioList.size() - 1;

    total += ((firstAlongPadding() + secondAlongPadding()) * n).toPixels(constraint);
    total += (firstAlongMargin() + secondAlongMargin()).toPixels(constraint);
    total += (mSpacing * (n - 1)).toPixels(constraint);

    return total;
}

QSize GalleryView::viewportSize(bool scrolBar)
{
    QSize size = maximumViewportSize();

    QStyleOption opt;
    opt.init(this);
    const int extra = style()->pixelMetric(QStyle::PM_ScrollView_ScrollBarSpacing, &opt, this)
                    + style()->pixelMetric(QStyle::PM_ScrollBarExtent, &opt, this);

    int& accross = raccrossDimension(size);
    if(scrolBar)
        accross -= extra;
    return size;
}

void GalleryView::updateScrollBars(void)
{
    if(mAspectRatioList.size() == 0)
        updateAspectRatioList();

    unsigned int constraintWithoutScrollBar = computeConstraint(false),
                 constraintWithScrollBar = computeConstraint(true),
                 pageSize = alongDimension(maximumViewportSize());
    qreal totalSizeWithoutScrollBars = computeTotal(constraintWithoutScrollBar),
          totalSizeWithScrollBars = computeTotal(constraintWithScrollBar);

    qDebug() << "Constraints: " << constraintWithScrollBar << constraintWithoutScrollBar;
    qDebug() << "Totaux: " << totalSizeWithScrollBars << totalSizeWithoutScrollBars;

    if(totalSizeWithoutScrollBars > pageSize) {
        // Take into account constraintWithScrollBar
        // WARNING the scrool bar might not be shown even in this case.
        if(totalSizeWithScrollBars > pageSize) {
            // There will really be scroll bars.
            cConstraint = constraintWithScrollBar;
            cAcross = accrossDimension(viewportSize(true));
        } else {
            // There won't be scroll bars => extend to max.
            cConstraint = computeAlernative();
            cAcross = accrossDimension(viewportSize(false));
        }

        qDebug() << "Total width (with scroll bar):" << totalSizeWithScrollBars << pageSize;

        alongScrollBar()->setMinimum(0);
        alongScrollBar()->setMaximum(qMax((unsigned int) 0,
                                          qCeil(totalSizeWithScrollBars) - pageSize));

    } else {
        // Take into account constraintWithoutScrollBar
        cConstraint = constraintWithoutScrollBar;
        cAcross = accrossDimension(viewportSize(false));

        qDebug() << "Total width (without scroll bar):" << totalSizeWithoutScrollBars << pageSize;

        alongScrollBar()->setMinimum(0);
        alongScrollBar()->setMaximum(0);
    }

    cTotal = qCeil(computeTotal(cConstraint));
    qDebug() << "Cached constraint and total:" << cConstraint << cTotal;
    /*
     * NOTE here the given orientation is the direction perpendicular to item
     * flow because it is the orientation of the constraint on preview size.
     */
    if(mModel)
        mModel->setPreviewConstraint(cConstraint,
                                     mOrientation == Qt::Vertical ?
                                         Qt::Horizontal :
                                         Qt::Vertical);
}
