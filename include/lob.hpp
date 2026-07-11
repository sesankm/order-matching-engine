#ifndef LOB_H
#define LOB_H

#include <map>
#include <vector>
#include "order.hpp"

auto inline comparator = [](const auto& f1, const auto& f2) { return f1 > f2; };

/* makes it more clear what we're storing in
   bid/ask and lookup maps */
using bucket  = std::uint64_t; 
using order_id = std::uint64_t;

struct Entry {
    OSide side;
    bucket b;
    std::vector<Order>::iterator it;
};

class Lob {
public:
    Lob();
    std::uint64_t addOrder(float price, long quantity, OType type, OSide side);

    void cancelOrder(order_id id);
    void updateOrder(order_id id);
    Order getOrder(order_id id) const;

    std::uint64_t getBestBid() const;
    std::uint64_t getBestAsk() const;

private:
    std::map<bucket, std::vector<Order>, decltype(comparator)> bids {comparator};
    std::map<bucket, std::vector<Order>> asks;

    // O(1) lookup, tells us where the order is
    std::unordered_map<order_id, Entry> lookup; 
};

#endif
