#ifndef STRATEGY_H
#define STRATEGY_H

#include "Hand.h"
#include "Card.h"

#include <string>

class Strategy {
public:
    double estimateWinRateFromTrueCount(double trueCount) const;
    int suggestBet(int balance, double trueCount, int minBet = 10) const;

    std::string suggestAction(
        const Hand& playerHand,
        const Card& dealerUpCard,
        bool canDouble,
        double standWinRate,
        double hitWinRate
    ) const;
};

#endif
