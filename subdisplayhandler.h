#ifndef SUBDISPLAYHANDLER_H
#define SUBDISPLAYHANDLER_H

#include <QObject>
#include <QVector>

class PresModel;
class ProjDisplay;

/*!
 * \brief The SubDisplayHandler class manages multiple ProjDisplay.
 *
 * This class defines generic method for objects controlling multiple
 * instances of ProjDisplay. It can be used a class member (as done in ProjController)
 * or as the class parent (as done in ProjManager).
 *
 * It defines the master model used by the children ProjDisplay,
 * as well as some functions to interact with it.
 *
 * It also handles the context menu actions for the ProjDisplay
 * and manages it.
 */
class SubDisplayHandler : public QObject, public QVector<ProjDisplay *>
{
    Q_OBJECT
public:
    /*!
     * \brief Constructor
     *
     * Initializes the class members.
     * \param model The master model for the children ProjDisplay.
     * \param parent The parent object.
     */
    inline SubDisplayHandler(PresModel* model = NULL, QObject* parent = NULL):
        QObject(parent), QVector<ProjDisplay *>(), mModel(model), mParent(NULL) {}
    /*!
     * \brief Destructor
     *
     * Desallocates the children ProjDisplay.
     */
    virtual ~SubDisplayHandler(void);

    /*!
     * \brief Set parent widget
     *
     * Sets the parent widget used as a parent for the dialogs.
     * \param widget The widget to be used as parent.
     * \sa parentWidget()
     */
    inline void setParentWidget(QWidget* widget) {mParent = widget;}
    /*!
     * \brief Get parent widget
     *
     * Returns the widget used as parent by dialogs.
     * \return The widget used as parent.
     * \sa setParentWidget()
     */
    inline QWidget* parentWidget(void) const {return mParent;}

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
     * \brief Get the master model
     *
     * Returns the model used by all children ProjDisplay.
     * \return The model used as master.
     * \sa setModel()
     */
    inline PresModel* model() const {return mModel;}

    /*!
     * \brief Go to next page
     *
     * This function changes forward the current page in the slave models
     * (models not equal to the master model) used by children ProjDisplay.
     * \sa goToPrevPage()
     */
    void goToNextPage();
    /*!
     * \brief Go to prev page
     *
     * This function changes backward the current page in the slave models
     * (models not equal to the master model) used by children ProjDisplay.
     * \sa goToNextPage()
     */
    void goToPrevPage();

    /*!
     * \brief Update display actions
     *
     * Updates the actions shown in the context menu of the children ProjDisplay.
     * \internal
     * \sa updateDisplayActions(ProjDisplay*)
     * \endinternal
     */
    virtual void updateDisplayActions(void);
public slots:
    /*!
     * \brief Load file
     *
     * This slot asks the user for the file to load.
     * It then loads the file in the master model
     * \internal
     * \note Depending on the action which calls the slot,
     * the user-provided file may be loaded
     * in the master or the slave model.
     * \endinternal
     * \sa load(const QString&, int, int)
     */
    void load(void);
    /*!
     * \brief Load master file.
     *
     * Loads a file in master model with the given number of
     * horizontal and vertical virtual screens.
     * \internal
     * \note If the number of horizontal virtual screens is 0 or less,
     * handleVirtualScreens() is used to ask the user for the number of
     * horizontal virtual screens in the presentation.
     * \endinternal
     * \param file The path to the file to load in master model.
     * \param h The number of horizontal virtual screens (positive).
     * \param v The number of vertical virtual screens (strictly positive).
     * \sa load()
     */
    virtual void load(const QString& file, int h = 0, int v = 1);
private slots:
    /*!
     * \brief Let the user choose the number of horizontal virtual screens
     *
     * This slot show a prompt dialog where the user can choose the number of
     * horizontal virtual screens in the presentation.
     * \note Depending on the action which calls the slot,
     * it may act on the master or the slave model.
     */
    void handleVirtualScreens(void);
    /*!
     * \brief Let the user choose the number of vertical virtual screens
     *
     * This slot show a prompt dialog where the user can choose the offset.
     * \note Depending on the action which calls the slot,
     * it may act on the master or the slave model.
     */
    void handleOffset(void);
private:
    /*!
     * \brief Update display actions
     *
     * Updates display actions for the given ProjDisplay.
     * \param display A children ProjDisplay.
     * \sa updateDisplayActions()
     */
    void updateDisplayActions(ProjDisplay* display);

    PresModel* mModel;  /*!< The master model used by default by all children. */
    QWidget* mParent;   /*!< The widget used as parent for dialogs. */
};

#endif // SUBDISPLAYHANDLER_H
