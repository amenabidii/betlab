#ifndef BETRECORD_H
#define BETRECORD_H
#include <string>
using namespace std;

// This is the class for one completed bet. We save the team name (selectedteam) because future turns have other teams.
class BetRecord {
private:

    int turnNumber;
    double amountBet;
    double odds;
    bool won;
    string selectedTeam;

public:
    BetRecord() {
        turnNumber = 0;
        amountBet = 0;
        odds = 1;
        won = false;
        selectedTeam = "";
    }

    BetRecord(int turn, double amount, double betOdds, bool result, string team = "") {
        turnNumber = turn;
        amountBet = amount;
        odds = betOdds;
        won = result;
        selectedTeam = team;
    }

    BetRecord(const BetRecord& other) {
        turnNumber = other.turnNumber;
        amountBet = other.amountBet;
        odds = other.odds;
        won = other.won;
        selectedTeam = other.selectedTeam;
    }

    BetRecord& operator=(const BetRecord& other) {
        if (this != &other) {
            turnNumber = other.turnNumber;
            amountBet = other.amountBet;
            odds = other.odds;
            won = other.won;
            selectedTeam = other.selectedTeam;
        }
        return *this;
    }

    int getTurnNumber() const {
        return turnNumber;
    }

    double getAmountBet() const {
        return amountBet;
    }

    double getOdds() const {
        return odds;
    }
    bool getWon() const {
        return won;
    }

    string getSelectedTeam() const {
        return selectedTeam;
    }
    double getNetProfit() const {
        if (won) return amountBet * (odds - 1);
        return -amountBet;
    }

    // In my first draft I used these names and when I remove them It's complicated to identify the error
    // so I kept these names bcause I used them in the other files (i did this because it's much easier than modifying all of them again)
    int getTurn() const {
        return getTurnNumber();
    }
    double getStake() const {
        return getAmountBet();
    }
    double getProfit() const {
        return getNetProfit();
    }
};
#endif
