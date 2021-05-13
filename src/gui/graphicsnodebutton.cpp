#include "graphicsnodebutton.h"

GraphicsNodeButton::GraphicsNodeButton(QWidget *parent) : QToolButton(parent)
{

}

void GraphicsNodeButton::enterEvent(QEvent* ev)
{
    this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    QToolButton::enterEvent(ev);
}

void GraphicsNodeButton::leaveEvent(QEvent* ev)
{
    this->setToolButtonStyle(Qt::ToolButtonIconOnly);
    QToolButton::enterEvent(ev);
}
