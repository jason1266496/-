#include "Game.h"
#include "WinRateSimulator.h"

#include <iostream>
#include <iomanip>
#include <limits>
#include <cctype>

using namespace std;

Game::Game(int decks, int money)
    : totalDeckCount(decks),
      deck(decks),
      player(money),
      dealerHiddenCounted(false) {}

void Game::printLine() const {
    cout << "--------------------------------------------------\n";
}

int Game::readInt(const string& prompt, int minValue, int maxValue) const {
    while (true) {
        cout << prompt;

        int value;

        if (cin >> value && value >= minValue && value <= maxValue) {
            return value;
        }

        cout << "輸入錯誤，請輸入 " << minValue << " 到 " << maxValue << " 之間的整數。\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

char Game::readAction(bool canDouble) const {
    while (true) {
        cout << "請選擇動作：H = 要牌, S = 停牌";

        if (canDouble) {
            cout << ", D = 加倍";
        }

        cout << "：";

        string input;
        cin >> input;

        if (input.empty()) {
            continue;
        }

        char action = static_cast<char>(tolower(static_cast<unsigned char>(input[0])));

        if (action == 'h' || action == 's') {
            return action;
        }

        if (canDouble && action == 'd') {
            return action;
        }

        cout << "無效動作，請重新輸入。\n";
    }
}

Card Game::drawVisibleCard() {
    Card card = deck.drawCard();
    counter.updateCount(card);
    return card;
}

void Game::revealDealerHiddenCard() {
    const vector<Card>& dealerCards = dealer.getHand().getCards();

    if (!dealerHiddenCounted && dealerCards.size() >= 2) {
        counter.updateCount(dealerCards[1]);
        dealerHiddenCounted = true;
    }
}

vector<Card> Game::buildUnseenCardsForSimulation() const {
    vector<Card> unseen = deck.getRemainingCards();
    const vector<Card>& dealerCards = dealer.getHand().getCards();

    // 玩家不知道莊家的暗牌，所以模擬勝率時，要把暗牌也視為未知牌之一。
    if (!dealerHiddenCounted && dealerCards.size() >= 2) {
        unseen.push_back(dealerCards[1]);
    }

    return unseen;
}

void Game::reshuffleIfNeeded() {
    int cutCardLimit = static_cast<int>(totalDeckCount * 52 * 0.25);

    if (deck.remainingCards() <= cutCardLimit) {
        cout << "\n牌堆剩餘牌數不足，重新洗牌並重置算牌數值。\n";
        deck.initialize(totalDeckCount);
        deck.shuffleDeck();
        counter.reset();
    }
}

void Game::showTable(bool revealDealer) const {
    cout << "\n目前牌桌狀態\n";
    printLine();

    cout << "莊家手牌：";

    const vector<Card>& dealerCards = dealer.getHand().getCards();

    if (revealDealer) {
        dealer.getHand().show();
    } else {
        if (!dealerCards.empty()) {
            cout << dealerCards[0].toString() << " [暗牌] ";
            cout << "(明牌點數: " << dealerCards[0].getValue() << ")";
        }
    }

    cout << "\n玩家手牌：";
    player.getHand().show();

    cout << "\n目前下注：" << player.getCurrentBet();
    cout << "\n玩家資金：" << player.getBalance();
    cout << "\nRunning Count：" << counter.getRunningCount();
    cout << "\nTrue Count：" << fixed << setprecision(2) << counter.getTrueCount(deck.remainingCards());
    cout << "\n";

    printLine();
}

void Game::dealInitialCards() {
    player.getHand().addCard(drawVisibleCard());
    dealer.getHand().addCard(drawVisibleCard());
    player.getHand().addCard(drawVisibleCard());

    // 莊家第二張是暗牌，玩家看不到，所以先不更新 Running Count。
    dealer.getHand().addCard(deck.drawCard());
    dealerHiddenCounted = false;
}

bool Game::checkInitialBlackjack() {
    bool playerBJ = player.getHand().isBlackjack();
    bool dealerBJ = dealer.getHand().isBlackjack();

    if (!playerBJ && !dealerBJ) {
        return false;
    }

    revealDealerHiddenCard();
    showTable(true);

    if (playerBJ && dealerBJ) {
        cout << "玩家與莊家都是 Blackjack，平手，退回下注。\n";
        player.pushBet();
    } else if (playerBJ) {
        cout << "玩家 Blackjack！獲得 1.5 倍獎金。\n";
        player.winBlackjack();
    } else {
        cout << "莊家 Blackjack，玩家輸。\n";
        player.loseBet();
    }

    cout << "結算後資金：" << player.getBalance() << "\n";

    return true;
}

void Game::playerTurn() {
    bool firstDecision = true;

    while (true) {
        showTable(false);

        vector<Card> unseenCards = buildUnseenCardsForSimulation();
        Card dealerUpCard = dealer.getHand().getCards()[0];

        double standWinRate = WinRateSimulator::estimateStandWinRate(
            player.getHand(),
            dealerUpCard,
            unseenCards,
            3000
        );

        double hitWinRate = WinRateSimulator::estimateHitWinRate(
            player.getHand(),
            dealerUpCard,
            unseenCards,
            3000
        );

        bool canDouble = firstDecision && player.canDouble();

        string advice = strategy.suggestAction(
            player.getHand(),
            dealerUpCard,
            canDouble,
            standWinRate,
            hitWinRate
        );

        cout << fixed << setprecision(2);
        cout << "停牌勝率估算：約 " << standWinRate * 100 << "%\n";
        cout << "要牌勝率估算：約 " << hitWinRate * 100 << "%\n";
        cout << "系統建議動作：" << advice << "\n";

        if (player.getHand().getValue() >= 21) {
            break;
        }

        char action = readAction(canDouble);

        if (action == 's') {
            cout << "玩家選擇停牌。\n";
            break;
        }

        if (action == 'h') {
            Card card = drawVisibleCard();
            player.getHand().addCard(card);

            cout << "玩家抽到：" << card.toString() << "\n";

            if (player.getHand().isBust()) {
                showTable(false);
                cout << "玩家爆牌。\n";
                break;
            }
        }

        if (action == 'd') {
            if (player.doubleBet()) {
                Card card = drawVisibleCard();
                player.getHand().addCard(card);

                cout << "玩家選擇加倍，抽到：" << card.toString() << "\n";
                showTable(false);
                break;
            } else {
                cout << "資金不足，無法加倍。\n";
            }
        }

        firstDecision = false;
    }
}

void Game::dealerTurn() {
    revealDealerHiddenCard();

    cout << "\n莊家翻開暗牌。\n";
    showTable(true);

    while (dealer.getHand().getValue() < 17) {
        Card card = drawVisibleCard();
        dealer.getHand().addCard(card);

        cout << "莊家抽到：" << card.toString() << "\n";
        showTable(true);
    }
}

void Game::settleRound() {
    int playerTotal = player.getHand().getValue();
    int dealerTotal = dealer.getHand().getValue();

    cout << "\n本局結果\n";
    printLine();

    cout << "玩家點數：" << playerTotal << "\n";
    cout << "莊家點數：" << dealerTotal << "\n";

    if (playerTotal > 21) {
        cout << "玩家爆牌，玩家輸。\n";
        player.loseBet();
    } else if (dealerTotal > 21) {
        cout << "莊家爆牌，玩家贏。\n";
        player.winBet();
    } else if (playerTotal > dealerTotal) {
        cout << "玩家點數較高，玩家贏。\n";
        player.winBet();
    } else if (playerTotal < dealerTotal) {
        cout << "莊家點數較高，玩家輸。\n";
        player.loseBet();
    } else {
        cout << "平手，退回下注。\n";
        player.pushBet();
    }

    cout << "結算後資金：" << player.getBalance() << "\n";

    printLine();
}

bool Game::playOneRound() {
    reshuffleIfNeeded();

    player.newRound();
    dealer.clearHand();
    dealerHiddenCounted = false;

    double trueCount = counter.getTrueCount(deck.remainingCards());
    double estimatedWinRate = strategy.estimateWinRateFromTrueCount(trueCount);
    int suggestedBet = strategy.suggestBet(player.getBalance(), trueCount);

    cout << "\n新的一局\n";
    printLine();

    cout << "玩家資金：" << player.getBalance() << "\n";
    cout << "剩餘牌數：" << deck.remainingCards() << "\n";
    cout << "Running Count：" << counter.getRunningCount() << "\n";

    cout << fixed << setprecision(2);
    cout << "True Count：" << trueCount << "\n";
    cout << "依 True Count 預估勝率：約 " << estimatedWinRate * 100 << "%\n";
    cout << "系統建議下注：" << suggestedBet << "\n";
    cout << "輸入 0 可以結束遊戲。\n";

    printLine();

    int bet = readInt("請輸入下注金額：", 0, player.getBalance());

    if (bet == 0) {
        return false;
    }

    if (!player.placeBet(bet)) {
        cout << "下注失敗。\n";
        return true;
    }

    dealInitialCards();
    showTable(false);

    if (checkInitialBlackjack()) {
        return true;
    }

    playerTurn();

    if (player.getHand().isBust()) {
        settleRound();
        return true;
    }

    dealerTurn();
    settleRound();

    return true;
}

void Game::start() {
    cout << "\nBlackjack 21 點算牌與下注決策系統\n";
    cout << "牌面說明：S=黑桃, H=紅心, D=方塊, C=梅花\n";
    cout << "算牌方法：Hi-Lo Counting System\n";
    cout << "2~6 = +1，7~9 = 0，10/J/Q/K/A = -1\n";

    printLine();

    while (player.getBalance() > 0) {
        bool continueGame = playOneRound();

        if (!continueGame) {
            break;
        }

        if (player.getBalance() <= 0) {
            cout << "資金歸零，遊戲結束。\n";
            break;
        }

        cout << "是否繼續下一局？Y/N：";

        string input;
        cin >> input;

        if (input.empty()) {
            break;
        }

        char choice = static_cast<char>(tolower(static_cast<unsigned char>(input[0])));

        if (choice != 'y') {
            break;
        }
    }

    cout << "\n遊戲結束，最終資金：" << player.getBalance() << "\n";
}
