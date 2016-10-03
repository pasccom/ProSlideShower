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

#ifndef PRESSTYLE_H
#define PRESSTYLE_H

#include <QProxyStyle>

/*!
 * \brief The PresStyle class is a style for painting presentation widgets.
 *
 * This class defines a style to paint nice presentation widgets,
 * which are easy to read, but do no catch the eye too much.
 * The default wiget background is black while the foreground is white.
 *
 * Currently, only the progress bar drawing is tuned.
 */
class PresStyle : public QProxyStyle
{
public:
    /*!
     * \brief Constructor
     *
     * Calls the parent constructor.
     * \param base The base style.
     */
    inline PresStyle(QStyle* base = NULL) :
        QProxyStyle(base) {}
#if QT_VERSION >= 0x050000
    /*!
     * \brief Constructor
     *
     * Calls the parent constructor.
     * \param key The kay for the base style.
     */
    inline PresStyle(const QString& key) :
        QProxyStyle(key) {}
#endif
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget = NULL) const;
    /*!
     * \brief Draw a control
     *
     * This function overrides the base function.
     * It only tunes the draving of the progress bar (label, groove and contents).
     * See <tt>QStyle::drawControl()</tt> for more information.
     * \param element The element to draw.
     * \param option Options for drawing the element.
     * \param painter Painter to be used when drawing.
     * \param widget An aid for drawing the control.
     */
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = NULL) const;
};

#endif // PRESSTYLE_H
