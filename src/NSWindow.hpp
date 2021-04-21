#ifndef _NS_WINDOW_HPP_
#define _NS_WINDOW_HPP_

#include <QMainWindow>

#include "SimulationLogger.hpp"
#include "NetworkNode.h"
#include "EthernetInterface.hpp"

#include "ui_NSWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class NSWindow;
}
QT_END_NAMESPACE

class NSWindow : public QMainWindow {
    Q_OBJECT;

signals:
    void quit();

public:
    NSWindow(QWidget *parent=nullptr);
    ~NSWindow();

private:
    Ui::NSWindow *ui;

private slots:
    void on_actionQuit_triggered();
    void on_addPCButton_clicked();
    void on_addHubButton_clicked();
    void on_addSwitchButton_clicked();
    void on_toolButton_3_clicked();
    void on_resetButton_clicked();

    void onLoggerMessage(const SimulationLogger::Message &msg);
    void onLogClear();

    void on_logClearButton_clicked();
};

#endif
