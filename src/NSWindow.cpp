#include <QObject>
#include <QMessageBox>
#include <QPushButton>
#include <QAction>

#include "NSWindow.hpp"

NSWindow::NSWindow()
{
    setupUi(this);
}

void NSWindow::on_actionQuit_triggered()
{
    emit quit();
}
