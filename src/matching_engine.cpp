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
        std::uint64_t best = lob.getBestAsk();
        return best > 0 && price <= lob.getBestBid();
    }
}

// TODO: Add buy side handling
void MatchingEngine::walkOppositeSide(OSide side, std::uint64_t price, long& quantity) {
    for (auto& [bucket, orders] : lob.bids) {
        if (quantity <= 0 || bucket < price) { break; }
        while (quantity > 0 && orders.size() > 0) {
            if (orders.front().m_quantity > quantity) {
                orders.front().m_quantity -= quantity;
                quantity = 0;
                break;
            } else {
                quantity -= orders.front().m_quantity;
                std::cout << "Filled buy order " << orders.front().m_order_id << "\n";
                orders.pop_front();
                continue;
            }
        }
    }
}
