CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET = blackjack
SOURCES = main.cpp Card.cpp Deck.cpp Hand.cpp CardCounter.cpp Player.cpp Dealer.cpp Strategy.cpp WinRateSimulator.cpp Game.cpp

all:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) blackjack.exe
