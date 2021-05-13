#ifndef _NS_INTERFACE_BOUND_HPP_
#define _NS_INTERFACE_BOUND_HPP_

#include "GenericNetworkInterface.hpp"

class InterfaceBound {
    GenericNetworkInterface *_iface = nullptr;

public:
    GenericNetworkInterface *boundInterface() const { return _iface; }
    void setBoundInterface(GenericNetworkInterface *x) { _iface = x; }
};

#endif
