#include "AdaptiveBettor.h"
#include "World.h"
using namespace std;

// Adaptive strategy: the amount risked goes up or down depending on the latest finished bet.

AdaptiveBettor::AdaptiveBettor() : Bettor() {
    stakePercent = 0.10;
}

AdaptiveBettor::AdaptiveBettor(int id, string name, double balance) : Bettor(id, name, balance) {
    stakePercent = 0.10;
}

AdaptiveBettor::AdaptiveBettor(const AdaptiveBettor& other) : Bettor(other) {
    stakePercent = other.stakePercent;
}

string AdaptiveBettor::getType() const {
    return "Adaptive";
}

void AdaptiveBettor::performTurn(World& world) {
    adapt();

    // Follow the public favorite after changing risk from previous bets.

    bool home = world.getOpportunity().publicChance >= 0.50;
    if (!placeBet(world, getBalance() * stakePercent, home))
    cout << getName() << " cannot place a bet.\n";
}

void AdaptiveBettor::adapt() {
    int count = getHistorySize();
    if (count == 0) return;
    if (getRecord(count - 1).getWon()) stakePercent += 0.01;
    else stakePercent -= 0.015;
    if (stakePercent < 0.03) stakePercent = 0.03;
    if (stakePercent > 0.20) stakePercent = 0.20;
    cout << getName() << " [Adaptive] now risks " << stakePercent * 100 << "% of balance.\n";
}
void AdaptiveBettor::print() const {
    Bettor::print();
    cout << "  Stake percentage: " << stakePercent * 100 << "%\n";
}
Entity* AdaptiveBettor::clone() const {
    return new AdaptiveBettor(*this);
}
