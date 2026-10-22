#include "AnalyticalBettor.h"
#include "World.h"
AnalyticalBettor::AnalyticalBettor() : Bettor() {}
AnalyticalBettor::AnalyticalBettor(int id, string name, double balance) : Bettor(id, name, balance) {}
AnalyticalBettor::AnalyticalBettor(const AnalyticalBettor& other) : Bettor(other) {}
string AnalyticalBettor::getType() const { return "Analytical"; }
void AnalyticalBettor::performTurn(World& world) {
    const Opportunity& chance = world.getOpportunity();
    double estimatedReturn = chance.publicChance * chance.odds - 1;
    if (estimatedReturn <= 0.02) { cout << getName() << " skips (low expected return).\n"; return; }
    if (!placeBet(world, getBalance() * 0.10)) cout << getName() << " cannot place a bet.\n";
}
void AnalyticalBettor::analyze(const Opportunity& chance) const {
    cout << "Published estimated chance: " << chance.publicChance * 100 << "%\n";
    cout << "Estimated return per credit: " << chance.publicChance * chance.odds - 1 << "\n";
}
Entity* AnalyticalBettor::clone() const { return new AnalyticalBettor(*this); }
