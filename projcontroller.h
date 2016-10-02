#ifndef PROJCONTROLLER_H
#define PROJCONTROLLER_H

#include <QWidget>
#include "subdisplayhandler.h"
#include "presstyle.h"
#include "projcontrollerpane.h"
#include <QTimer>
#include <QTime>

class PresModel;
class ProjDisplay;
class QSplitter;
class QLabel;
class QProgressBar;

/*!
 * \brief The ProjController describes a widget controller for the presentation
 *
 * This class allows to control easily the unfolding of the presentation.
 * It is only useful to the presenter and is not shown to the audience.
 *
 * It displays up to three ProjDisplay in a splitter, so that uneeded views
 * can easily be closed. It also shows progress bars for the presentation
 * and the time. The color of the bars changes depending on whether the presentation
 * is too fast or too slow. Finally, there is a miniature gallery, which allows to
 * easily reach the desired slide in the presentation.
 */
class ProjController : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construnctor
     *
     * Initializes the members and allocates the resources.
     * \param model The PresModel being presented.
     * \param parent The parent widget.
     */
    ProjController(PresModel *model, QWidget *parent = NULL);
    /*!
     * \brief Destructor
     *
     * Deletes the style and saves presentation time in settings.
     */
    ~ProjController(void);

    /*!
     * \brief Set the model
     *
     * Sets the PresModel from which slides are fetched.
     * \param model The PresModel being presented.
     * \sa model()
     */
    void setModel(PresModel* model);
    /*!
     * \brief Get the model
     *
     * Gets the PresModel from which slides are fetched.
     * \return The PresModel being presented.
     * \sa setModel()
     */
    inline PresModel* model(void) const {return mDisplays->model();}

    /*!
     * \brief Set time format
     *
     * Sets the format in which the time is displayed.
     * \param format The time format to use.
     * \sa timeFormat()
     */
    inline void setTimeFormat(const QString& format) {mFormat = format; updateTime();}
    /*!
     * \brief Get time format
     *
     * Gets the format in which the time is displayed.
     * \return The time format used.
     * \sa setTimeFormat()
     */
    inline QString timeFormat(void) const {return mFormat;}

    /*!
     * \brief Set presentation duration
     *
     * Sets the total presentation duration.
     * \param length The time of the presentation.
     * \sa totalTime()
     */
    inline void setTotalTime(const QTime& length) {mTotalTime = length; updateTime();}
    /*!
     * \brief Get presentation duration
     *
     * Gets the total presentation duration.
     * \return The time of the presentation.
     * \sa setTotalTime()
     */
    inline QTime totalTime(void) const {return mTotalTime;}

    /*!
     * \brief Set height of gallery panel
     *
     * Sets the height of the auto-hide gallery panel
     * displaying the miniatures of the slides in the presentation.
     * \param height The height of the gallery panel to use.
     * \sa paneHeight()
     */
    inline void setPaneHeight(int height) {mPane->setFixedHeight(height);}
    /*!
     * \brief Get height of gallery panel
     *
     * Gets the height of the auto-hide gallery panel
     * displaying the miniatures of the slides in the presentation.
     * \return The height of the gallery panel in use.
     * \sa setPaneHeight()
     */
    inline int paneHeight(void) const {return mPane->height();}
    /*!
     * \brief Whether gallery panel is shown
     *
     * Tells whether the auto-hide gallery panel displaying
     * the miniatures of the slides in the presentation is being shown.
     * \return \c true, if it is shown, \c false otherwise.
     * \sa showControlPane(), hideControlPane()
     */
    inline bool isPaneShown(void) const {return mPane->isVisible();}
