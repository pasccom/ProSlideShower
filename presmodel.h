#ifndef PRESMODEL_H
#define PRESMODEL_H

#include "previewmodel.h"
#include <QImage>
#include <QList>

namespace Poppler {
    class Document;
}

/*!
 * \brief The PresModel class implements a model for a presentation.
 *
 * This class is a model for a presentation document.
 * It can handle any PDF file, but it is especially designed
 * for LaTeX Beamer generated files, since it can retrieve
 * the real slide number (not taking into account overlays and
 * other Beamer tricks generating multiple slides) by parsing
 * the <tt>*.nav</tt> or the <tt>*.aux</tt> file.
 *
 * It also handle the concept of virtual screen introduced
 * by Beamer. It can handle both horizontal vitual screens
 * and vertical virtual screens.
 *
 * In the backend it uses the open-source PDF rendering library
 * <a href="https://poppler.freedesktop.org/">Poppler</a>.
 */
class PresModel : public PreviewModel
{
    Q_OBJECT
public:
    /*!
     * \brief Constructor
     *
     * Initializes the members and load() the file.
     * \param file A presentation file to load in the model.
     * \param parent The parent object.
     */
    PresModel(const QString& file, QObject* parent = NULL);
    /*!
     * \brief Destructor
     *
     * Desallocates the internal <tt>Popper::Document</tt>.
     * */
    ~PresModel(void);
    /*!
     * \brief Whether the model has no error
     *
     * Tells whether the model sucessfully loaded the document
     * and has no erros.
     * \return \c true, when it has no errors, \c false otherwise.
     */
    inline bool isOK(void) const {return (mDoc != NULL);}

    /*!
     * \brief Get current page number
     *
     * Returns number of the current document page.
     * \return The current page number.
     * \sa setCurrentPageNumber(), getCurentIndex(), getCurrentFrameNumber()
     */
    inline int getCurrentPageNumber(void) const {return mCurrentPage;}
    /*!
     * \brief Get the current model index
     *
     * Get the model index corresponding the the current page.
     * \return The model index for the current page.
     * \sa setCurrentPage(), getCurrentPageNumber()
     */
    inline QModelIndex getCurentIndex(void) const {return index(mCurrentPage);}
    /*!
     * \brief Get total number of pages
     *
     * Gets the total number of pages in the document.
     * \return The total number of pages.
     * \sa getFrameCount()
     */
    int getPageCount(void) const;

    /*!
     * \brief Get current frame number
     *
     * Return the number of the current Beamer frame.
     * \note This number may be the same for multiple document pages
     * in cases of overlays for instance.
     * \return The current Beamer frame number.
     * \sa getCurrentPageNumber()
     */
    int getCurrentFrameNumber(void) const;
    /*!
     * \brief Get total number of frames
     *
     * Gets the total number of Beamer frames.
     * \note This number may not correspond to the number of
     * document pages in the case of overlays for instance.
     * \return The number of Beamer frames.
     * \sa getPageCount()
     */
    inline int getFrameCount(void) const {return mTotalFrames == 0 ? getPageCount() : mTotalFrames;}

    /*!
     * \brief Set number of virtual screens
     *
     * Sets the number of horizontal and vertical virtual screens.
     * \param horizontal The number of horizontal vitural screens.
     * \param vertical The number of vertical vitural screens.
     * \sa horizontalVirtualScreens(), verticalVirtualScreens()
     */
    void setVirtualScreens(int horizontal, int vertical = 1);
    /*!
     * \brief Get the number of horizontal virtual screens.
     *
     * Returns the number of horizontal virtual screens.
     * \return The number of horizontal virtual screens.
     * \sa setVirtualScreens(), verticalVirtualScreens()
     */
    inline int horizontalVirtualScreens(void) const {return mHVirtualScreens;}
    /*!
     * \brief Get the number of vertical virtual screens.
     *
     * Returns the number of vertical virtual screens.
     * \return The number of vertical virtual screens.
     * \sa setVirtualScreens(), horizontalVirtualScreens()
     */
    inline int verticalVirtualScreens(void) const {return mVVirtualScreens;}

