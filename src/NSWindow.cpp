#include <QObject>
#include <QMessageBox>
#include <QPushButton>
#include <QAction>

#include "NSWindow.hpp"

NSWindow::NSWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NSWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->resetButton, &QPushButton::clicked,
                     ui->graphicsView, &NSGraphicsView::resetModel);

    QObject::connect(ui->stopSimAction, &QAction::triggered,
                     ui->graphicsView, &NSGraphicsView::stopSimulation);

    QObject::connect(ui->stepSimAction, &QAction::triggered,
                     ui->graphicsView, &NSGraphicsView::stepSimulation);

    QObject::connect(ui->toggleStepSimAction, &QAction::toggled,
                     this, &NSWindow::onSteppingToggled);

    ui->logTable->setColumnWidth(0, 300);
    ui->logTable->setColumnWidth(1, 300);
    ui->logTable->horizontalHeader()->setStretchLastSection(true);

    // use default logger
    QObject::connect(SimulationLogger::currentLogger(),
                     &SimulationLogger::message,
                     this, &NSWindow::onLoggerMessage);

    onSteppingToggled(ui->toggleStepSimAction->isChecked());
}

NSWindow::~NSWindow()
{
    delete ui;
}

void NSWindow::on_actionQuit_triggered()
{
    emit quit();
}

void NSWindow::on_addPCButton_clicked()
{
    ui->graphicsView->setMode(NSGraphicsViewMode::ADD_NODE);
    ui->graphicsView->setNode(NSGraphicsViewNode::PC);
}

void NSWindow::on_addHubButton_clicked()
{
    ui->graphicsView->setMode(NSGraphicsViewMode::ADD_NODE);
    ui->graphicsView->setNode(NSGraphicsViewNode::HUB);
}

void NSWindow::on_toolButton_3_clicked()
{
    ui->graphicsView->setMode(NSGraphicsViewMode::ADD_CONNECTION);
}

void NSWindow::onLoggerMessage(const SimulationLoggerMessage &msg)
{
    int row = ui->logTable->rowCount();

    ui->logTable->setRowCount(row + 1);

    auto *iface = msg.interface();
    auto *node = dynamic_cast<NetworkNode *>(iface->parent());

    auto *i0 = new QTableWidgetItem(
        QString{"Узел @ 0x%1"}
        .arg((size_t)node, sizeof(size_t)*2, 16, QChar{'0'}));
    i0->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    ui->logTable->setItem(row, 0, i0);

    auto *i1 = new QTableWidgetItem{
        QString{"Интерфейс @ 0x%1"}
        .arg((size_t)iface, sizeof(size_t)*2, 16, QChar{'0'})};
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

void NSWindow::onSteppingToggled(bool value)
{
    if (value) {
        ui->graphicsView->pauseSimulation();
    } else {
        ui->graphicsView->resumeSimulation();
    }

    ui->stepSimAction->setEnabled(value);
}

void NSWindow::on_journalView_toggled(bool arg1)
{
    ui->logDock->setHidden(!arg1);
}

void NSWindow::on_logDock_visibilityChanged(bool visible)
{
    ui->journalView->setChecked(visible);
}
