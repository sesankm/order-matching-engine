#ifndef LOB_H
#define LOB_H

#include <map>
#include <vector>
#include "order.hpp"

class Lob {
public:
    Lob();
    void addOrder();
    void cancelOrder();
    void updateOrder();

    void getOrder() const;
    void getBestBid() const;
    void getBestAsk() const;

private:
    std::map<long, std::vector<Order>> buy;
};

#endif
