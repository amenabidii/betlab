#include "AdaptiveBettor.h"
#include "World.h"
AdaptiveBettor::AdaptiveBettor() : Bettor() { stakePercent = 0.10; }
AdaptiveBettor::AdaptiveBettor(int id, string name, double balance) : Bettor(id, name, balance) { stakePercent = 0.10; }
AdaptiveBettor::AdaptiveBettor(const AdaptiveBettor& other) : Bettor(other) { stakePercent = other.stakePercent; }
string AdaptiveBettor::getType() const { return "Adaptive"; }
void AdaptiveBettor::performTurn(World& world) {
    adapt();
    if (!placeBet(world, getBalance() * stakePercent)) cout << getName() << " cannot place a bet.\n";
}
void AdaptiveBettor::adapt() {
    int count = getHistorySize();
    if (count == 0) { cout << "No history yet; using starting strategy.\n"; return; }
    if (getRecord(count - 1).getWon()) stakePercent += 0.01;
    else stakePercent -= 0.015;
    if (stakePercent < 0.03) stakePercent = 0.03;
    if (stakePercent > 0.20) stakePercent = 0.20;
    cout << getName() << " now bets " << stakePercent * 100 << "% of balance.\n";
}
void AdaptiveBettor::print() const {
    Bettor::print();
    cout << "  Stake percentage: " << stakePercent * 100 << "%\n";
}
Entity* AdaptiveBettor::clone() const { return new AdaptiveBettor(*this); }
