#include "Game.h"

#include <iostream>
#include <limits>
#include <cstdlib>

using namespace std;

int main() {
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    cout << "請輸入使用幾副牌，建議 4 或 6：";

    int decks;

    while (!(cin >> decks) || decks < 1 || decks > 8) {
        cout << "請輸入 1 到 8 之間的整數：";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "請輸入初始資金，建議 1000：";

    int money;

    while (!(cin >> money) || money <= 0) {
        cout << "請輸入大於 0 的整數：";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    Game game(decks, money);
    game.start();

    return 0;
}
