#include <QObject>
#include <QMessageBox>
#include <QPushButton>
#include <QAction>

#include "NSWindow.hpp"

NSWindow::NSWindow()
{
    setupUi(this);

    QObject::connect(clickmeButton, &QPushButton::clicked,
                     this, &NSWindow::onClickMeClicked);
    QObject::connect(actionQuit, &QAction::triggered,
                     this, &NSWindow::onQuitTriggered);
}

void
NSWindow::onClickMeClicked()
{
    QMessageBox::information(this, "Hello", "Hello world!");
}

void
NSWindow::onQuitTriggered()
{
    emit quit();
}
