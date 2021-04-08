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

    ui->logTable->setColumnWidth(0, 300);
    ui->logTable->setColumnWidth(1, 300);
    ui->logTable->horizontalHeader()->setStretchLastSection(true);

    // use default logger
    QObject::connect(SimulationLogger::currentLogger(),
                     &SimulationLogger::message,
                     this, &NSWindow::onLoggerMessage);
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

void NSWindow::on_resetButton_clicked()
{
    ui->graphicsView->resetModel();
}

void NSWindow::onLoggerMessage(const SimulationLogger::Message &msg)
{
    int row = ui->logTable->rowCount();

    ui->logTable->setRowCount(row + 1);

    auto *i0 = new QTableWidgetItem(
        QString{"Узел @ 0x%1"}
        .arg((size_t)msg.node(), sizeof(size_t)*2, 16, QChar{'0'}));
    i0->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    ui->logTable->setItem(row, 0, i0);

    auto *i1 = new QTableWidgetItem{
        QString{"Интерфейс @ 0x%1"}
        .arg((size_t)msg.interface(), sizeof(size_t)*2, 16, QChar{'0'})};
    i1->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    ui->logTable->setItem(row, 1, i1);

    auto *i2 = new QTableWidgetItem {msg.text()};
    i2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    ui->logTable->setItem(row, 2, i2);
}

void NSWindow::onLogClear()
{
    ui->logTable->setRowCount(0);
}

void NSWindow::on_logClearButton_clicked()
{
    onLogClear();
}
