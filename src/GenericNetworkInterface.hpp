#ifndef _NS_GENERIC_NETWORK_INTERFACE_HPP_
#define _NS_GENERIC_NETWORK_INTERFACE_HPP_

#include <QObject>

#include "Steppable.hpp"

class GenericNetworkInterface : public QObject,
                                public Steppable {
    Q_OBJECT

public:
    virtual bool connect(GenericNetworkInterface *iface) =0;
    virtual bool disconnect(GenericNetworkInterface *iface) =0;

signals:
    void connected(GenericNetworkInterface *other);
    void disconnected(GenericNetworkInterface *other);
};

#endif
