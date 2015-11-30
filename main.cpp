#include "projmanager.h"

#include <QApplication>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    ProjManager manager(NULL);
    Q_UNUSED(manager);

    return app.exec();
}
