#include "Deck.h"

#include <algorithm>
#include <chrono>
#include <stdexcept>

Deck::Deck(int decks)
    : deckCount(decks),
      rng(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count())) {
    initialize(decks);
    shuffleDeck();
}

void Deck::initialize(int decks) {
    deckCount = decks;
    cards.clear();

    std::vector<std::string> suits = {"S", "H", "D", "C"};
    std::vector<std::string> ranks = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

    for (int d = 0; d < deckCount; d++) {
        for (const std::string& suit : suits) {
            for (const std::string& rank : ranks) {
                cards.emplace_back(suit, rank);
            }
        }
    }
}

void Deck::shuffleDeck() {
    std::shuffle(cards.begin(), cards.end(), rng);
}

Card Deck::drawCard() {
    if (cards.empty()) {
        throw std::runtime_error("牌堆已經沒有牌了");
    }

    Card card = cards.back();
    cards.pop_back();
    return card;
}

int Deck::remainingCards() const {
    return static_cast<int>(cards.size());
}

double Deck::remainingDecks() const {
    double decksLeft = static_cast<double>(cards.size()) / 52.0;
    return decksLeft < 0.25 ? 0.25 : decksLeft;
}

std::vector<Card> Deck::getRemainingCards() const {
    return cards;
}
