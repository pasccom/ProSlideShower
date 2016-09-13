#ifndef PROJMANAGER_H
#define PROJMANAGER_H

#include "subdisplayhandler.h"

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
