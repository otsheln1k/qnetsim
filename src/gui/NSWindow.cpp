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

void NSWindow::on_addSwitchButton_clicked()
{
    ui->graphicsView->setMode(NSGraphicsViewMode::ADD_NODE);
    ui->graphicsView->setNode(NSGraphicsViewNode::SWITCH);
}

void NSWindow::on_addRouterButton_clicked()
{
    ui->graphicsView->setMode(NSGraphicsViewMode::ADD_NODE);
    ui->graphicsView->setNode(NSGraphicsViewNode::ROUTER);
}

void NSWindow::on_toolButton_3_clicked()
{
    ui->graphicsView->setMode(NSGraphicsViewMode::ADD_CONNECTION);
}

void NSWindow::onLoggerMessage(const SimulationLoggerMessage &msg)
{
    QObject *obj = msg.object();

    GenericNetworkInterface *iface = nullptr;
    NetworkNode *node = nullptr;

    for (; obj; obj = obj->parent()) {
        if (auto *i = dynamic_cast<GenericNetworkInterface *>(obj)) {
            iface = i;
        } else if (auto *n = dynamic_cast<NetworkNode *>(obj)) {
            node = n;
        } else if (auto *ib = dynamic_cast<InterfaceBound *>(obj)) {
            iface = ib->boundInterface();
        }
    }

    if (node == nullptr) {
        return;
    }

    int row = ui->logTable->rowCount();

    ui->logTable->setRowCount(row + 1);

    auto *i0 = new QTableWidgetItem(
        QString{"???????? @ 0x%1"}
        .arg((size_t)node, sizeof(size_t)*2, 16, QChar{'0'}));
    i0->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    ui->logTable->setItem(row, 0, i0);

    QString ifacename;
    if (iface != nullptr) {
        auto *gnode = ui->graphicsView->lookupGraphicsNode(node);
        ifacename = gnode->interfaceName(iface);
    } else {
        ifacename = "-";
    }
    auto *i1 = new QTableWidgetItem{ifacename};
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

void NSWindow::on_saveButton_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, "Save Dialog", "/home", "*.bin");
    Serialization* serialization = new Serialization(ui->graphicsView, path);
    serialization->createSave();
}

void NSWindow::on_loadButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Load Dialog", "/home", "*.bin");
    Serialization* serialization = new Serialization(ui->graphicsView, path);
    serialization->loadSave();
}

void NSWindow::on_journalView_toggled(bool arg1)
{
    ui->logDock->setHidden(!arg1);
}

void NSWindow::on_logDock_visibilityChanged(bool visible)
{
    ui->journalView->setChecked(visible);
}
