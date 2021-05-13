#ifndef GRAGPICSNODEBUTTON_H
#define GRAGPICSNODEBUTTON_H

#include <QToolButton>

class GraphicsNodeButton : public QToolButton
{
public:
    GraphicsNodeButton(QWidget*);

protected:
    void enterEvent(QEvent*) override;
    void leaveEvent(QEvent*) override;
};

#endif // GRAGPICSNODEBUTTON_H
