#include "Serialization.h"

Serialization::Serialization(NSGraphicsView* view, QString path) : view{view}, path{path}, interfaceView{view}
{}

void Serialization::createSave()
{
    QFile file(path);
    QDataStream stream(&file);
    if(!file.open(QIODevice::WriteOnly))
        return;
    std::map<GenericNetworkInterface*, int> iface_indexes{};
    int conn_count = 0;

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
        }

        for(GenericNetworkInterface* iface : *item.first){
            iface_indexes[iface] = iface_indexes.size();
            conn_count += iface->connectionsCount();
        }

        qDebug() << "Start read:";
        qDebug() << type;
        qDebug() << pos;
        qDebug() << size;
        qDebug() << name;
        qDebug() << countInterfaces;
        qDebug() << "End read";
    }

    stream << conn_count;
    for(auto iface_pair : iface_indexes){
        auto *iface = iface_pair.first;
        int iface_i = iface_pair.second;

        int c = iface->connectionsCount();
        for(int i = 0; i < c; ++i){
            GenericNetworkInterface* other = iface->connectionByIndex(i);
            int other_i = iface_indexes[other];
            if(iface_i < other_i)
                stream << iface_i << other_i;
        }
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
    std::vector<GenericNetworkInterface*> ifaces{};

    stream >> count;
    for(int i = 0; i < count; i++){
        stream >> type >> pos >> size >> nameStr >> countInterfaces;

        if(nameStr == "None")
            name = nullptr;

        qDebug() << "Start write:";
        qDebug() << type;
        qDebug() << pos;
        qDebug() << size;
        qDebug() << nameStr;
        qDebug() << countInterfaces;
        qDebug() << "End write";

        // adding nodes

        QPointF scn = view->mapToScene(pos);

        NetworkNode *nd = nullptr;
        NSGraphicsNode *gnode = nullptr;

        PCNode* pnode;
        HubNode* hnode;
        SwitchNode* snode;

        if(type == "PC"){
            pnode = new PCNode{};
            nd = pnode;
            gnode = new NSGraphicsPCNode(view, pnode, scn, size, name);
        }
        else if(type == "HUB"){
            hnode = new HubNode {};
            nd = hnode;
            gnode = new NSGraphicsHubNode(view, hnode, scn, size, name);
        }
        else if(type == "SWITCH"){
            snode = new SwitchNode {};
            nd = snode;
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

        // adding interfaces

        if(type == "PC"){
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

                ifaces.push_back(iface);
            }
        }
        else if(type == "HUB"){
            for(unsigned int i = 0; i < countInterfaces; i++){
                auto *iface = new EthernetInterface {};
                iface->moveToThread(hnode->thread());
                hnode->addInterface((GenericNetworkInterface *)iface);
                ifaces.push_back(iface);
            }

        }
        else if(type == "SWITCH"){
            for(unsigned int i = 0; i < countInterfaces; i++){
                auto *iface = new EthernetInterface {};
                iface->moveToThread(snode->thread());
                snode->addInterface((GenericNetworkInterface *)iface);
                ifaces.push_back(iface);
            }
        }
    }

    // adding edges

    int conn_count;
    stream >> conn_count;
    for(int i = 0; i < (conn_count / 2); ++i){
        int iface_i, other_i;
        stream >> iface_i >> other_i;
        ifaces[iface_i]->connect(ifaces[other_i]);
    }

    file.close();
}
