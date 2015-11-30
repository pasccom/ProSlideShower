#ifndef PRESMODEL_H
#define PRESMODEL_H

#include <QObject>
#include <QImage>
#include <QList>

namespace Poppler {
    class Document;
}

class PresModel :public QObject
{
    Q_OBJECT
public:
    PresModel(const QString& file, QObject* parent = NULL);
    ~PresModel(void);
    bool load(const QString& file);
    inline bool isOK(void) const {return (mDoc != NULL);}

    inline int getCurrentPageNumber(void) const {return mCurrentPage;}
    inline int getCurrentFrameNumber(void) const {return mFramePages.isEmpty() ? getCurrentPageNumber() + 1 : mFramePages.at(mCurrentPage);}
    int getPageCount(void) const;
    inline int getFrameCount(void) const {return mTotalFrames == 0 ? getPageCount() : mTotalFrames;}

    QImage getPage(int number, const QRect& boundingRect, int horizontalVirtualScreen = 1, int verticalVirtualScreen = 1) const;
    inline QImage getCurrentPage(const QRect& boundingRect, int horizontalVirtualScreen = 1, int verticalVirtualScreen = 1) const {return getPage(mCurrentPage, boundingRect, horizontalVirtualScreen, verticalVirtualScreen);}
    inline QImage getNextPage(const QRect& boundingRect, int horizontalVirtualScreen = 1, int verticalVirtualScreen = 1) const {return getPage(mCurrentPage + 1, boundingRect, horizontalVirtualScreen, verticalVirtualScreen);}
    inline QImage getPrevPage(const QRect& boundingRect, int horizontalVirtualScreen = 1, int verticalVirtualScreen = 1) const {return getPage(mCurrentPage - 1, boundingRect, horizontalVirtualScreen, verticalVirtualScreen);}

    void setVirtualScreens(int horizontal, int vertical = 1);
    inline int horizontalVirtualScreens(void) const {return mHVirtualScreens;}
    inline int verticalVirtualScreens(void) const {return mVVirtualScreens;}

public slots:
    bool setCurrentPageNumber(int page);
    inline bool goToNextPage(void) {return setCurrentPageNumber(mCurrentPage + 1);}
    inline bool goToPrevPage(void) {return setCurrentPageNumber(mCurrentPage - 1);}
signals:
    void currentPageChanged(void);
private:
    bool parseNavFile(const QString& pdfFilePath);
    void parseNavLine(const QString& line);
    bool parseAuxFile(const QString& pdfFilePath);
    void parseAuxLine(const QString& line);

    Poppler::Document *mDoc;
    QList<int> mFramePages;
    int mTotalFrames;
    bool mInAppendix;

    int mCurrentPage;
    int mHVirtualScreens;
    int mVVirtualScreens;
};

#endif // PRESMODEL_H
