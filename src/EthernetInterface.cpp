#include <vector>
#include <utility>

#include "SimulationLogger.hpp"
#include "GenericNetworkInterface.hpp"
#include "EthernetInterface.hpp"

bool EthernetInterface::connect(GenericNetworkInterface *iface)
{
    auto *eiface = dynamic_cast<EthernetInterface *>(iface);
    if (!eiface) {
        return false;
    }

    if (_peer) {
        disconnect(_peer);
    }
    if (eiface->_peer) {
        eiface->disconnect(eiface->_peer);
    }

    _peer = eiface;
    eiface->_peer = this;

    emit connected(iface);
    emit eiface->connected(this);

    return true;
}

bool EthernetInterface::disconnect(GenericNetworkInterface *iface)
{
    auto *eiface = dynamic_cast<EthernetInterface *>(iface);
    if (!eiface) {
        return false;
    }

    if (_peer != eiface) {
        return false;
    }

    _peer = nullptr;
    eiface->_peer = nullptr;

    emit disconnected(iface);
    emit eiface->disconnected(this);

    return true;
}

size_t EthernetInterface::connectionsCount() const
{
    return (_peer != nullptr) ? 1 : 0;
}

GenericNetworkInterface *
EthernetInterface::connectionByIndex(size_t index) const
{
    return (index == 0) ? _peer : nullptr;
}

bool EthernetInterface::sendFrame(const EthernetFrame &frame)
{
    SimulationLogger::currentLogger()->setCurrentInterface(this);

    std::vector<uint8_t> bytes;
    bytes.resize(frame.size());
    if (!frame.write(bytes.data())) {
        SimulationLogger::currentLogger()->unsetCurrentInterface();
        return false;
    }

    _sq.emplace(std::move(bytes));

    SimulationLogger::currentLogger()->unsetCurrentInterface();
    return true;
}

bool EthernetInterface::stepSend()
{
    if (_sq.empty()) {
        return false;
    }

    while (!_sq.empty()) {
        if (_peer) {
            _peer->_bq.emplace(std::move(_sq.front()));
        }

        _sq.pop();
    }

    return true;
}

bool EthernetInterface::stepRecv()
{
    bool res = !_rq.empty() || !_bq.empty();

    SimulationLogger::currentLogger()->setCurrentInterface(this);

    while (!_rq.empty()) {
        const auto &bytes = _rq.front();

        EthernetFrame frame;
        if (frame.read(bytes.data(), bytes.size())) {
            emit receivedFrame(&frame);
        }

        _rq.pop();
    }

    SimulationLogger::currentLogger()->unsetCurrentInterface();

    std::swap(_rq, _bq);

    return res;
}

EthernetInterface::~EthernetInterface()
{
    if (_peer != nullptr) {
        disconnect(_peer);
    }
}
