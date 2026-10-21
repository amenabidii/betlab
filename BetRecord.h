#ifndef BETRECORD_H
#define BETRECORD_H

using namespace std;

// This class stores the details of one completed bet:
class BetRecord {
private:
    int turnNumber;
    double amountBet;
    double odds;
    bool won;

public:
    BetRecord() {
        turnNumber = 0;
        amountBet = 0;
        odds = 1;
        won = false;
    }

    BetRecord(int turn, double amount, double betOdds, bool result) {
        turnNumber = turn;
        amountBet = amount;
        odds = betOdds;
        won = result;
    }

    BetRecord(const BetRecord& other) {
        turnNumber = other.turnNumber;
        amountBet = other.amountBet;
        odds = other.odds;
        won = other.won;
    }

    int getTurnNumber() const { return turnNumber; }
    double getAmountBet() const { return amountBet; }
    double getOdds() const { return odds; }
    bool getWon() const { return won; }

    // If the bet wins, the stake is returned separately from the profit.
    double getNetProfit() const {
        if (won) {
            return amountBet * (odds - 1);
        }
        return -amountBet;
    }
};

#endif