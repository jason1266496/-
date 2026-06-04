#ifndef WIN_RATE_SIMULATOR_H
#define WIN_RATE_SIMULATOR_H

#include "Hand.h"
#include "Card.h"

#include <vector>
#include <random>

class WinRateSimulator {
private:
    static Card drawRandomCard(std::vector<Card>& cards, std::mt19937& rng);
    static int compareHands(const Hand& playerHand, const Hand& dealerHand);
    static void simulateDealerPlay(Hand& dealerHand, std::vector<Card>& shoe, std::mt19937& rng);

public:
    static double estimateStandWinRate(
        const Hand& playerHand,
        const Card& dealerUpCard,
        const std::vector<Card>& unseenCards,
        int simulations = 3000
    );

    static double estimateHitWinRate(
        const Hand& playerHand,
        const Card& dealerUpCard,
        const std::vector<Card>& unseenCards,
        int simulations = 3000
    );
};

#endif
