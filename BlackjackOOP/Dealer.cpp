#include "Dealer.h"

void Dealer::clearHand() {
    hand.clear();
}

Hand& Dealer::getHand() {
    return hand;
}

const Hand& Dealer::getHand() const {
    return hand;
}
