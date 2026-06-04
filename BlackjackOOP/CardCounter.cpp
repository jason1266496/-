#include "CardCounter.h"

CardCounter::CardCounter() : runningCount(0) {}

void CardCounter::reset() {
    runningCount = 0;
}

void CardCounter::updateCount(const Card& card) {
    runningCount += card.getCountValue();
}

int CardCounter::getRunningCount() const {
    return runningCount;
}

double CardCounter::getTrueCount(int remainingCards) const {
    double decksLeft = static_cast<double>(remainingCards) / 52.0;

    if (decksLeft < 0.25) {
        decksLeft = 0.25;
    }

    return runningCount / decksLeft;
}
