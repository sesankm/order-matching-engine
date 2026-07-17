#include "matching_engine.hpp"
#include <iostream>

MatchingEngine::MatchingEngine() = default;

void MatchingEngine::submitOrder(float price,
                                 long quantity,
                                 OType type,
                                 OSide side) {
    std::uint64_t norm_price = Order::get_price(price);

    if (doesCross(side, norm_price)) {
        walkOppositeSide(side, norm_price, quantity);
    }

    if (quantity > 0) {
        lob.addOrder(price, quantity, type, side);
    }
}


void MatchingEngine::submitOrder(Order&& o) {
    if (doesCross(o.m_side, o.m_price)) {
        walkOppositeSide(o.m_side, o.m_price, o.m_quantity);
    }

    if (o.m_quantity > 0) {
        lob.addOrder(o);
    }
}

bool MatchingEngine::doesCross(const OSide side, const std::uint64_t price) const {
    if (side == OSide::BUY) {
        std::uint64_t best = lob.getBestAsk();
        return best > 0 && price >= best;
    }
    std::uint64_t best = lob.getBestBid();
    return best > 0 && price <= lob.getBestBid();
}

void MatchingEngine::walkOppositeSide(OSide side, std::uint64_t price, long &quantity) {
    auto cmp = [side, price](std::uint64_t bucket) {
        return side == OSide::SELL ? bucket < price : bucket > price;
    };

    if (side == OSide::SELL) {
        walkOppositeSide(quantity, lob.bids, cmp);
    } else {
        walkOppositeSide(quantity, lob.asks, cmp);
    }
    lob.cleanFilledOrders();
}

void MatchingEngine::walkOppositeSide(long &quantity, auto& buckets, auto& cmp) {
    for (auto &[bucket, b_orders]: buckets) {
        if (quantity <= 0 || cmp(bucket)) { break; }
        while (quantity > 0 && b_orders.size() > 0) {
            if (b_orders.front().m_quantity > quantity) {
                b_orders.front().m_quantity -= quantity;
                quantity = 0;
                break;
            }
            quantity -= b_orders.front().m_quantity;
            b_orders.front().m_quantity = 0;
            lob.cancelOrder(b_orders.front().m_order_id);
        }
    }
}