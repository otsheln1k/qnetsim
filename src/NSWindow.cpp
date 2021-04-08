#include <QObject>
#include <QMessageBox>
#include <QPushButton>
#include <QAction>

#include "NSWindow.hpp"

NSWindow::NSWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NSWindow)
{
    this->ui->setupUi(this);
}

NSWindow::~NSWindow()
{
    delete this->ui;
}

void NSWindow::on_actionQuit_triggered()
{
    emit quit();
}

void NSWindow::on_toolButton_2_clicked()
{
    ui->graphicsView->setMode(NSGraphicsViewMode::ADD_NODE);
    ui->graphicsView->setNode(NSGraphicsViewNode::PC);
}

void NSWindow::on_toolButton_3_clicked()
{
    ui->graphicsView->setMode(NSGraphicsViewMode::ADD_CONNECTION);
}
