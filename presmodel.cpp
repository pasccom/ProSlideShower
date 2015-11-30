#include "presmodel.h"

#if QT_VERSION >= 0x050000
#   include <poppler/qt5/poppler-qt5.h>
#elif QT_VERSION >= 0x040000
#   include <poppler/qt4/poppler-qt4.h>
#else
#   error "Qt version should be at least 4.0.0"
#endif

#include <QtCore>
#include <QtDebug>

PresModel::PresModel(const QString &file, QObject *parent)
    : QObject(parent), mDoc(NULL), mCurrentPage(0), mHVirtualScreens(1), mVVirtualScreens(1)
{
    mFramePages.clear();
    mTotalFrames = 0;

    load(file);
}

PresModel::~PresModel(void)
{
    if (mDoc != NULL)
        delete mDoc;
    mDoc = NULL;
}

bool PresModel::load(const QString& file)
{
    if (mDoc != NULL)
        delete mDoc;
    mDoc = NULL;
    mFramePages.clear();
    mTotalFrames = 0;

    if (file.isEmpty())
        return false;

    mDoc = Poppler::Document::load(file);

    if (mDoc ==  NULL)
        return false;
    if (mDoc->isLocked()) {
        delete mDoc;
        mDoc = NULL;
        return false;
    }

    mDoc->setRenderHint(Poppler::Document::TextAntialiasing);

    qDebug() << mDoc->metadata();
    qDebug() << mDoc->infoKeys();
    foreach (QString key, mDoc->infoKeys())
        qDebug() << key << ":" << mDoc->info(key);

    if (!parseNavFile(file))
        parseAuxFile(file);
    qDebug() << mTotalFrames << mFramePages;

    mCurrentPage = 0;
    emit currentPageChanged();
    return true;
}

void PresModel::setVirtualScreens(int horizontal, int vertical)
{
    Q_ASSERT(horizontal > 0);
    Q_ASSERT(vertical > 0);

    mHVirtualScreens = horizontal;
    mVVirtualScreens = vertical;
    emit currentPageChanged();
}

bool PresModel::setCurrentPageNumber(int page)
{
    if ((page < 0) || (page >= mDoc->numPages()))
        return false;

    mCurrentPage = page;
    emit currentPageChanged();
    return true;
}

QImage PresModel::getPage(int number, const QRect& boundingRect, int horizontalVirtualScreen, int verticalVirtualScreen) const
{
    Q_ASSERT((horizontalVirtualScreen > 0) && (horizontalVirtualScreen <= mHVirtualScreens));
    Q_ASSERT((verticalVirtualScreen > 0) && (verticalVirtualScreen <= mVVirtualScreens));

    if (mDoc == NULL)
        return QImage();

    Poppler::Page* page = mDoc->page(number);
    if (page == NULL)
        return QImage();

    double aspectRatio = qMin(boundingRect.width() * mHVirtualScreens / page->pageSizeF().width(), boundingRect.height() * mVVirtualScreens / page->pageSizeF().height());
    double pageWidth = page->pageSizeF().width()*aspectRatio;
    double pageHeight = page->pageSizeF().height()*aspectRatio;
    return page->renderToImage(72.*aspectRatio, 72.*aspectRatio,
                               (horizontalVirtualScreen - 1) * pageWidth / mHVirtualScreens,
                               (verticalVirtualScreen - 1) * pageHeight / mVVirtualScreens,
                               pageWidth / mHVirtualScreens, pageHeight/mVVirtualScreens);
}

int PresModel::getPageCount(void) const
{
    if (mDoc == NULL)
        return 0;

    return mDoc->numPages();
}

bool PresModel::parseNavFile(const QString& pdfFilePath)
{
    QString navFilePath = pdfFilePath;
    navFilePath.replace(QRegExp("\\.pdf$", Qt::CaseSensitive), ".nav");
    QFile navFile(navFilePath);
    qDebug() << navFilePath;

    if (!navFile.exists()) {
        qDebug() << "*.nav file does not exist";
        return false;
    }

    if (!navFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open *.nav file:" << navFile.errorString();
        return false;
    }

    mInAppendix = false;
    while (!navFile.atEnd() && !mInAppendix) {
        parseNavLine(QString::fromLocal8Bit(navFile.readLine()));
    }

    navFile.close();
    return true;
}

void PresModel::parseNavLine(const QString& line)
{
    QRegExp slideEntryRegExp("\\\\headcommand\\s*\\{\\\\slideentry\\s*\\{(\\d+)\\}\\{(\\d+)\\}\\{(\\d+)\\}\\{(\\d+)/(\\d+)\\}\\{(.*)\\}\\{(\\d+)\\}\\}");
    QRegExp appendixEntryRegExp("\\\\headcommand\\s*\\{\\\\partentry\\s*\\{Appendix\\}\\{(\\d+)\\}\\}");

    if (slideEntryRegExp.exactMatch(line.trimmed())) {
        int start = slideEntryRegExp.capturedTexts().at(4).toInt();
        int end = slideEntryRegExp.capturedTexts().at(5).toInt();

        mTotalFrames++;
        for (int i = start; i <= end; i++)
            mFramePages.append(mTotalFrames);
        return;
    }

    if (appendixEntryRegExp.exactMatch(line.trimmed()))
        mInAppendix = true;
}

bool PresModel::parseAuxFile(const QString& pdfFilePath)
{
    QString auxFilePath = pdfFilePath;
    auxFilePath.replace(QRegExp("\\.pdf$", Qt::CaseSensitive), ".aux");
    QFile auxFile(auxFilePath);
    qDebug() << auxFilePath;

    if (!auxFile.exists()) {
        qDebug() << "*.aux file does not exist";
        return false;
    }

    if (!auxFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open *.aux file:" << auxFile.errorString();
        return false;
    }

    mInAppendix = false;
    while (!auxFile.atEnd() && !mInAppendix) {
        parseAuxLine(QString::fromLocal8Bit(auxFile.readLine()));
    }

    auxFile.close();
    return true;
}

void PresModel::parseAuxLine(const QString& line)
{
    QRegExp writeNavRegexp("\\\\@writefile\\{nav\\}\\{(.*)\\}");
    if (writeNavRegexp.exactMatch(line.trimmed()))
        parseNavLine(writeNavRegexp.capturedTexts().at(1));
}

