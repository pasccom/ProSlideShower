#ifndef GALLERYMODEL_H
#define GALLERYMODEL_H

#include <QAbstractListModel>
#include <QVector>
#include <QStringList>
#include <QSize>
#include <QtDebug>

class PreviewModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum GalleryItemDataRole {PreviewRole = Qt::UserRole};

    inline PreviewModel(QObject *parent = 0) :
        QAbstractListModel(parent) {}

    virtual inline void setPreviewConstraint(int dim, Qt::Orientation orientation = Qt::Vertical)
    {
        mConstraintDim = dim;
        mConstraintOrientation = orientation;
        qDebug() << "Constraint: " << dim;
    }
protected:
    int mConstraintDim;
    Qt::Orientation mConstraintOrientation;
};

#endif // GALLERYMODEL_H
