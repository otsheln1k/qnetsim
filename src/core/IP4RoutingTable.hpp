#ifndef _NS_IP4_ROUTING_TABLE_HPP_
#define _NS_IP4_ROUTING_TABLE_HPP_

#include <stdint.h>

#include <vector>

#include "IP4Address.hpp"

class IP4RoutingTable {
public:
    struct Entry {
        IP4Address net;
        uint8_t cidr;
        IP4Address dest;
        int metric;

        bool operator==(const Entry &e) const
        {
            return net == e.net
                && cidr == e.cidr
                && dest == e.dest
                && metric == e.metric;
        }

        bool operator!=(const Entry &e) const
        {
            return !(*this == e);
        }
    };

private:
    std::vector<Entry> _entries {};
    int _defaultMetric = 0;

public:
    using iterator = std::vector<Entry>::const_iterator;

    void addEntry(Entry e);

    void addEntry(IP4Address net, uint8_t cidr,
                  IP4Address dest, int metric);

    void addEntry(IP4Address net, uint8_t cidr, IP4Address dest);

    void addDefaultRoute(IP4Address dest, int metric);

    void addDefaultRoute(IP4Address dest);

    void removeEntry(iterator iter);

    void removeEntry(Entry entry);

    // Note: removes *any* matching route
    void removeEntry(IP4Address net, uint8_t cidr, IP4Address dest);

    iterator begin() const { return _entries.begin(); }
    iterator end() const { return _entries.end(); }

    size_t entriesCount() const { return _entries.size(); }

    iterator pickRoute(IP4Address addr) const;

    int defaultMetric() const { return _defaultMetric; }
    void setDefaultMetric(int x) { _defaultMetric = x; }
};

#endif
