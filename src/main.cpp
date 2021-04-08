#include <QApplication>
#include <QObject>
#include <QDebug>

#include "NSWindow.hpp"

int
main(int argc, char **argv)
{
    QApplication app {argc, argv};

    NSWindow win;

    QObject::connect(&win, &NSWindow::quit,
                     &app, &QApplication::quit);

    win.show();

    return app.exec();
}
