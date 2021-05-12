#ifndef _NS_ARP_TABLE_HPP_
#define _NS_ARP_TABLE_HPP_

#include <map>

#include "Tickable.hpp"
#include "EthernetFrame.hpp"    // EtherType
#include "ARPPacket.hpp"

template<typename HwAddr, typename PrAddr>
class ARPTable : public Tickable {

    struct Entry {
        HwAddr hw;
        int remain;             // -1 for permanent
    };

    std::map<PrAddr, Entry> _map;
    int _lifetime = -1;
    int _maxsize = -1;

    void trimTable()
    {
        if (_maxsize < 0) {
            return;
        }

        while (_map.size() > (size_t)_maxsize) {
            auto worstiter = _map.begin();
            auto worstlt = worstiter->second.remain;
            for (auto iter = std::next(worstiter); iter != _map.end(); ++iter) {
                int lt = iter->second.remain;
                if (lt >= 0
                    && (worstlt < 0
                        || lt < worstlt)) {
                    worstiter = iter;
                    worstlt = lt;
                }
            }
            _map.erase(worstiter);
        }
    }

public:
    void addEntry(PrAddr pr, HwAddr hw, int lifetime)
    {
        // Don’t overwrite permanent entries
        auto existing = _map.find(pr);
        if (existing != _map.end()
            && existing->second.remain < 0
            && lifetime >= 0) {
            return;
        }

        _map[pr] = {hw, lifetime};
        trimTable();
    }

    void addEntry(PrAddr pr, HwAddr hw)
    {
        addEntry(pr, hw, _lifetime);
    }

    void addPermanentEntry(PrAddr pr, HwAddr hw)
    {
        addEntry(pr, hw, -1);
    }

    void removeEntry(PrAddr pr, HwAddr hw)
    {
        auto etriter = _map.find(pr);
        if (etriter == _map.end()
            || etriter->second.hw != hw) {
            return;
        }
        _map.erase(etriter);
    }

    std::optional<HwAddr> query(PrAddr pr) const
    {
        auto etriter = _map.find(pr);
        if (etriter != _map.end()) {
            return {etriter->second.hw};
        }
        return {};
    }

    void clearTable()
    {
        _map.clear();
    }

    int defaultLifetime() const { return _lifetime; }
    void setDefaultLifetime(int x) { _lifetime = x; }

    int maxSize() const { return _maxsize; }
    void setMaxSize(int x) { _maxsize = x; }

    virtual bool tick() override
    {
        for (auto iter = _map.begin(); iter != _map.end();) {
            Entry &entry = iter->second;
            if (entry.remain == 0) {
                _map.erase(iter++);
            } else {
                if (entry.remain > 0) {
                    --entry.remain;
                }
                ++iter;
            }
        }

        return false;
    }
};

#endif
