#include "projmanager.h"

#include <QTranslator>
#include <QApplication>

#include <QtDebug>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    app.setOrganizationName("Pascom");
    app.setApplicationName("ProSlideShower");

    QTranslator translator(NULL);
    foreach (QString lang, QLocale::system().uiLanguages()) {
        QString qmFile = QString(QLatin1String("proslideshower_%1.qm")).arg(lang);
        if (translator.load(qmFile, ".."))
            break;
        if (lang.length() > 2) {
            qmFile = QString(QLatin1String("proslideshower_%1.qm")).arg(lang.left(2));
            if (translator.load(qmFile, ".."))
                break;
        }
    }

    if (!translator.isEmpty())
        app.installTranslator(&translator);
    else
        qWarning() << "Could not find a translator.";

    ProjManager manager(NULL);
    Q_UNUSED(manager);

    return app.exec();
}
