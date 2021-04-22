#include "Serialization.h"

Serialization::Serialization(NSGraphicsView* view, QString path) : view{view}, path{path}, scene{view->scene},
    model{view->model}, nodetab{view->nodetab}, edgetab{view->edgetab}
{}

void Serialization::createSave()
{
    QFile file(path);
    QDataStream stream(&file);
    file.open(QIODevice::WriteOnly);

    int count = nodetab.size();
    stream << count;
    for(auto item : nodetab)
    {
        QPoint pos;
        QSize size;
        QString name, type;

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

        stream << type << pos << size << name;
    }

    file.close();
}

void Serialization::loadSave()
{
    QFile file(path);
    QDataStream stream(&file);
    file.open( QIODevice::ReadOnly );
    view->resetModel();
    int count;
    QPoint pos;
    QSize size;
    QString name, type;

    stream >> count;
    for(int i = 0; i < count; i++){
        stream >> type >> pos >> size >> name;

        if(name == "None")
            name = nullptr;

        qDebug() << type;
        qDebug() << pos;
        qDebug() << size;
        qDebug() << name;

        QPointF scn = view->mapToScene(pos);

        NetworkNode *nd = nullptr;
        NSGraphicsNode *gnode = nullptr;

        if(type == "PC"){
            auto *pnode = new PCNode {};
            nd = pnode;
            gnode = new NSGraphicsPCNode(view, pnode, scn, size, &name);
        }
        else if(type == "HUB"){
            auto *pnode = new HubNode {};
            nd = pnode;
            gnode = new NSGraphicsHubNode(view, pnode, scn, size, &name);
        }

        nd->moveToThread(&view->simulationThread);
        model->addNode(nd);

        scene->addItem(gnode);
        scene->update(0,0,view->width(),view->height());
        auto r = view->sceneRect();
        auto pos2 = view->mapFromScene(scn);
        r.setX(r.x() - (pos.x() - pos2.x()));
        r.setY(r.y() - (pos.y() - pos2.y()));
        view->setSceneRect(r);

        nodetab[nd] = gnode;
    }

    file.close();
}
