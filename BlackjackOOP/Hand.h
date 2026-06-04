#ifndef HAND_H
#define HAND_H

#include "Card.h"

#include <vector>

class Hand {
private:
    std::vector<Card> cards;

public:
    void clear();
    void addCard(const Card& card);
    int size() const;
    const std::vector<Card>& getCards() const;

    int getValue() const;
    bool isSoft() const;
    bool isBust() const;
    bool isBlackjack() const;
    void show() const;
};

#endif
