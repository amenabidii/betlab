#include "ConservativeBettor.h"
#include "World.h"
#include <algorithm>
ConservativeBettor::ConservativeBettor() : Bettor() {}
ConservativeBettor::ConservativeBettor(int id, string name, double balance) : Bettor(id, name, balance) {}
ConservativeBettor::ConservativeBettor(const ConservativeBettor& other) : Bettor(other) {}
string ConservativeBettor::getType() const { return "Conservative"; }
void ConservativeBettor::performTurn(World& world) {
    const Opportunity& chance = world.getOpportunity();
    if (chance.publicChance < 0.58) { cout << getName() << " skips (too risky).\n"; return; }
    double stake = min(30.0, getBalance() * 0.05);
    if (!placeBet(world, stake)) cout << getName() << " cannot place a bet.\n";
}
Entity* ConservativeBettor::clone() const { return new ConservativeBettor(*this); }
