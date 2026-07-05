#include "order.hpp"
#include <chrono>
#include <stdexcept>

std::uint64_t get_price(float price) {
    if (price < 0) { throw std::runtime_error("Price can't be negative"); }
    return static_cast<std::uint64_t>(std::llround(price * std::pow(10, PRICE_PRCSN)));
}

Order::Order(float price, long quantity, OType type, OSide side)
    : m_price { get_price(price) }, m_quantity {quantity}, m_type {type}, m_side {side},
      m_timestamp {static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count())},
      m_order_id {generate_id()}
{}

std::uint64_t Order::generate_id() {
    static std::uint64_t id { 0 };
    return ++id;
}
