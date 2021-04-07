#include <QObject>
#include <QMessageBox>
#include <QPushButton>
#include <QAction>

#include "NSWindow.hpp"

NSWindow::NSWindow()
{
    setupUi(this);

    logTable->setColumnWidth(0, 300);
    logTable->setColumnWidth(1, 300);
    logTable->horizontalHeader()->setStretchLastSection(true);

    __n.addInterface(&__i);
    QObject::connect(&__l, &SimulationLogger::message,
                     this, &NSWindow::onLoggerMessage);
}

void NSWindow::on_actionQuit_triggered()
{
    emit quit();
}

void NSWindow::onLoggerMessage(const SimulationLogger::Message &msg)
{
    int row = logTable->rowCount();

    logTable->setRowCount(row + 1);

    auto *i0 = new QTableWidgetItem(
        QString{"Узел @ 0x%1"}
        .arg((size_t)msg.node(), sizeof(size_t)*2, 16, QChar{'0'}));
    i0->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    logTable->setItem(row, 0, i0);

    auto *i1 = new QTableWidgetItem{
        QString{"Интерфейс @ 0x%1"}
        .arg((size_t)msg.interface(), sizeof(size_t)*2, 16, QChar{'0'})};
    i1->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    logTable->setItem(row, 1, i1);

    auto *i2 = new QTableWidgetItem {msg.text()};
    i2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    logTable->setItem(row, 2, i2);
}

void NSWindow::onLogClear()
{
    logTable->setRowCount(0);
}

void NSWindow::on_logTestButton_clicked()
{
    __l.setCurrentNode(&__n);
    __l.setCurrentInterface(&__i);

    __l.log(QString{"фывап"});

    __l.unsetCurrentInterface();
    __l.unsetCurrentNode();
}

void NSWindow::on_logClearButton_clicked()
{
    onLogClear();
}
