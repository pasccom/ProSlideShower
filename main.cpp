#include "projmanager.h"

#include <QApplication>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    app.setOrganizationName("Pascom");
    app.setApplicationName("ProSlideShower");

    ProjManager manager(NULL);
    Q_UNUSED(manager);

    return app.exec();
}
