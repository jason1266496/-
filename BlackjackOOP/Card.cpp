#include "Card.h"

#include <utility>

Card::Card() : suit("?"), rank("?") {}

Card::Card(std::string s, std::string r)
    : suit(std::move(s)), rank(std::move(r)) {}

std::string Card::getSuit() const {
    return suit;
}

std::string Card::getRank() const {
    return rank;
}

int Card::getValue() const {
    if (rank == "A") return 11;
    if (rank == "J" || rank == "Q" || rank == "K") return 10;
    return std::stoi(rank);
}

int Card::getCountValue() const {
    int value = getValue();

    if (rank == "A" || value == 10) return -1;
    if (value >= 2 && value <= 6) return 1;
    return 0;
}

std::string Card::toString() const {
    return rank + suit;
}
