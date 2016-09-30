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
