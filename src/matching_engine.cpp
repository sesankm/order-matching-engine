#include "matching_engine.hpp"
#include <iostream>

MatchingEngine::MatchingEngine() {};

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

bool MatchingEngine::doesCross(OSide side, std::uint64_t price) {
    if (side == OSide::BUY) {
        std::uint64_t best = lob.getBestAsk();
        return best > 0 && price >= best;
    }
    else {
        std::uint64_t best = lob.getBestBid();
        return best > 0 && price <= lob.getBestBid();
    }
}

// TODO: Add buy side handling
void MatchingEngine::walkOppositeSide(OSide side, std::uint64_t price, long& quantity) {
    for (auto& [bucket, b_orders] : lob.bids) {
        if (quantity <= 0 || bucket < price) { break; }
        while (quantity > 0 && b_orders.size() > 0) {
            if (b_orders.front().m_quantity > quantity) {
                b_orders.front().m_quantity -= quantity;
                quantity = 0;
                break;
            } else {
                quantity -= b_orders.front().m_quantity;
                b_orders.front().m_quantity = 0;
                std::cout << "Filled buy order " << b_orders.front().m_order_id << " in at " << b_orders.front().m_price << "\n";
                lob.cancelOrder(b_orders.front().m_order_id);
                break;
            }
        }
    }
    lob.cleanFilledOrders();
}
