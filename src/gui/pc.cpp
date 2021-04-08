#include "pc.h"

PC::PC(QObject *parent, QPointF position, QSize size, QString *name) : Node(parent, position, size, name)
{
    this->image = new QPixmap("models/018-monitor screen.png");
    *this->image = this->image->scaled(size);
}
