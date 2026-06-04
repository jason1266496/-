#ifndef DECK_H
#define DECK_H

#include "Card.h"

#include <vector>
#include <random>

class Deck {
private:
    std::vector<Card> cards;
    int deckCount;
    std::mt19937 rng;

public:
    Deck(int decks = 4);

    void initialize(int decks);
    void shuffleDeck();
    Card drawCard();
    int remainingCards() const;
    double remainingDecks() const;
    std::vector<Card> getRemainingCards() const;
};

#endif
