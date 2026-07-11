#include "order.hpp"
#include "lob.hpp"
#include <cassert>

#include <iostream>

// TODO: Add gtest later
int main()
{
    Lob  l{};
    long b_id =  l.addOrder(20,    100, OType::LIMIT, OSide::BUY);
    long b_id2 = l.addOrder(21.10, 100, OType::LIMIT, OSide::BUY);
    long b_id3 = l.addOrder(22.12, 100, OType::LIMIT, OSide::BUY);

    long s_id =  l.addOrder(22,    100, OType::LIMIT, OSide::SELL);
    long s_id2 = l.addOrder(21.10, 100, OType::LIMIT, OSide::SELL);
    long s_id3 = l.addOrder(23.12, 100, OType::LIMIT, OSide::SELL);

    assert(l.getBestAsk() == Order::get_price(21.10));
    assert(l.getBestBid() == Order::get_price(22.12));

    l.cancelOrder(s_id2);
    assert(l.getBestAsk() != Order::get_price(21.10));

    l.cancelOrder(s_id);
    l.cancelOrder(s_id3);
    assert(l.getBestAsk() != EMPTY_ASK);
}
