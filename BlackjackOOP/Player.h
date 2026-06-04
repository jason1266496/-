#ifndef PLAYER_H
#define PLAYER_H

#include "Hand.h"

class Player {
private:
    Hand hand;
    int balance;
    int currentBet;

public:
    Player(int money = 1000);

    void newRound();
    Hand& getHand();
    const Hand& getHand() const;
    int getBalance() const;
    int getCurrentBet() const;

    bool placeBet(int amount);
    bool canDouble() const;
    bool doubleBet();

    void winBet();
    void winBlackjack();
    void pushBet();
    void loseBet();
};

#endif
