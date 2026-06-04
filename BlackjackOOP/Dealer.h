#ifndef DEALER_H
#define DEALER_H

#include "Hand.h"

class Dealer {
private:
    Hand hand;

public:
    void clearHand();
    Hand& getHand();
    const Hand& getHand() const;
};

#endif
