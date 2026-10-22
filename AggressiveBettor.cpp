#include "AggressiveBettor.h"
#include "World.h"
AggressiveBettor::AggressiveBettor() : Bettor() {}
AggressiveBettor::AggressiveBettor(int id, string name, double balance) : Bettor(id, name, balance) {}
AggressiveBettor::AggressiveBettor(const AggressiveBettor& other) : Bettor(other) {}
string AggressiveBettor::getType() const { return "Aggressive"; }
void AggressiveBettor::performTurn(World& world) {
    if (!placeBet(world, getBalance() * 0.22)) cout << getName() << " cannot place a bet.\n";
}
Entity* AggressiveBettor::clone() const { return new AggressiveBettor(*this); }
