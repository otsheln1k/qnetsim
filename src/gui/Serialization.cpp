#include "Serialization.h"

Serialization::Serialization(NSGraphicsView* view, QString path) : view{view}, path{path}
{}

void Serialization::createSave()
{
    QFile file(path);
    QDataStream stream(&file);
    if(!file.open(QIODevice::WriteOnly))
        return;

    int countNodes = view->nodetab.size();
    stream << countNodes;
    for(auto item : view->nodetab)
    {
        QPoint pos;
        QSize size;
        QString name, type;
        unsigned int countInterfaces;

        if(dynamic_cast<NSGraphicsPCNode*>(item.second))
            type = "PC";
        else if(dynamic_cast<NSGraphicsHubNode*>(item.second))
            type = "HUB";
        pos = item.second->pos().toPoint();
        size = item.second->size;
        if(item.second->name)
            name = *(item.second->name);
        else
            name = "None";

        countInterfaces = item.first->interfacesCount();

        stream << type << pos << size << name << countInterfaces;

        qDebug() << "Start read:";
        qDebug() << type;
        qDebug() << pos;
        qDebug() << size;
        qDebug() << name;
        qDebug() << countInterfaces;
        qDebug() << "End read";

    }

    file.close();
}

void Serialization::loadSave()
{
    QFile file(path);
    QDataStream stream(&file);
    if(!file.open( QIODevice::ReadOnly))
        return;
    view->resetModel();
    int count;
    QPoint pos;
    QSize size;
    QString nameStr, *name, type;
    unsigned int countInterfaces;

    stream >> count;
    for(int i = 0; i < count; i++){
        stream >> type >> pos >> size >> nameStr >> countInterfaces;

        if(nameStr == "None")
            name = nullptr;

        qDebug() << "Start write:";
        qDebug() << type;
        qDebug() << pos;
        qDebug() << size;
        qDebug() << name;
        qDebug() << countInterfaces;
        qDebug() << "End write";

        QPointF scn = view->mapToScene(pos);

        NetworkNode *nd = nullptr;
        NSGraphicsNode *gnode = nullptr;

        if(type == "PC"){
            auto *pnode = new PCNode {};
            nd = pnode;
            for(unsigned int i = 0; i < countInterfaces; i++){
                auto *iface = new EthernetInterface {};
                iface->moveToThread(pnode->thread());
                pnode->addInterface((GenericNetworkInterface *)iface);
            }
            gnode = new NSGraphicsPCNode(view, pnode, scn, size, name);
        }
        else if(type == "HUB"){
            auto *hnode = new HubNode {};
            nd = hnode;
            for(unsigned int i = 0; i < countInterfaces; i++){
                auto *iface = new EthernetInterface {};
                iface->moveToThread(hnode->thread());
                hnode->addInterface((GenericNetworkInterface *)iface);
            }
            gnode = new NSGraphicsHubNode(view, hnode, scn, size, name);
        }

        nd->moveToThread(&view->simulationThread);
        view->model->addNode(nd);

        QObject::connect(gnode, &NSGraphicsNode::removingNode,
                                     view, &NSGraphicsView::onRemovingGraphicsNode);

        view->scene->addItem(gnode);
        view->scene->update(0,0,view->width(),view->height());
        auto r = view->sceneRect();
        auto pos2 = view->mapFromScene(scn);
        r.setX(r.x() - (pos.x() - pos2.x()));
        r.setY(r.y() - (pos.y() - pos2.y()));
        view->setSceneRect(r);

        view->nodetab[nd] = gnode;
    }

    file.close();
}
