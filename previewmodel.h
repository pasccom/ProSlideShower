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

#ifndef GALLERYMODEL_H
#define GALLERYMODEL_H

#include <QAbstractListModel>
#include <QVector>
#include <QStringList>
#include <QSize>
#include <QtDebug>

/*!
 * \brief The PreviewModel class is a model for GallleryView.
 *
 * This class implements a model suitable for GalleryView.
 * It only defines a constraint for the horizontal or vertical size
 * of the preview miniature (returned when PreviewRole is used).
 */
class PreviewModel : public QAbstractListModel
{
    Q_OBJECT
public:
    /*!
     * \brief Special roles
     *
     * Special roles used by the GalleryView,
     * that a PreviewModel should implement.
     */
    enum {
        /*!
         * \brief Role for miniature
         *
         * When this role is used, the model should return an image
         * respecting the preview constraint (see setPreviewConstraint()).
         */
        PreviewRole = Qt::UserRole
   };

    /*!
     * \brief Constructor
     *
     * Only call the parent constructor.
     * \param parent The parent object.
     */
    inline PreviewModel(QObject *parent = 0) :
        QAbstractListModel(parent) {}

    /*!
     * \brief Set the preview constraint
     *
     * Set the preview constraint and its orientation (the constrained dimension)
     * \param dim The value of the constraint (in pixels)
     * \param orientation The constrained dimension (<tt>Qt::Vertical</tt> for the height, <tt>Qt::Horizontal</tt> for the width).
     */
    virtual inline void setPreviewConstraint(int dim, Qt::Orientation orientation = Qt::Vertical)
    {
        mConstraintDim = dim;
        mConstraintOrientation = orientation;
        qDebug() << "Constraint: " << dim;
    }
protected:
    int mConstraintDim;                     /*!< \internal The value of the constraint */
    Qt::Orientation mConstraintOrientation; /*!< \internal The constrained dimension (<tt>Qt::Vertical</tt> for the height, <tt>Qt::Horizontal</tt> for the width). */
};

#endif // GALLERYMODEL_H
