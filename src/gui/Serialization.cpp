#include "Serialization.h"

Serialization::Serialization(NSGraphicsView* view, QString path) : view{view}, path{path}, interfaceView{view}
{}

void Serialization::createSave()
{
    QFile file(path);
    QDataStream stream(&file);
    if(!file.open(QIODevice::WriteOnly))
        return;

    std::map<NetworkNode *, NSGraphicsNode *> nodetab = *(interfaceView->getNodetab());
    int countNodes = nodetab.size();
    qDebug() << "Reading count Nodes:" << countNodes;
    stream << countNodes;
    for(auto item : nodetab)
    {
        QPoint pos;
        QSize size;
        QString name, type;
        unsigned int countInterfaces;

        SerializationInterfaceNode* interfaceNode;
        if(dynamic_cast<NSGraphicsPCNode*>(item.second)){
            interfaceNode = dynamic_cast<NSGraphicsPCNode*>(item.second);
            type = "PC";
        }
        else if(dynamic_cast<NSGraphicsHubNode*>(item.second)){
            interfaceNode = dynamic_cast<NSGraphicsHubNode*>(item.second);
            type = "HUB";
        }
        else if(dynamic_cast<NSGraphicsSwitchNode*>(item.second)){
            interfaceNode = dynamic_cast<NSGraphicsSwitchNode*>(item.second);
            type = "SWITCH";
        }

        pos = item.second->pos().toPoint();
        size = interfaceNode->getSize();
        if(interfaceNode->getName())
            name = *(interfaceNode->getName());
        else
            name = "None";

        countInterfaces = item.first->interfacesCount();

        stream << type << pos << size << name << countInterfaces;


        if(type == "PC"){
            PCNode *pnode = dynamic_cast<PCNode*>(item.first);
            for(auto genIface = item.first->begin(); genIface != item.first->end(); ++genIface){
                EthernetInterface *iface = dynamic_cast<EthernetInterface *>(*genIface);
                EthernetDriver* driver = pnode->getDriver(iface);
                MACAddr mac = driver->address();
                stream << mac;
                qDebug() << "Read Mac: " << mac;
            }
            //std::map<std::pair<QObject *, QObject *>, QGraphicsLineItem *> edgetab = *(interfaceView->getEdgetab());
            //qDebug() << "Count lines: " << edgetab.size();
        }
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
                EthernetDriver* driver = pnode->getDriver(iface);
                QString macStr;
                stream >> macStr;
                qDebug() << "Write Mac: " << macStr;
                MACAddr mac = driver->address();
                qDebug() << mac.parseQString(macStr);
                driver->setAddress(mac);
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
        else if(type == "SWITCH"){
            auto *snode = new SwitchNode {};
            nd = snode;
            for(unsigned int i = 0; i < countInterfaces; i++){
                auto *iface = new EthernetInterface {};
                iface->moveToThread(snode->thread());
                snode->addInterface((GenericNetworkInterface *)iface);
            }
            gnode = new NSGraphicsSwitchNode(view, snode, scn, size, name);
        }

        interfaceView->moveToThread(nd);
        NetworkModel* model = interfaceView->getNetworkModel();
        model->addNode(nd);

        interfaceView->connectNodeWithRemoving(gnode);

        QGraphicsScene* scene = interfaceView->getScene();
        scene->addItem(gnode);
        scene->update(0,0,view->width(),view->height());
        auto r = view->sceneRect();
        auto pos2 = view->mapFromScene(scn);
        r.setX(r.x() - (pos.x() - pos2.x()));
        r.setY(r.y() - (pos.y() - pos2.y()));
        view->setSceneRect(r);

        std::map<NetworkNode *, NSGraphicsNode *>& nodetab = *(interfaceView->getNodetab());
        nodetab[nd] = gnode;
    }

    file.close();
}