    /*!
     * \brief Get a page
     *
     * Returns the image of the page at given number, for the given virtual sceens,
     * which fits inside the given bounding rect.
     * \param number The number of the page.
     * \param boundingRect A rectangle which defines the size of the desired page (the aspect ratio is always kept).
     * \param horizontalVirtualScreen The number of the horizontal virtual screen.
     * \param verticalVirtualScreen The number of the vertical virtual screen.
     * \return The page image.
     * \sa getCurrentPage()
     */
    QImage getPage(int number, const QRect& boundingRect, int horizontalVirtualScreen = 1, int verticalVirtualScreen = 1) const;
    /*!
     * \brief Get the current page
     *
     * Returns the image of the current page up to the given offset, for the given virtual sceens,
     * which fits inside the given bounding rect.
     * \param boundingRect A rectangle which defines the size of the desired page (the aspect ratio is always kept).
     * \param horizontalVirtualScreen The number of the horizontal virtual screen.
     * \param verticalVirtualScreen The number of the vertical virtual screen.
     * \param offset The offset of the desired page.
     * \return The current page image.
     * \sa getPage(), getNextPage(), getPrevPage()
     */
    inline QImage getCurrentPage(const QRect& boundingRect, int horizontalVirtualScreen = 1, int verticalVirtualScreen = 1, int offset = 0) const {return getPage(mCurrentPage + offset, boundingRect, horizontalVirtualScreen, verticalVirtualScreen);}
    /*!
     * \brief Get the next page
     *
     * Returns the image of the next page, for the given virtual sceens,
     * which fits inside the given bounding rect.
     * \param boundingRect A rectangle which defines the size of the desired page (the aspect ratio is always kept).
     * \param horizontalVirtualScreen The number of the horizontal virtual screen.
     * \param verticalVirtualScreen The number of the vertical virtual screen.
     * \return The next page image.
     * \sa getPage(), getCurrentPage(), getPrevPage()
     */
    inline QImage getNextPage(const QRect& boundingRect, int horizontalVirtualScreen = 1, int verticalVirtualScreen = 1) const {return getPage(mCurrentPage + 1, boundingRect, horizontalVirtualScreen, verticalVirtualScreen);}
    /*!
     * \brief Get the previous page
     *
     * Returns the image of the previous page, for the given virtual sceens,
     * which fits inside the given bounding rect.
     * \param boundingRect A rectangle which defines the size of the desired page (the aspect ratio is always kept).
     * \param horizontalVirtualScreen The number of the horizontal virtual screen.
     * \param verticalVirtualScreen The number of the vertical virtual screen.
     * \return The previous page image.
     * \sa getPage(), getCurrentPage(), getNextPage()
     */
    inline QImage getPrevPage(const QRect& boundingRect, int horizontalVirtualScreen = 1, int verticalVirtualScreen = 1) const {return getPage(mCurrentPage - 1, boundingRect, horizontalVirtualScreen, verticalVirtualScreen);}

