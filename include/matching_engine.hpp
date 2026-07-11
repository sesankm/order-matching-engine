#ifndef MATCHING_H
#define MATCHING_H
#include "lob.hpp"

/*

  MatchingEngine::submitOrder() is the entry point for new orders.

  When submitOrder() is called:
    - first check if the new order crosses.
    - cross means that the order's buy price is >= than the min ask price
      or order's sell price is <= than max buy price

  If there's a cross:
    - walk the opposite side's orders
    - fill as many shares as possible, if there's still shares left after
      consuming all orders in the matching buckets, add it to the list of
      resting orders

 */

class MatchingEngine {
    Lob lob {};

public:
    MatchingEngine();
    void submitOrder(float price, long quantity, OType type, OSide side); 

private:
    void fill();
    void walkOppositeSide();
    bool doesCross();
};

#endif
