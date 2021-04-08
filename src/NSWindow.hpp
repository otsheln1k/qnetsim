#ifndef _NS_WINDOW_HPP_
#define _NS_WINDOW_HPP_

#include <QMainWindow>

#include "ui_NSWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class NSWindow;
}
QT_END_NAMESPACE

class NSWindow : public QMainWindow,
                 private Ui::NSWindow {
    Q_OBJECT

protected:
    void onClickMeClicked();
    void onQuitTriggered();

signals:
    void quit();

public:
    NSWindow(QWidget *parent=nullptr);
    ~NSWindow();

private:
    Ui::NSWindow *ui;

private slots:
    void on_actionQuit_triggered();
    void on_toolButton_2_clicked();
    void on_toolButton_3_clicked();
    void on_resetButton_clicked();
};

#endif
