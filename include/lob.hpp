#ifndef LOB_H
#define LOB_H

#include <map>
#include <vector>
#include "order.hpp"

auto inline comparator = [](const auto& f1, const auto& f2) { return f1 > f2; };

class Lob {
public:
    Lob();
    std::uint64_t addOrder(float price, long quantity, OType type, OSide side);
    void cancelOrder(long id);
    void updateOrder(long id);

    Order getOrder(long id) const;
    std::uint64_t getBestBid() const;
    std::uint64_t getBestAsk() const;

private:
    std::map<long, std::vector<Order>, decltype(comparator)> bids {comparator};
    std::map<long, std::vector<Order>> asks;

    // O(1) lookup, tells us where the order is
    std::unordered_map<long, OSide> lookup; 
};

#endif
