#include "pc.h"

PC::PC(QObject *parent, QPointF position, QSize size, QString *name)
    : Node(parent, new QPixmap("models/018-monitor screen.png"),
           position, size, name) {}
