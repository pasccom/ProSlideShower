#include "projmanager.h"
#include "presmodel.h"

#include <QTranslator>
#include <QApplication>
#include <QCommandLineParser>

#include <QtDebug>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    app.setOrganizationName("Pascom");
    app.setApplicationName("ProSlideShower");
    app.setApplicationVersion("0.0.1");

    // Command line arguments parsing
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main", "A small software to show your slides like a professional"));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("filepath", QCoreApplication::translate("main", "Presentation file path."));

    QCommandLineOption minutesOption(QStringList() << "m" << "minues", QCoreApplication::translate("main", "Presentation duration in minutes (adds up with hours)."), "0");
    parser.addOption(minutesOption);
    QCommandLineOption hoursOption(QStringList() << "o" << "hours", QCoreApplication::translate("main", "Presentation duration in hours (adds up with minutes)."), "0");
    parser.addOption(hoursOption);
    QCommandLineOption screensOption(QStringList() << "s" << "screens", QCoreApplication::translate("main", "Number of horizontal virtual screens in presentation file."), "1");
    parser.addOption(screensOption);

    parser.process(qApp->arguments());

    // Total time from command line
    bool ok;
    QTime totalTime(0, 0, 0);

    ok = false;
    int minutes = parser.value(minutesOption).toInt(&ok, 10);
    if (ok)
        totalTime = totalTime.addSecs(minutes * 60);

    ok = false;
    int hours = parser.value(hoursOption).toInt(&ok, 10);
    if (ok)
        totalTime = totalTime.addSecs(hours * 3600);

    qDebug() << hours << minutes << totalTime;

    // Screens from command line
    ok = false;
    int screens = parser.value(screensOption).toInt(&ok, 10);
    if (!ok)
        screens = 0;

    // Translator
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
    if ((totalTime.hour() != 0) || (totalTime.minute() != 0))
        manager.setTotalTime(totalTime);
    if (parser.positionalArguments().size() >= 1) {
        PresModel* model = new PresModel(parser.positionalArguments().at(0));
        if (screens != 0)
            model->setVirtualScreens(screens);
        manager.setModel(model);
    }
    Q_UNUSED(manager);

    return app.exec();
}