    /*!
     * \brief Get number of rows for given index
     *
     * Returns the number of rows in the model.
     * It corresponds to the number of pages in the presentation for the root index
     * and is \c 0 for all other indexes.
     * \param parent The parent index.
     * \return The number of rows for the given index.
     * \sa getPageCount()
     */
    int rowCount(const QModelIndex& parent = QModelIndex()) const {return !parent.isValid() ? getPageCount() : 0;}
    /*!
     * \brief Return data for given index
     *
     * Returns the data for the given index and the given role.
     * Currently only Qt::DisplayRole, Qt::SizeHintRole and PreviewModel::PreviewRole
     * are supported.
     * \param index A valid model index.
     * \param role A item data role.
     * \return The data associated with given role for given index.
     */
    QVariant data(const QModelIndex& index, int role = PreviewRole) const;
public slots:
    /*!
     * \brief Loads a file in the model
     *
     * Uses Poppler backend to parse the PDF file and
     * load it in the model. If this function succeeds,
     * the model isOK().
     * \param file A PDF file to load.
     * \return \c true if the PDF file was sucessfully loaded, \c false otherwise.
     */
    bool load(const QString& file);
    /*!
     * \brief Set the current page number
     *
     * Moves the presentation to the given page number.
     * \param page The new current page number.
     * \return \c true if the given page exists, \c false otherwise.
     * \sa getCurrentPageNumber()
     */
    bool setCurrentPageNumber(int page);
    /*!
     * \brief Set current page
     *
     * Moves the presentation to the given page index.
     * \param index A valid model index.
     * \sa getCurentIndex()
     */
    inline void setCurrentPage(const QModelIndex& index) {Q_ASSERT(setCurrentPageNumber(index.row()));}
    /*!
     * \brief Go to next page
     *
     * Moves the presentation to next page.
     * \return \c true, when there is a next page, \c false when the current page is the last one.
     * \sa goToPrevPage()
     */
    inline bool goToNextPage(void) {return setCurrentPageNumber(mCurrentPage + 1);}
    /*!
     * \brief Go to previous page
     *
     * Moves the presentation to previous page.
     * \return \c true, when there is a previous page, \c false when the current page is the first one.
     * \sa goToNextPage()
     */
    inline bool goToPrevPage(void) {return setCurrentPageNumber(mCurrentPage - 1);}
signals:
    /*!
     * \brief Emitted when current page changes
     *
     * This signal is emitted when the current page is changed.
     * \sa setCurrentPageNumber(), setCurrentPage(), currentFrameChanged()
     */
    void currentPageChanged(void);
    /*!
     * \brief Emitted when current frame changes
     *
     * This signal is emitted when the current frame is changed.
     * \sa currentPageChanged()
     */
    void currentFrameChanged(void);
    /*!
     * \brief Emitted when virtual screen setup is changed
     *
     * This signal is emitted when the number of
     * horizontal or vertical virtual sceens is changed.
     * \sa setVirtualScreens()
     */
    void virtualScreenNumberChanged(void);
    /*!
     * \brief Emitted when the presentation document is changed.
     *
     * This signal is emitted when the underlying document is changed.
     * \sa load()
     */
    void documentChanged(void);
private:
    /*!
     * \brief Parse <tt>*.nav</tt> file
     *
     * Parses the <tt>*.nav</tt> file associated with the Beamer presentation
     * (same name as the PDF document, up to the extension) to get the frame limits.
     * \param pdfFilePath PDF file path.
     * \return \c true on sucess, \c false when the file does not exist or cannob be opened.
     * \sa parseAuxFile()
     */
    bool parseNavFile(const QString& pdfFilePath);
    /*!
     * \brief Parse <tt>*.nav</tt> file line.
     *
     * Called by parseNavFile() on each line of the <tt>*.nav</tt> file.
     * \param line The line of the <tt>*.nav</tt> file.
     */
    void parseNavLine(const QString& line);
    /*!
     * \brief Parse <tt>*.aux</tt> file
     *
     * Parses the <tt>*.aux</tt> file associated with the Beamer presentation
     * (same name as the PDF document, up to the extension) to get the frame limits.
     * \param pdfFilePath PDF file path.
     * \return \c true on sucess, \c false when the file does not exist or cannob be opened.
     * \sa parseNavFile()
     */
    bool parseAuxFile(const QString& pdfFilePath);
    /*!
     * \brief Parse <tt>*.aux</tt> file line.
     *
     * Called by parseAuxFile() on each line of the <tt>*.aux</tt> file.
     * \param line The line of the <tt>*.aux</tt> file.
     */
    void parseAuxLine(const QString& line);

    Poppler::Document *mDoc;    /*!< The underlying <tt>Poppler::Document</tt>. */
    QList<int> mFramePages;     /*!< Correspondance between pages and frames. */

    int mTotalFrames;           /*!< Total number of frames in presentation */
    int mCurrentPage;           /*!< The current page number. */

    int mHVirtualScreens;       /*!< The number of horizontal virtual sceens. */
    int mVVirtualScreens;       /*!< The number of vertical virtual sceens. */

    bool mInAppendix;           /*!< The parser is currently in the appendix. */
};

#endif // PRESMODEL_H
