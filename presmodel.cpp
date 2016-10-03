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
    : PreviewModel(parent), mDoc(NULL), mCurrentPage(0), mHVirtualScreens(1), mVVirtualScreens(1)
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
    beginResetModel();

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

    mDoc->setRenderHint((Poppler::Document::RenderHint) ((int) Poppler::Document::TextAntialiasing | Poppler::Document::Antialiasing));

    qDebug() << mDoc->metadata();
    qDebug() << mDoc->infoKeys();
    foreach (QString key, mDoc->infoKeys())
        qDebug() << key << ":" << mDoc->info(key);

    if (!parseNavFile(file))
        parseAuxFile(file);
    qDebug() << mTotalFrames << mFramePages;

    mCurrentPage = 0;
    emit documentChanged();
    endResetModel();
    return true;
}

void PresModel::setVirtualScreens(int horizontal, int vertical)
{
    Q_ASSERT(horizontal > 0);
    Q_ASSERT(vertical > 0);

    beginResetModel();
    mHVirtualScreens = horizontal;
    mVVirtualScreens = vertical;
    endResetModel();
    emit virtualScreenNumberChanged();
}

bool PresModel::setCurrentPageNumber(int page)
{
    if ((mDoc == NULL) || (page < 0) || (page >= mDoc->numPages()))
        return false;

    int oldFrame = getCurrentFrameNumber();
    mCurrentPage = page;
    emit currentPageChanged();
    if (getCurrentFrameNumber() != oldFrame)
        emit currentFrameChanged();
    return true;
}

int PresModel::getCurrentFrameNumber(void) const
{
    if (mFramePages.isEmpty())
        return getCurrentPageNumber() + 1;
    if (mCurrentPage >= mFramePages.size())
        return getCurrentPageNumber() + 1;
    return mFramePages.at(mCurrentPage);
}

QVariant PresModel::data(const QModelIndex& index, int role) const
{
    if (mDoc == NULL)
        return QVariant();

    Poppler::Page* page = mDoc->page(index.row());
    if (page == NULL)
        return QVariant();

    if ((role != PreviewRole) && (role != Qt::DisplayRole) && (role != Qt::SizeHintRole))
        return QVariant();

    double aspectRatio = 1.;
    double pageWidth = page->pageSizeF().width();
    double pageHeight = page->pageSizeF().height();
    if (mConstraintOrientation == Qt::Horizontal)
        aspectRatio = mConstraintDim * mHVirtualScreens / pageWidth;
    if (mConstraintOrientation == Qt::Vertical)
        aspectRatio = mConstraintDim * mVVirtualScreens / pageHeight;
    pageWidth = pageWidth * aspectRatio / mHVirtualScreens;
    pageHeight = pageHeight * aspectRatio / mVVirtualScreens;

    if (role == Qt::SizeHintRole)
        return QVariant(QSizeF(pageWidth, pageHeight));
    // TODO remove this: return QImage(pageWidth, pageHeight, QImage::Format_RGB32);
    QImage pageImg = page->renderToImage(72.*aspectRatio, 72.*aspectRatio,
                                         0, 0, pageWidth, pageHeight);
    delete page;
    return QVariant::fromValue<QImage>(pageImg);

}

QImage PresModel::getPage(int number, const QRect& boundingRect, int horizontalVirtualScreen, int verticalVirtualScreen) const
{
    if ((horizontalVirtualScreen <= 0) || (horizontalVirtualScreen > mHVirtualScreens))
        return QImage();
    if ((verticalVirtualScreen <= 0) || (verticalVirtualScreen > mVVirtualScreens))
         return QImage();
    if (mDoc == NULL)
        return QImage();

    Poppler::Page* page = mDoc->page(number);
    if (page == NULL)
        return QImage();

    double aspectRatio = qMin(boundingRect.width() * mHVirtualScreens / page->pageSizeF().width(), boundingRect.height() * mVVirtualScreens / page->pageSizeF().height());
    double pageWidth = page->pageSizeF().width()*aspectRatio;
    double pageHeight = page->pageSizeF().height()*aspectRatio;
    // TODO remove this: return QImage(boundingRect.size(), QImage::Format_RGB32);
    QImage pageImg = page->renderToImage(72.*aspectRatio, 72.*aspectRatio,
                                         (horizontalVirtualScreen - 1) * pageWidth / mHVirtualScreens,
                                         (verticalVirtualScreen - 1) * pageHeight / mVVirtualScreens,
                                         pageWidth / mHVirtualScreens, pageHeight/mVVirtualScreens);
    delete page;
    return pageImg;
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

