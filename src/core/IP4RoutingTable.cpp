#include "IP4RoutingTable.hpp"

void IP4RoutingTable::addEntry(IP4RoutingTable::Entry entry)
{
    _entries.push_back(entry);
}

void IP4RoutingTable::addEntry(IP4Address net, uint8_t cidr,
                               IP4Address dest, int metric)
{
    addEntry({net, cidr, dest, metric});
}

void IP4RoutingTable::addEntry(IP4Address net, uint8_t cidr, IP4Address dest)
{
    addEntry(net, cidr, dest, _defaultMetric);
}

void IP4RoutingTable::addDefaultRoute(IP4Address dest, int metric)
{
    addEntry({0,0,0,0}, 0, dest, metric);
}

void IP4RoutingTable::addDefaultRoute(IP4Address dest)
{
    addDefaultRoute(dest, _defaultMetric);
}

void IP4RoutingTable::removeEntry(iterator iter)
{
    if (iter != end()) {
        _entries.erase(iter);
    }
}

void IP4RoutingTable::removeEntry(IP4RoutingTable::Entry entry)
{
    auto it = std::find(begin(), end(), entry);
    removeEntry(it);
}

void IP4RoutingTable::removeEntry(IP4Address net, uint8_t cidr, IP4Address dest)
{
    auto it = std::find_if(begin(), end(),
                           [net, cidr, dest](const Entry &e)
                           {
                               return e.net == net
                                   && e.cidr == cidr
                                   && e.dest == dest;
                           });
    removeEntry(it);
}

IP4RoutingTable::iterator IP4RoutingTable::pickRoute(IP4Address addr) const
{
    iterator it, start = begin(), low = end();
    while ((it = std::find_if(
                start, end(),
                [addr](const Entry &e)
                {
                    return addr.inNetwork(e.net, e.cidr);
                })) != end()) {
        if (low == end()
            || it->cidr >= low->cidr
            || it->metric < low->metric) {
            low = it;
        }
        start = ++it;
    }
    return low;
}
