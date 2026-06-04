#include "Strategy.h"

#include <algorithm>

using namespace std;

double Strategy::estimateWinRateFromTrueCount(double trueCount) const {
    // 教學用簡化模型：True Count 越高，玩家優勢越高。
    double winRate = 0.49 + trueCount * 0.005;

    if (winRate < 0.42) {
        winRate = 0.42;
    }

    if (winRate > 0.62) {
        winRate = 0.62;
    }

    return winRate;
}

int Strategy::suggestBet(int balance, double trueCount, int minBet) const {
    if (balance <= 0) {
        return 0;
    }

    double winRate = estimateWinRateFromTrueCount(trueCount);
    double loseRate = 1.0 - winRate;
    double edge = winRate - loseRate;

    // 簡化 Kelly Criterion：下注比例約等於 edge。
    // 這裡使用 half Kelly，避免下注過大。
    double ratio;

    if (edge <= 0) {
        ratio = 0.01;
    } else {
        ratio = edge / 2.0;
    }

    if (ratio < 0.01) {
        ratio = 0.01;
    }

    if (ratio > 0.20) {
        ratio = 0.20;
    }

    int bet = static_cast<int>(balance * ratio);

    if (bet < minBet) {
        bet = minBet;
    }

    if (bet > balance) {
        bet = balance;
    }

    // 下注金額取 10 的倍數，比較像籌碼。
    bet = (bet / 10) * 10;

    if (bet <= 0) {
        bet = min(balance, minBet);
    }

    return bet;
}

string Strategy::suggestAction(
    const Hand& playerHand,
    const Card& dealerUpCard,
    bool canDouble,
    double standWinRate,
    double hitWinRate
) const {
    int total = playerHand.getValue();
    int dealerValue = dealerUpCard.getValue();

    if (total >= 17) {
        return "STAND";
    }

    if (canDouble && total >= 9 && total <= 11 && hitWinRate > standWinRate + 0.03) {
        return "DOUBLE";
    }

    if (total <= 11) {
        return "HIT";
    }

    if (total >= 12 && total <= 16) {
        if (dealerValue >= 2 && dealerValue <= 6) {
            if (hitWinRate > standWinRate + 0.05) {
                return "HIT";
            }

            return "STAND";
        }

        return "HIT";
    }

    if (hitWinRate > standWinRate + 0.02) {
        return "HIT";
    }

    return "STAND";
}
