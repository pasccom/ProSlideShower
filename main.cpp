#include "projmanager.h"

#include <QApplication>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    ProjManager manager(NULL);
    manager.load("/home/pascal/Documents/soutenance.pdf");

    return app.exec();
}
