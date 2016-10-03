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

#include "presstyle.h"

#include <QtGui>
#if QT_VERSION >= 0x050000
#   include <QtWidgets>
#endif

void PresStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    if (control == CC_ScrollBar) {
        const QStyleOptionSlider* sliderOpt = qstyleoption_cast<const QStyleOptionSlider*>(option);
        if (sliderOpt == NULL)
            return;

        QStyleOptionSlider opt = *sliderOpt;
        State stateFlags = opt.state;

        painter->setPen(QPen(opt.palette.brush(QPalette::WindowText), 2., Qt::SolidLine, Qt::RoundCap));
        painter->drawRoundedRect(1, opt.rect.height() / 2 - 6, opt.rect.width() - 2, 12, 6., 6.);

        if (sliderOpt->subControls & SC_ScrollBarSubLine) {
            opt.rect = sliderOpt->rect;
            opt.state = stateFlags;
            opt.rect = proxy()->subControlRect(control, &opt, SC_ScrollBarSubLine, widget);
            if (opt.rect.isValid()) {
                if (!(sliderOpt->activeSubControls & SC_ScrollBarSubLine))
                    opt.state &= ~(State_Sunken | State_MouseOver);
                proxy()->drawControl(CE_ScrollBarSubLine, &opt, painter, widget);
            }
        }
        if (sliderOpt->subControls & SC_ScrollBarAddLine) {
            opt.rect = sliderOpt->rect;
            opt.state = stateFlags;
            opt.rect = proxy()->subControlRect(control, &opt, SC_ScrollBarAddLine, widget);
            if (opt.rect.isValid()) {
                if (!(sliderOpt->activeSubControls & SC_ScrollBarAddLine))
                    opt.state &= ~(State_Sunken | State_MouseOver);
                proxy()->drawControl(CE_ScrollBarAddLine, &opt, painter, widget);
            }
        }
        if (sliderOpt->subControls & SC_ScrollBarSubPage) {
            opt.rect = sliderOpt->rect;
            opt.state = stateFlags;
            opt.rect = proxy()->subControlRect(control, &opt, SC_ScrollBarSubPage, widget);
            if (opt.rect.isValid()) {
                if (!(sliderOpt->activeSubControls & SC_ScrollBarSubPage))
                    opt.state &= ~(State_Sunken | State_MouseOver);
                proxy()->drawControl(CE_ScrollBarSubPage, &opt, painter, widget);
            }
        }
        if (sliderOpt->subControls & SC_ScrollBarAddPage) {
            opt.rect = sliderOpt->rect;
            opt.state = stateFlags;
            opt.rect = proxy()->subControlRect(control, &opt, SC_ScrollBarAddPage, widget);
            if (opt.rect.isValid()) {
                if (!(sliderOpt->activeSubControls & SC_ScrollBarAddPage))
                    opt.state &= ~(State_Sunken | State_MouseOver);
                proxy()->drawControl(CE_ScrollBarAddPage, &opt, painter, widget);
            }
        }
        if (sliderOpt->subControls & SC_ScrollBarFirst) {
            opt.rect = sliderOpt->rect;
            opt.state = stateFlags;
            opt.rect = proxy()->subControlRect(control, &opt, SC_ScrollBarFirst, widget);
            if (opt.rect.isValid()) {
                if (!(sliderOpt->activeSubControls & SC_ScrollBarFirst))
                    opt.state &= ~(State_Sunken | State_MouseOver);
                proxy()->drawControl(CE_ScrollBarFirst, &opt, painter, widget);
            }
        }
        if (sliderOpt->subControls & SC_ScrollBarLast) {
            opt.rect = sliderOpt->rect;
            opt.state = stateFlags;
            opt.rect = proxy()->subControlRect(control, &opt, SC_ScrollBarLast, widget);
            if (opt.rect.isValid()) {
                if (!(sliderOpt->activeSubControls & SC_ScrollBarLast))
                    opt.state &= ~(State_Sunken | State_MouseOver);
                proxy()->drawControl(CE_ScrollBarLast, &opt, painter, widget);
            }
        }
        if (sliderOpt->subControls & SC_ScrollBarSlider) {
            opt.rect = sliderOpt->rect;
            opt.state = stateFlags;
            opt.rect = proxy()->subControlRect(control, &opt, SC_ScrollBarSlider, widget);
            if (opt.rect.isValid()) {
                if (!(sliderOpt->activeSubControls & SC_ScrollBarSlider))
                    opt.state &= ~(State_Sunken | State_MouseOver);
                proxy()->drawControl(CE_ScrollBarSlider, &opt, painter, widget);
            }
        }
        return;
    }

    QProxyStyle::drawComplexControl(control, option, painter, widget);
}


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

        if ((opt->progress < opt->minimum) || (opt->progress > opt->maximum) || (opt->minimum == opt->maximum))
            return;

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

    if (element == CE_ScrollBarAddLine) {
        const QStyleOptionSlider* opt = qstyleoption_cast<const QStyleOptionSlider*>(option);

        painter->setPen(QPen(opt->palette.brush(QPalette::WindowText), 10., Qt::SolidLine, Qt::RoundCap));
        //painter->setPen(QPen(Qt::red, 10., Qt::SolidLine, Qt::RoundCap));
        painter->drawLine(opt->rect.x() - 2, opt->rect.height() / 2, opt->rect.right() - 6, opt->rect.height() / 2);

        if (opt->state & State_Sunken)
            painter->setPen(QPen(Qt::red, 3., Qt::SolidLine, Qt::RoundCap));
        else if (opt->state & State_MouseOver)
             painter->setPen(QPen(Qt::darkRed, 3., Qt::SolidLine, Qt::RoundCap));
        else
            painter->setPen(QPen(opt->palette.color(QPalette::Window), 3., Qt::SolidLine, Qt::RoundCap));
        painter->drawLine(opt->rect.right() - 4, opt->rect.height() / 2, opt->rect.right() - 10, opt->rect.height() / 2 - 4);
        painter->drawLine(opt->rect.right() - 4, opt->rect.height() / 2, opt->rect.right() - 10, opt->rect.height() / 2 + 4);
        return;
    }
    if (element == CE_ScrollBarSubLine) {
        const QStyleOptionSlider* opt = qstyleoption_cast<const QStyleOptionSlider*>(option);

        painter->setPen(QPen(opt->palette.brush(QPalette::WindowText), 10., Qt::SolidLine, Qt::RoundCap));
        //painter->setPen(QPen(Qt::blue, 10., Qt::SolidLine, Qt::RoundCap));
        painter->drawLine(opt->rect.x() + 7, opt->rect.height() / 2, opt->rect.right() + 2, opt->rect.height() / 2);

        if (opt->state & State_Sunken)
            painter->setPen(QPen(Qt::red, 3., Qt::SolidLine, Qt::RoundCap));
        else if (opt->state & State_MouseOver)
            painter->setPen(QPen(Qt::darkRed, 3., Qt::SolidLine, Qt::RoundCap));
        else
            painter->setPen(QPen(opt->palette.color(QPalette::Window), 3., Qt::SolidLine, Qt::RoundCap));
        painter->drawLine(opt->rect.left() + 5, opt->rect.height() / 2, opt->rect.left() + 11, opt->rect.height() / 2 - 4);
        painter->drawLine(opt->rect.left() + 5, opt->rect.height() / 2, opt->rect.left() + 11, opt->rect.height() / 2 + 4);
        return;
    }
    if (element == CE_ScrollBarAddPage) {
        const QStyleOptionSlider* opt = qstyleoption_cast<const QStyleOptionSlider*>(option);

        if (opt->state & State_Sunken)
            painter->setPen(QPen(QColor(Qt::darkGray).darker(), 10., Qt::SolidLine, Qt::RoundCap));
        else
            painter->setPen(QPen(opt->palette.color(QPalette::Window), 10., Qt::SolidLine, Qt::RoundCap));
        //painter->setPen(QPen(Qt::darkRed, 10., Qt::SolidLine, Qt::RoundCap));
        painter->drawLine(opt->rect.left() - 5, opt->rect.height() / 2, opt->rect.right() - 4, opt->rect.height() / 2);
    }
    if (element == CE_ScrollBarSubPage) {
        const QStyleOptionSlider* opt = qstyleoption_cast<const QStyleOptionSlider*>(option);

        if (opt->state & State_Sunken)
            painter->setPen(QPen(QColor(Qt::darkGray).darker(), 10., Qt::SolidLine, Qt::RoundCap));
        else
            painter->setPen(QPen(opt->palette.color(QPalette::Window), 10., Qt::SolidLine, Qt::RoundCap));
        //painter->setPen(QPen(Qt::darkBlue, 10., Qt::SolidLine, Qt::RoundCap));
        painter->drawLine(opt->rect.left() + 4, opt->rect.height() / 2, opt->rect.right() + 5, opt->rect.height() / 2);
    }
    if (element == CE_ScrollBarFirst) {
        const QStyleOptionSlider* opt = qstyleoption_cast<const QStyleOptionSlider*>(option);
        qWarning() << "Draw CE_ScrollBarFirst with PresStyle" << opt->rect;
        return;
    }
    if (element == CE_ScrollBarLast) {
        const QStyleOptionSlider* opt = qstyleoption_cast<const QStyleOptionSlider*>(option);
        qWarning() << "Draw CE_ScrollBarLast with PresStyle" << opt->rect;
        return;
    }
    if (element == CE_ScrollBarSlider) {
        const QStyleOptionSlider* opt = qstyleoption_cast<const QStyleOptionSlider*>(option);

        if (opt->state & (State_MouseOver | State_Sunken))
            painter->setPen(QPen(opt->palette.color(QPalette::WindowText), 10., Qt::SolidLine, Qt::RoundCap));
        else
            painter->setPen(QPen(opt->palette.color(QPalette::WindowText).darker(), 10., Qt::SolidLine, Qt::RoundCap));
        //painter->setPen(QPen(Qt::green, 10., Qt::SolidLine, Qt::RoundCap));
        painter->drawLine(opt->rect.left() + 5, opt->rect.height() / 2, opt->rect.right() - 5, opt->rect.height() / 2);
        return;
    }

    QProxyStyle::drawControl(element, option, painter, widget);
}
