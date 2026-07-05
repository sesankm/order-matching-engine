#ifndef ORDER_H
#define ORDER_H

class Lob {
public:
    Lob();
    void addOrder();
    void cancelOrder();
    void updateOrder();

    void getOrder() const;
    void getBestBid() const;
    void getBestAsk() const;
};

#endif
