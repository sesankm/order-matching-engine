#include "lob.hpp"
#include <stdexcept>
#include <utility>
#include <iostream>

Lob::Lob() {}

std::uint64_t Lob::addOrder(float price, long quantity, OType type, OSide side) {
    Order o {price, quantity, type, side};
    long bucket = o.m_price;
    long order_id = o.m_order_id;

    if (side == OSide::BUY)
        bids[price].push_back(std::move(o));
    else
        asks[price].push_back(std::move(o));

    lookup.emplace(bucket, side);
    return order_id;
}

void Lob::cancelOrder(long id) {
    if (lookup.at(id) == OSide::BUY)
        bids.erase(id);
    else
        asks.erase(id);
}

Order Lob::getOrder(long id) const {
    // Need to update lookup to also store an iterator to make this an O(1) lookup
    throw std::runtime_error("Not implemented yet");
}

void Lob::updateOrder(long id) {
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
