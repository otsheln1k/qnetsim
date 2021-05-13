#ifndef SERIALIZATIONINTERFACENODE_H
#define SERIALIZATIONINTERFACENODE_H

#include <QString>
#include <QSize>

class SerializationInterfaceNode
{
public:
    SerializationInterfaceNode(){};
    virtual QString* getName() = 0;
    virtual QSize getSize() = 0;
};

#endif // SERIALIZATIONINTERFACENODE_H
