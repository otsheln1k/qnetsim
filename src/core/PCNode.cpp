#include "SimulationLogger.hpp"
#include "IP4OnEthernetDriver.hpp"
#include "PCNode.h"

PCNode::PCNode()
{
    ipNode.setParent(this);
}

MACAddr PCNode::createMac(){
    const uint32_t orgID = 787458;
    uint32_t ifaceID = 16777216; //00:00:00;
    //33554430 - FF:FF:FE
    uint32_t newAdd = rand()%33554430;
    ifaceID += newAdd;
    return *(new MACAddr(orgID, ifaceID));
}

void PCNode::addInterface(GenericNetworkInterface *iface){
    EthernetInterface *eiface = dynamic_cast<EthernetInterface *>(iface);
    if(eiface == nullptr){
        return;
    }else{
        NetworkNode::addInterface(iface);
        MACAddr MAC = createMac();
        EthernetDriver* driver = new EthernetDriver(MAC, eiface);

        driver->setParent(this);
        interfaces[eiface] = driver;
        driver->setBoundInterface(eiface);

        IP4Driver *drv = new IP4OnEthernetDriver {driver};
        ipNode.addDriver(drv);
        drv->setBoundInterface(eiface);
    }
}

void PCNode::removeInterface(GenericNetworkInterface *iface){
    EthernetInterface *eiface = dynamic_cast<EthernetInterface *>(iface);
    if (eiface == nullptr) {
        return;
    }

    auto driver = interfaces.find(eiface);
    if(driver != interfaces.end()){
        delete driver->second;
        interfaces.erase(driver);
    }

    IP4Driver *drv = ipNode.driverByInterface(iface);
    delete drv;

    NetworkNode::removeInterface(iface);
}

EthernetDriver* PCNode::getDriver(EthernetInterface *iface){
    auto driver = interfaces.find(iface);
    if(driver != interfaces.end()){
        return driver->second;
    }

    return nullptr;
}

void PCNode::sendEthernetFrame(EthernetInterface *eiface,
                               MACAddr dest,
                               EtherType etherType,
                               const QVector<uint8_t> &payload)
{
    auto *drv = getDriver(eiface);
    drv->sendFrame(dest, etherType, payload.begin(), payload.end());
}

void PCNode::sendECTPLoopback(GenericNetworkInterface *iface,
                              uint16_t seq,
                              MACAddr through,
                              const QVector<uint8_t> &payload)
{
    auto *eiface = dynamic_cast<EthernetInterface *>(iface);
    auto *drv = getDriver(eiface);

    QVector<uint8_t> bytes;
    ECTPDriver::makeLoopback(drv->address(), seq,
                             payload.begin(), payload.end(),
                             std::back_inserter(bytes));

    SimulationLogger::currentLogger()->log(
        this,
        QString{"Prepared loopback ECTP message: dest=%1, seq=%2"}
        .arg(drv->address())
        .arg(seq));

    sendEthernetFrame(eiface, through, ETHERTYPE_ECTP, bytes);
}

void PCNode::setInterfaceSettings(GenericNetworkInterface *iface,
                                  MACAddr hw,
                                  IP4Address ip,
                                  uint8_t cidr)
{
    auto *eiface = dynamic_cast<EthernetInterface *>(iface);
    if (eiface == nullptr) {
        return;
    }

    getDriver(eiface)->setAddress(hw);
    auto *ipdrv = getIP4Driver(eiface);
    ipdrv->setAddress(ip);
    ipdrv->setCidr(cidr);
}

IP4Driver *PCNode::getIP4Driver(EthernetInterface *iface)
{
    return ipNode.driverByInterface(iface);
}

IP4Node *PCNode::getIP4Node()
{
    return &ipNode;
}
