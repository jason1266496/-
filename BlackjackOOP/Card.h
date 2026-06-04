#ifndef CARD_H
#define CARD_H

#include <string>

class Card {
private:
    std::string suit;
    std::string rank;

public:
    Card();
    Card(std::string s, std::string r);

    std::string getSuit() const;
    std::string getRank() const;
    int getValue() const;
    int getCountValue() const;
    std::string toString() const;
};

#endif
