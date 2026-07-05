#ifndef ORDER_H
#define ORDER_H

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
    Order();

private:
    long  m_order_id;
    long  m_quantity;
    float m_price;
    OType m_type;
    OSide m_side;

    long long timestamp;
};

#endif
