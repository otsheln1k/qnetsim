#ifndef _NS_WINDOW_HPP_
#define _NS_WINDOW_HPP_

#include <QMainWindow>
#include <QFileDialog>

#include "SimulationLogger.hpp"
#include "NetworkNode.h"
#include "EthernetInterface.hpp"

#include "ui_NSWindow.h"
#include "Serialization.h"

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

    void onLoggerMessage(const SimulationLoggerMessage &msg);
    void onLogClear();

    void on_logClearButton_clicked();

    void onSteppingToggled(bool value);
    void on_saveButton_clicked();
    void on_loadButton_clicked();
    void on_journalView_toggled(bool arg1);
    void on_logDock_visibilityChanged(bool visible);
};

#endif
