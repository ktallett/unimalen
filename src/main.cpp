#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("unimalen");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("unimalen");

    MainWindow window;
    window.show();

    return app.exec();
}