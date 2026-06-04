Blackjack 21 點算牌與下注決策系統

一、檔案說明
main.cpp                主程式入口
Card.h / Card.cpp        撲克牌類別
Deck.h / Deck.cpp        牌堆類別
Hand.h / Hand.cpp        手牌類別
Player.h / Player.cpp    玩家類別
Dealer.h / Dealer.cpp    莊家類別
CardCounter.h / .cpp     Hi-Lo 算牌類別
Strategy.h / .cpp        下注與行動建議類別
WinRateSimulator.h/.cpp  Monte Carlo 勝率模擬類別
Game.h / Game.cpp        遊戲流程控制類別

二、編譯方式
在此資料夾開啟終端機，輸入：

g++ main.cpp Card.cpp Deck.cpp Hand.cpp CardCounter.cpp Player.cpp Dealer.cpp Strategy.cpp WinRateSimulator.cpp Game.cpp -o blackjack.exe -std=c++17

三、執行方式
Windows：
blackjack.exe

Mac / Linux：
./blackjack.exe

四、系統功能
1. 可遊玩的 21 點遊戲
2. 玩家下注與資金管理
3. 莊家自動補牌
4. Hi-Lo 算牌法
5. Running Count 與 True Count
6. Monte Carlo 模擬勝率
7. 根據勝率建議 Hit / Stand / Double
8. 根據 True Count 與簡化 Kelly Criterion 建議下注金額
