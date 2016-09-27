#ifndef PROJCONTROLLERPANE_H
#define PROJCONTROLLERPANE_H

#include <QWidget>
#include "galleryview.h"

class QToolButton;
class PresModel;

/*!
 * \brief The ProjControllerPane class describes a auto-hide pane showing slide miniatures.
 *
 * This class allows to navigate very easily in the presentation
 * by double-clicking the miniature of the target slide.
 * It shows the miniatures of all the slides in a horizontal GalleryView.
 * It also proposes some buttons:
 *  \li A close button, which closes all the windows of the application.
 *  \li An open master document button, which allows to open a master document.
 *
 * The code for the auto-hide feature is located in the ProjManager.
 * It shows this widget when the mouse approaches and hides it
 * when the mouse leaves it.
 */
class ProjControllerPane : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Constructor
     *
     * Initializes the members and allocates the resources.
     * \param parent The parent widget.
     */
    ProjControllerPane(QWidget* parent = NULL);

    /*!
     * \brief Set the model
     *
     * Sets the PresModel from which slides are fetched.
     * \param model The PresModel being presented.
     * \sa model()
     */
    void setModel(PresModel *model);
    /*!
     * \brief Get the model
     *
     * Gets the PresModel from which slides are fetched.
     * \return The PresModel being presented.
     * \sa setModel()
     */
    inline QAbstractItemModel* model(void) const {return mSlidesView->model();}
signals:
    /*!
     * \brief Open master document request
     *
     * This signal is emitted when the user clicks
     * the open master document button.
     */
    void openDocumentRequest(void);
protected:
    /*!
     * \internal
     * \brief Event filter
     *
     * Filters the events originating from children widget.
     * This hack is necessary for children widget tooltips to appear.
     *
     * \param watched The object on which the event was filtered.
     * \param event The filtered event.
     * \return \c true if the event do not need further processing, \c false otherwise.
     */
    bool eventFilter(QObject* watched, QEvent* event);
private slots:
    /*!
     * \brief Handles a slide change
     *
     * This slot is called when the current slide is changed (e.g. with arrow keys).
     * It synchronizes the gallery view with the current slide in the presentation.
     */
    void handlePageChange(void);
private:
    GalleryView* mSlidesView;   /*!< The gallery view displaying slides miniatures .*/
    QToolButton* mOpenButton;   /*!< Open master document button. */
    QToolButton* mConfigButton; /*!< Configuration button (nothing implemented yet). */
    QToolButton* mCloseButton;  /*!< Close button. Closes the whole application. */
};

#endif // PROJCONTROLLERPANE_H
