#ifndef CARD_COUNTER_H
#define CARD_COUNTER_H

#include "Card.h"

class CardCounter {
private:
    int runningCount;

public:
    CardCounter();

    void reset();
    void updateCount(const Card& card);
    int getRunningCount() const;
    double getTrueCount(int remainingCards) const;
};

#endif
