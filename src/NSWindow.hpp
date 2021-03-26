#ifndef _NS_WINDOW_HPP_
#define _NS_WINDOW_HPP_

#include <QMainWindow>

#include "ui_NSWindow.h"

class NSWindow : public QMainWindow,
                 private Ui::NSWindow {
    Q_OBJECT

protected:
    void onClickMeClicked();
    void onQuitTriggered();

signals:
    void quit();

public:
    NSWindow();
};

#endif
