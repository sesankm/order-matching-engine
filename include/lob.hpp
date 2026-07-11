#ifndef LOB_H
#define LOB_H

#include <map>
#include <list>
#include "order.hpp"

#define EMPTY_ASK 0
#define EMPTY_BID std::numeric_limits<std::uint64_t>::max()

auto inline comparator = [](const auto& f1, const auto& f2) { return f1 > f2; };

/* makes it more clear what we're storing in
   bid/ask and lookup maps */
using bucket  = std::uint64_t; 
using order_id = std::uint64_t;

struct Entry {
    OSide side;
    bucket b;
    std::list<Order>::iterator it;
};

class Lob {
public:
    Lob();
    std::uint64_t addOrder(float price, long quantity, OType type, OSide side);

    void cancelOrder(order_id id);
    void updateOrder(order_id id);
    Order getOrder(order_id id) const;

    void cleanFilledOrders();

    std::uint64_t getBestBid() const;
    std::uint64_t getBestAsk() const;

    // Giving up vecor's cache locality for O(1) mid container deletions
    std::map<bucket, std::list<Order>, decltype(comparator)> bids {comparator};
    std::map<bucket, std::list<Order>> asks;

private:

    // O(1) lookup, tells us where the order is
    std::unordered_map<order_id, Entry> lookup; 
};

#endif