public slots:
    /*!
     * \brief Go to next page
     *
     * Tells the SubDisplayHandler to switch to the next page,
     * which changes the current page in the PresModel.
     *
     * Also start the presentation, it it is not already started.
     * \sa goToPrevPage(), SubDisplayHandler::goToNextPage()
     */
    inline void goToNextPage() {mDisplays->goToNextPage(); start();}
    /*!
     * \brief Go to previous page
     *
     * Tells the SubDisplayHandler to switch to the previous page,
     * which changes the current page in the PresModel.
     * \sa goToNextPage(), SubDisplayHandler::goToPrevPage()
     */
    inline void goToPrevPage() {mDisplays->goToPrevPage();}

    /*!
     * \brief Start the presentation
     *
     * Starts the presentation, which means that the presentation time
     * begins to increase.
     * \sa stop(), pause()
     */
    inline void start(void) {if (!mTimer->isActive()) mTimer->start();}
    /*!
     * \brief Pause the presentation
     *
     * Pauses the presentation, which means that the presentation time
     * stops increasing but is not reset.
     * \sa start(), stop()
     */
    inline void pause(void) {if (mTimer->isActive()) mTimer->stop();}
    /*!
     * \brief Stop the presentation
     *
     * Stops the prsentation, which means that the presentation time
     * is reset.
     * \sa start(),  pause()
     */
    void stop(void);

    /*!
     * \brief Show the gallery pane
     *
     * Shows the auto-hide gallery panel displaying
     * the miniatures of the slides in the presentation.
     * \sa hideControlPane()
     */
    inline void showControlPane(void) {mPane->show();}
    /*!
     * \brief Hide the gallery pane
     *
     * Hides the auto-hide gallery panel displaying
     * the miniatures of the slides in the presentation.
     * \sa showControlPane()
     */
    inline void hideControlPane(void) {mPane->hide();}
    /*!
     * \brief Configuration
     *
     * Opens a dialog to configure the presentation.
     * Currently only the total presentation time can be configured.
     */
    void configure(void);
private slots:
    /*!
     * \brief Updates the presentation time
     *
     * This slot is triggerred every second when the presentation is running.
     * It updates the the presentation time.
     */
    inline void handleSecondStep(void) {mTime = mTime.addSecs(1); updateTime();}
    /*!
     * \brief Handle change of document
     *
     * This slot updates the controller when the document is changed in the PresModel.
     * \sa handleFrameChange()
     */
    void handleDocumentChange(void);
    /*!
     * \brief Handle change of frame
     *
     * This slot is called when the current frame (slide) is changed.
     * It updates the frame progress bar.
     * \sa handleDocumentChange()
     */
    void handleFrameChange(void);
private:
    /*!
     * \brief Updatesthe displays
     *
     * Triggers a repaint of the children ProjDisplay.
     */
    void updateDisplays(void);
    /*!
     * \brief Update presentation time
     *
     * Updates the presentation time progress bar.
     * \sa updateColor()
     */
    void updateTime(void);
    /*!
     * \brief Update presentation time
     *
     * Updates the presentation frame progress bar.
     * \sa updateColor()
     */
    void updateFrame(void);
    /*!
     * \brief Updates and repaints the progress bars
     *
     * Updates the color of the progress bars depending on
     * whether the presentation is too fast or too slow
     * and triggers the repaint.
     * \sa updateTime(), updateFrame()
     */
    void updateColor(void);

    SubDisplayHandler *mDisplays;   /*!< The handler of the children ProjDisplay. */
    QSplitter* mSplitter;           /*!< The splitter which contains the children ProjDisplay. */
    ProjControllerPane* mPane;      /*!< The auto-hide pane contraining the miniature gallery for the presentation slides. */
    QLabel* mTimeLabel;             /*!< The time label displaying the presentation time. */
    QLabel* mSlideLabel;            /*!< The slide label diplaying the current slide (and also the total number of slides in the presentation). */
    QProgressBar* mSlideProgress;   /*!< The progress bar for the slides. */
    QProgressBar* mTimeProgress;    /*!< The progress bar for the time. */

    QTimer* mTimer;                 /*!< The timer which increases the presentation time every second. */
    QTime mTime;                    /*!< The presentation time. */
    QTime mTotalTime;               /*!< The total duration of the presentation. */
    QString mFormat;                /*!< The format in which the presentation time will be displayed. */
};

#endif // PROJCONTROLLER_H
