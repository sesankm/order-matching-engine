#include "lob.hpp"
#include "order.hpp"
#include <stdexcept>
#include <utility>

Lob::Lob() {}

/*
  TODO: This method works for synchronous processing since orders arrive sequentially.
  Needs to be updated to serialize order submissions when mutliple threads get involved.
*/
std::uint64_t Lob::addOrder(float price, long quantity, OType type, OSide side) {
    Order o {price, quantity, type, side};

    bucket   bucket = o.m_price;
    order_id order_id = o.m_order_id;

    std::list<Order>::iterator it; 
    if (side == OSide::BUY)
        it = bids[bucket].insert(bids[bucket].end(), std::move(o));
    else
        it = asks[bucket].insert(asks[bucket].end(), std::move(o));

    lookup.emplace(order_id, Entry {side, bucket, it});
    return order_id;
}

void Lob::cancelOrder(order_id id) {
    Entry e = lookup.at(id);
    if (e.side == OSide::BUY)
        bids.at(e.b).erase(e.it);
    else
        asks.at(e.b).erase(e.it);
    lookup.erase(id);
}

Order Lob::getOrder(order_id id) const {
    Entry e = lookup.at(id);
    return *e.it;
}

void Lob::updateOrder(order_id id) {
    throw std::runtime_error("Not implemented yet");
}


std::uint64_t Lob::getBestBid() const {
    std::list<Order> f = (*bids.begin()).second;
    return f.front().m_order_id;
}

std::uint64_t Lob::getBestAsk() const {
    std::list<Order> f = (*asks.begin()).second;
    return f.front().m_order_id;
}
