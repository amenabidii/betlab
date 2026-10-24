#include "AggressiveBettor.h"
#include "World.h"

#include <iostream>

using namespace std;

// Aggressive strategy: higher odds can give a bigger payout, but the selection is usually riskier.


AggressiveBettor::AggressiveBettor() : Bettor() {
}

AggressiveBettor::AggressiveBettor(int id, string name, double balance)
: Bettor(id, name, balance) {
}

// We pass by const reference because we only need to read the original
// Bettors copy constructor also copies the betting history independently
AggressiveBettor::AggressiveBettor(const AggressiveBettor& other)
: Bettor(other) {
}

string AggressiveBettor::getType() const {
    return "Aggressive";
}

void AggressiveBettor::performTurn(World& world) {

    // Aggressive bettors risk 22% of their current balance.
    double stake = getBalance() * 0.22;

    // This bettor chooses the team with higher odds because it offers a bigger payout.
    bool homeSelection = world.getOpportunity().homeOdds > world.getOpportunity().awayOdds;

    if (!placeBet(world, stake, homeSelection)) {
        cout << getName() << " cannot place a bet.\n";
    }
}

Entity* AggressiveBettor::clone() const {

    // The copy constructor makes a separate copy of the betting history 
    return new AggressiveBettor(*this);
}
