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

#ifndef PROJMANAGER_H
#define PROJMANAGER_H

#include "subdisplayhandler.h"
#include "projcontroller.h"

#include <QTime>

class ProjController;
class QKeyEvent;
class QMouseEvent;

/*!
 * \brief The ProjManager class manages the display layout
 *
 * This class dispatches the displays between the available screens.
 * Most probably your setup will fall into one of the two following cases
 *  \li You have only one screen: A slide display (see ProjDisplay class)
 * will fill the whole screen, no subsequent information is added
 *  \li You are connected to a projector (or a secondary display):
 * A slide display (see ProjDisplay class) will fill the secondary screen,
 * while a presentation controller (see ProjController class) fills the first.
 *
 * But more complex display setups can also be handled. There is no limit to the
 * number of auxiliary screens, the controller is always shown on the primary screen.
 *
 * This class is also in charge of handling global events, which includes:
 *  \li Navigation key events: <tt>Page down</tt>, <tt>Down</tt>, <tt>Right</tt> to go forward and
 * <tt>Page up</tt>, <tt>Up</tt>, <tt>Left</tt> to go backward.
 *  \li Showing or hiding the miniatures (see ProjControllerPane class).
 */
class ProjManager : public SubDisplayHandler
{
    Q_OBJECT
public:
    /*!
     * \brief Constructor
     *
     * Constructs all the needed objects (ProjDisplay for showing slides
     * and a ProjController if needed)
     * \param parent The parent of the object (usually \c null_ptr)
     */
    ProjManager(QObject *parent = NULL);
    /*!
     * \brief Destructor
     *
     * Desallocates ressources not handled by Qt (the ProjController).
     */
    ~ProjManager(void);

    /*!
     * \brief Set the master model
     *
     * Sets the master model used by all children ProjDisplay.
     * \note It may be \c NULL, to deactivate display.
     * \param model The model to be used as master.
     * \sa model()
     */
    void setModel(PresModel* model);

    /*!
     * \brief Set presentation duration
     *
     * Sets the total presentation duration.
     * \param length The time of the presentation.
     * \sa totalTime()
     */
    inline void setTotalTime(const QTime& length) {if (mController != NULL) mController->setTotalTime(length);}
    /*!
     * \brief Get presentation duration
     *
     * Gets the total presentation duration.
     * \return The time of the presentation.
     * \sa setTotalTime()
     */
    inline QTime totalTime(void) const {return (mController != NULL) ? mController->totalTime() : QTime();}
protected:
    /*!
     * \internal
     * \brief Filter for application-wide events
     *
     * This method is called whenever an event occurs at the application level.
     * It redirects appropriately the key press, key release and mouse move events
     * to the methods keyPressEvent(), keyReleaseEvent() and mouseMoveEvent()
     * depending on the event type. Also intercepts close event close all windows
     * whenever <tt>Alt+F4</tt> is pressed (i.e. whenever one window is closed).
     * \param watched The watched object upon which the event acted.
     * \param event The event recieved by the object.
     * \return \c true if the event is accepted
     */
    bool eventFilter(QObject* watched, QEvent* event);
    /*!
     * \internal
     * \brief Key press event callback
     *
     * This function is called when the user presses a key.
     * It intercepts <tt>Page down</tt>, <tt>Down</tt>, <tt>Right</tt>
     * <tt>Page up</tt>, <tt>Up</tt> and <tt>Left</tt> which are used
     * to navigate in the slideshow.
     * \param ke The key press event.
     * \sa keyReleaseEvent()
     */
    void keyPressEvent(QKeyEvent *ke);
    /*!
     * \internal
     * \brief Key release event callback
     *
     * This function is called when the user releases a key.
     * It checks whetehr it is one of <tt>Page down</tt>, <tt>Down</tt> and <tt>Right</tt>
     * to move one slide forward or <tt>Page up</tt>, <tt>Up</tt> and <tt>Left</tt>
     * to move one slide backward.
     * \param ke The key release event.
     * \sa keyPressEvent()
     */
    void keyReleaseEvent(QKeyEvent* ke);
    /*!
     * \internal
     * \brief Mouse move event callback
     *
     * This function is called whenever the user moves the mouse.
     * It hides or displays the miniature view (see ProjControllerPane) when necessary.
     * \param me The mouse event.
     */
    void mouseMoveEvent(QMouseEvent *me);
private:

    ProjController* mController; /*!< A ProjController to control the slideshow. */
};

#endif // PROJMANAGER_H
