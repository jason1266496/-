#ifndef GAME_H
#define GAME_H

#include "Deck.h"
#include "Player.h"
#include "Dealer.h"
#include "CardCounter.h"
#include "Strategy.h"

#include <vector>
#include <string>

class Game {
private:
    int totalDeckCount;
    Deck deck;
    Player player;
    Dealer dealer;
    CardCounter counter;
    Strategy strategy;
    bool dealerHiddenCounted;

    void printLine() const;
    int readInt(const std::string& prompt, int minValue, int maxValue) const;
    char readAction(bool canDouble) const;

    Card drawVisibleCard();
    void revealDealerHiddenCard();
    std::vector<Card> buildUnseenCardsForSimulation() const;
    void reshuffleIfNeeded();
    void showTable(bool revealDealer) const;

    void dealInitialCards();
    bool checkInitialBlackjack();
    void playerTurn();
    void dealerTurn();
    void settleRound();
    bool playOneRound();

public:
    Game(int decks, int money);
    void start();
};

#endif
