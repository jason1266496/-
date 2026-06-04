#include "WinRateSimulator.h"

#include <chrono>

Card WinRateSimulator::drawRandomCard(std::vector<Card>& cards, std::mt19937& rng) {
    std::uniform_int_distribution<int> dist(0, static_cast<int>(cards.size()) - 1);
    int index = dist(rng);

    Card card = cards[index];
    cards.erase(cards.begin() + index);

    return card;
}

int WinRateSimulator::compareHands(const Hand& playerHand, const Hand& dealerHand) {
    int playerTotal = playerHand.getValue();
    int dealerTotal = dealerHand.getValue();

    if (playerTotal > 21) return -1;
    if (dealerTotal > 21) return 1;
    if (playerTotal > dealerTotal) return 1;
    if (playerTotal < dealerTotal) return -1;

    return 0;
}

void WinRateSimulator::simulateDealerPlay(Hand& dealerHand, std::vector<Card>& shoe, std::mt19937& rng) {
    while (dealerHand.getValue() < 17 && !shoe.empty()) {
        dealerHand.addCard(drawRandomCard(shoe, rng));
    }
}

double WinRateSimulator::estimateStandWinRate(
    const Hand& playerHand,
    const Card& dealerUpCard,
    const std::vector<Card>& unseenCards,
    int simulations
) {
    if (unseenCards.empty() || playerHand.isBust()) {
        return 0.0;
    }

    std::mt19937 rng(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
    double score = 0.0;

    for (int i = 0; i < simulations; i++) {
        std::vector<Card> shoe = unseenCards;
        Hand dealerHand;

        dealerHand.addCard(dealerUpCard);

        if (!shoe.empty()) {
            dealerHand.addCard(drawRandomCard(shoe, rng));
        }

        simulateDealerPlay(dealerHand, shoe, rng);

        int result = compareHands(playerHand, dealerHand);

        if (result > 0) {
            score += 1.0;
        } else if (result == 0) {
            score += 0.5;
        }
    }

    return score / simulations;
}

double WinRateSimulator::estimateHitWinRate(
    const Hand& playerHand,
    const Card& dealerUpCard,
    const std::vector<Card>& unseenCards,
    int simulations
) {
    if (unseenCards.empty() || playerHand.isBust()) {
        return 0.0;
    }

    std::mt19937 rng(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count() + 12345));
    double score = 0.0;

    for (int i = 0; i < simulations; i++) {
        std::vector<Card> shoe = unseenCards;
        Hand simulatedPlayer = playerHand;

        if (!shoe.empty()) {
            simulatedPlayer.addCard(drawRandomCard(shoe, rng));
        }

        if (simulatedPlayer.isBust()) {
            continue;
        }

        // 簡化策略：選擇 Hit 後，模擬玩家繼續補牌到至少 17 點。
        while (simulatedPlayer.getValue() < 17 && !shoe.empty()) {
            simulatedPlayer.addCard(drawRandomCard(shoe, rng));

            if (simulatedPlayer.isBust()) {
                break;
            }
        }

        if (simulatedPlayer.isBust()) {
            continue;
        }

        Hand dealerHand;
        dealerHand.addCard(dealerUpCard);

        if (!shoe.empty()) {
            dealerHand.addCard(drawRandomCard(shoe, rng));
        }

        simulateDealerPlay(dealerHand, shoe, rng);

        int result = compareHands(simulatedPlayer, dealerHand);

        if (result > 0) {
            score += 1.0;
        } else if (result == 0) {
            score += 0.5;
        }
    }

    return score / simulations;
}
