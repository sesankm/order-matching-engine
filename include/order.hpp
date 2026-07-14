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
    OSide m_side;

private:
    OType m_type;

    std::uint64_t m_timestamp;

    std::uint64_t generate_id();
};

#endif
