#include <QDebug>
#include <QMenu>

#include "pc.h"

PC::PC(QObject *parent, QPointF position, QSize size, QString *name)
    : Node(parent, new QPixmap("models/018-monitor screen.png"),
           position, size, name) {}

void PC::populateMenu(QMenu *menu)
{
    auto *a1 = menu->addAction("Добавить порт Ethernet");
    QObject::connect(a1, &QAction::triggered,
                     []()
                     {
                         qDebug() << "add ethernet nic!";
                     });
}
