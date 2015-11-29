#ifndef PRESSTYLE_H
#define PRESSTYLE_H

#include <QProxyStyle>

class PresStyle : public QProxyStyle
{
public:
    inline PresStyle(QStyle* base = NULL) :
        QProxyStyle(base) {}
#if QT_VERSION >= 0x050000
    inline PresStyle(const QString& key) :
        QProxyStyle(key) {}
#endif
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = NULL) const;
};

#endif // PRESSTYLE_H
