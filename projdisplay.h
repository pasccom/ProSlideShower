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

#ifndef PROJDISPLAY_H
#define PROJDISPLAY_H

#include <QWidget>

class PresModel;

/*!
 * \brief The ProjDisplay class displays the slides
 *
 * This class displays the current slide in the PresModel.
 * The displayed slide is not necessarily the current one,
 * there may be an offset (user adjustable through context menu).
 * The whole slide may not be displayed, you can choose to display
 * only a part of the slide, a virtual screen. This is especially
 * useful when showing LaTeX Beamer slides.
 * You can choose both the horizontal and the vertical virtual screen
 * to display.
 */
class ProjDisplay : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Constructor with user-provided model.
     *
     * Constructs an instance with the user-provided model.
     * \note The class does not take ownership of the model.
     * It must be deleted by the user.
     * \param model The presentation model.
     * \param parent The parent widget of the display.
     * \sa ProjDisplay(const QString&, QWidget*)
     */
    ProjDisplay(PresModel* model, QWidget* parent = NULL);
    /*!
     * \brief Constructor from presentation file path.
     *
     * Constructs an instance showing the slides in the given file.
     * It internally constructs a presentation model for the file.
     * \note The class has ownership of the model and does never yiels it.
     * \param path The path to the presentation file.
     * \param parent The parent widget of the display.
     * \sa ProjDisplay(PresModel*, QWidget*)
     */
    ProjDisplay(const QString& path, QWidget* parent = NULL);
    /*!
     * \brief Destructor
     *
     * Desallocates the model, when owned by the instance
     * (i.e., created internally from a presentation file path).
     */
    ~ProjDisplay(void);

    /*!
     * \brief Set a user-provided model
     *
     * After this call the given model will be used in the presentation.
     * \note The class does not take ownership of the model.
     * It must be deleted by the user.
     * \param model The presentation model.
     * \sa setModel(const QString&), model()
     */
    void setModel(PresModel* model);
    /*!
     * \brief Create a model from the presentation file path.
     *
     * Internally constucts a model to show the slides contained
     * in the given presentation file.
     * \note The class has ownership of the model and does never yiels it.
     * \param path The path to the presentation file.
     * \sa setModel(PresModel*), model()
     */
    void setModel(const QString& path);
    /*!
     * \brief The presentation model.
     *
     * Returns the current presentation model.
     * \return The presentation model.
     * \sa setModel(PresModel*), setModel(const QString&)
     */
    inline PresModel* model(void) const {return mModel;}

    /*!
     * \brief Set the offset of this display
     *
     * The display does not always displays the current slide.
     * Instead it can display any other one given with an offset
     * from the current slide in the model.
     * If no such slide exists, it does no display anything.
     * \param offset The offset for this display (positive or negative).
     * \sa offset()
     */
    inline void setOffset(int offset) {mOffset = offset;}
    /*!
     * \brief The offset for this display.
     *
     * Returns the offset of the slides shown on this display
     * with respect to the current slide in the model.
     * \return The offset for this display.
     * \sa setOffset()
     */
    inline int offset(void) const {return mOffset;}

    /*!
     * \brief The horizontal virtual screen for this display
     *
     * Returns the horizontal virtual screen shown on this display.
     * \return The horizontal virtual screen for this display.
     * \sa setHorizontalVirtualScreen(), verticalVirtualScreen()
     */
    inline int horizontalVirtualScreen(void) const {return mHVirtualScreen;}
    /*!
     * \brief The vertical virtual screen for this display
     *
     * Returns the vertical virtual screen shown on this display.
     * \return The vertical virtual screen for this display.
     * \sa setVericalVirtualScreen(), horizontalVirtualScreen()
     */
    inline int verticalVirtualScreen(void) const {return mVVirtualScreen;}
public slots:
    /*!
     * \brief Set the horizontal virtual screen shown on this display.
     *
     * Sets the horizontal virtual screen shown on this display.
     * The horizontal virtual screen should be less than the number
     * of horizontal virtual screens in the PresModel and greater than 0.
     *
     * \internal
     * Actually the number of virtual screens might be -1.
     * In this case, the slot tries to determine the horizontal virtual screen
     * using the sender action user data. This is only meant to be used internally.
     * \endinternal
     *
     * \param horizontal The horizontal virtual screen for this display.
     * \sa horizontalVirtualScreen(), setVericalVirtualScreen()
     */
    void setHorizontalVirtualScreen(int horizontal = -1);
    /*!
     * \brief Set the vertical virtual screen shown on this display.
     *
     * Sets the horizverticalontal virtual screen shown on this display.
     * The vertical virtual screen should be less than the number
     * of vertical virtual screens in the PresModel and greater than 0.
     *
     * \internal
     * Actually the number of virtual screens might be -1.
     * In this case, the slot tries to determine the vertical virtual screen
     * using the sender action user data. This is only meant to be used internally.
     * \endinternal
     *
     * \param vertical The vertical virtual screen for this display.
     * \sa verticalVirtualScreen(), setHorizontalVirtualScreen()
     */
    void setVericalVirtualScreen(int vertical = -1);
protected:
    /*!
     * \internal
     * \brief Paint event
     *
     * Repaints the display: Fetch the image in the PresModel and draws it.
     * \param pe The paint event.
     */
    void paintEvent(QPaintEvent *pe);
private:
    /*!
     * \brief Connect to the model.
     *
     * Connects the state change signals of the model
     * to trigger updates of the display.
     * \sa disconnectFromModel()
     */
    void connectToModel(void);
    /*!
     * \brief Disconnect from the model.
     *
     * Disconnects the state change signals of the model
     * from the update slot of the display.
     * \sa connectToModel()
     */
    void disconnectFromModel(void);

    PresModel* mModel;      /*!< The model from which the slides are fetched. */
    bool mModelOwned;       /*!< Whether the model is owned by the display. */
    int mOffset;            /*!< The offset for this display. */
    int mHVirtualScreen;    /*!< The horizontal virtual screen for this display. */
    int mVVirtualScreen;    /*!< The vertical virtual screen for this display. */
};

#endif // PROJDISPLAY_H
