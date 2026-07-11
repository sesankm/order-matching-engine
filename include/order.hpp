#ifndef ORDER_H
#define ORDER_H

#include <cstdint>

static const int PRICE_PRCSN = 4;

enum class OType {
    LIMIT,
    MARKET
};

enum class OSide {
    BUY,
    SELL
};

class Order {
public:
    Order(float, long, OType, OSide);
    static std::uint64_t get_price(float price);

    long m_quantity;
    std::uint64_t m_order_id;
    std::uint64_t m_price;

private:
    OType m_type;
    OSide m_side;

    std::uint64_t m_timestamp;

    std::uint64_t generate_id();
};

#endif
