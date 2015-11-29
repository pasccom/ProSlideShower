#include "presstyle.h"

#include <QtGui>
#if QT_VERSION >= 0x050000
#   include <QtWidgets>
#endif

void PresStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    if (element == CE_ProgressBar) {
        drawControl(CE_ProgressBarGroove, option, painter, widget);
        drawControl(CE_ProgressBarContents, option, painter, widget);
        return;
    }
    if (element == CE_ProgressBarLabel)
        return;
    if (element == CE_ProgressBarGroove) {
        const QStyleOptionProgressBar* opt = qstyleoption_cast<const QStyleOptionProgressBar *>(option);

        painter->setPen(QPen(opt->palette.brush(QPalette::WindowText), 2., Qt::SolidLine, Qt::RoundCap));
        painter->drawRoundedRect(1, opt->rect.height() / 2 - 7, opt->rect.width() - 2, 14, 7., 7.);
        return;
    }
    if (element == CE_ProgressBarContents) {
        const QStyleOptionProgressBar* opt = qstyleoption_cast<const QStyleOptionProgressBar *>(option);

        QImage buffer(opt->rect.width() - 4, 12, QImage::Format_ARGB32_Premultiplied);
        buffer.fill(QColor(0, 0, 0, 0));

        QPainter p(&buffer);
        p.setPen(QPen(opt->palette.brush(QPalette::NoRole), 12., Qt::SolidLine, Qt::RoundCap));
        p.drawLine(6, 6, buffer.width() - 6, 6);
        p.setCompositionMode(QPainter::CompositionMode_SourceIn);
        p.setPen(Qt::NoPen);
        p.setBrush(opt->palette.color(QPalette::Window));
        p.fillRect(qMax(0, opt->progress - opt->minimum)*buffer.width() / qMax(1, opt->maximum - opt->minimum), 0,
                   qMax(0, opt->maximum - opt->progress)*buffer.width() / qMax(1, opt->maximum - opt->minimum) + 1, buffer.height(),
                   Qt::SolidPattern);

        painter->drawImage(2, opt->rect.height() / 2 - 6, buffer);
        return;
    }


    QProxyStyle::drawControl(element, option, painter, widget);
}
