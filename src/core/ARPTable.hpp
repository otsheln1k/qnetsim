#ifndef _NS_ARP_TABLE_HPP_
#define _NS_ARP_TABLE_HPP_

#include <map>
#include <iterator>

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

    size_t currentSize() const { return _map.size(); }

    class iterator {
        using inner_t = typename std::map<PrAddr, Entry>::const_iterator;
        inner_t x;

    public:
        using value_type = std::tuple<PrAddr, HwAddr, int>;
        using reference = const value_type &;
        using pointer = const value_type *;
        using difference_type = ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

        iterator &operator++() { ++x; return *this; }
        iterator operator++(int) { return {x++}; }
        reference operator*() const
        {
            auto p = *x;
            return std::make_tuple(p->first, p->second.hw, p->second.remain);
        }
    };

    iterator begin() const { return iterator {_map.begin()}; }
    iterator end() const { return iterator {_map.end()}; }

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
