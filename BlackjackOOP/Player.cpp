#include "Player.h"

#include <cmath>

Player::Player(int money) : balance(money), currentBet(0) {}

void Player::newRound() {
    hand.clear();
    currentBet = 0;
}

Hand& Player::getHand() {
    return hand;
}

const Hand& Player::getHand() const {
    return hand;
}

int Player::getBalance() const {
    return balance;
}

int Player::getCurrentBet() const {
    return currentBet;
}

bool Player::placeBet(int amount) {
    if (amount <= 0 || amount > balance) {
        return false;
    }

    balance -= amount;
    currentBet = amount;
    return true;
}

bool Player::canDouble() const {
    return balance >= currentBet && currentBet > 0;
}

bool Player::doubleBet() {
    if (!canDouble()) {
        return false;
    }

    balance -= currentBet;
    currentBet *= 2;
    return true;
}

void Player::winBet() {
    balance += currentBet * 2;
}

void Player::winBlackjack() {
    balance += static_cast<int>(std::round(currentBet * 2.5));
}

void Player::pushBet() {
    balance += currentBet;
}

void Player::loseBet() {
    // 下注時已經先扣錢，所以輸了不用再扣。
}
