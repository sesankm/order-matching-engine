#include "lob.hpp"
#include <stdexcept>
#include <utility>
#include <iostream>

Lob::Lob() {}

std::uint64_t Lob::addOrder(float price, long quantity, OType type, OSide side) {
    Order o {price, quantity, type, side};

    bucket   bucket = o.m_price;
    order_id order_id = o.m_order_id;

    /*
      TODO:
      - this iterator logic completely breaks when an order is canceled/erased
      - Use a list instead of vector or use some kind of lazy deletion
    */
    std::vector<Order>::iterator it; 
    if (side == OSide::BUY)
        it = bids[price].insert(bids[price].end(), std::move(o));
    else
        it = asks[price].insert(asks[price].end(), std::move(o));

    lookup.emplace(order_id, Entry {side, bucket, bids[price].end()});
    return order_id;
}

void Lob::cancelOrder(order_id id) {
    throw std::runtime_error("Not implemented yet");
}

Order Lob::getOrder(order_id id) const {
    // Need to update lookup to also store an iterator to make this an O(1) lookup
    throw std::runtime_error("Not implemented yet");
}

void Lob::updateOrder(order_id id) {
    throw std::runtime_error("Not implemented yet");
}


std::uint64_t Lob::getBestBid() const {
    std::vector<Order> f = (*bids.begin()).second;
    return f.at(0).m_order_id;
}

std::uint64_t Lob::getBestAsk() const {
    std::vector<Order> f = (*asks.begin()).second;
    return f.at(0).m_order_id;
}
