#include "Hand.h"

#include <iostream>

void Hand::clear() {
    cards.clear();
}

void Hand::addCard(const Card& card) {
    cards.push_back(card);
}

int Hand::size() const {
    return static_cast<int>(cards.size());
}

const std::vector<Card>& Hand::getCards() const {
    return cards;
}

int Hand::getValue() const {
    int total = 0;
    int aceCount = 0;

    for (const Card& card : cards) {
        total += card.getValue();

        if (card.getRank() == "A") {
            aceCount++;
        }
    }

    while (total > 21 && aceCount > 0) {
        total -= 10;
        aceCount--;
    }

    return total;
}

bool Hand::isSoft() const {
    int total = 0;
    int aceCount = 0;

    for (const Card& card : cards) {
        total += card.getValue();

        if (card.getRank() == "A") {
            aceCount++;
        }
    }

    while (total > 21 && aceCount > 0) {
        total -= 10;
        aceCount--;
    }

    return aceCount > 0 && total <= 21;
}

bool Hand::isBust() const {
    return getValue() > 21;
}

bool Hand::isBlackjack() const {
    return cards.size() == 2 && getValue() == 21;
}

void Hand::show() const {
    for (const Card& card : cards) {
        std::cout << card.toString() << " ";
    }

    std::cout << "(點數: " << getValue();

    if (isSoft()) {
        std::cout << ", Soft";
    }

    std::cout << ")";
}
