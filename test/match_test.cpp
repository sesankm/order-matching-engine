#include "lob.hpp"
#include "order.hpp"
#include "matching_engine.hpp"
#include <cassert>

// TODO: Add gtest later
int main()
{
    MatchingEngine me {};

    me.submitOrder(4.5, 10, OType::LIMIT, OSide::BUY);
    me.submitOrder(5.6, 1, OType::LIMIT, OSide::BUY);
    me.submitOrder(7.0, 2, OType::LIMIT, OSide::BUY);
    me.submitOrder(8.0, 2, OType::LIMIT, OSide::BUY);

    assert(me.lob.bids[Order::get_price(7.0)].size() == 1);
    assert(me.lob.bids[Order::get_price(8.0)].size() == 1);
    assert(me.lob.asks.empty() == true);

    me.submitOrder(7.0, 20, OType::LIMIT, OSide::SELL);
    assert(me.lob.bids[Order::get_price(7.0)].size() == 0);
    assert(me.lob.bids[Order::get_price(8.0)].size() == 0);
    assert(me.lob.asks.empty() == false);

    me.submitOrder(3.0, 1, OType::LIMIT, OSide::SELL);
    assert(me.lob.asks.contains(Order::get_price(3.0)) == false);

    assert(me.lob.getBestBid() == Order::get_price(4.5));
    me.submitOrder(3.0, 1000, OType::LIMIT, OSide::SELL);
    assert(me.lob.asks.contains(Order::get_price(3.0)) == true);

    assert(me.lob.getBestAsk() == Order::get_price(3));
    assert(me.lob.getBestBid() == EMPTY_BID);


    me.submitOrder(100000.0, 2000, OType::LIMIT, OSide::BUY);
    assert(me.lob.getBestBid() == Order::get_price(100000.0));
    assert(me.lob.getBestAsk() == EMPTY_ASK);
}
