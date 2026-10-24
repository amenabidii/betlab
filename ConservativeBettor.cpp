#include "ConservativeBettor.h"
#include "World.h"
#include <algorithm>
using namespace std;

// Conservative strategy: choose the more likely team and skip a match when the chances are too close.
ConservativeBettor::ConservativeBettor() : Bettor() {
}
ConservativeBettor::ConservativeBettor(int id, string name, double balance) : Bettor(id, name, balance) {
}
ConservativeBettor::ConservativeBettor(const ConservativeBettor& other) : Bettor(other) {
}
string ConservativeBettor::getType() const {
    return "Conservative";
}
void ConservativeBettor::performTurn(World& world) {
    const Opportunity& match = world.getOpportunity();
    bool home = match.publicChance >= 0.50;
    double betterChance = home ? match.publicChance : 1.0 - match.publicChance;
    if (betterChance < 0.58) {
        cout << getName() << " [Conservative] skipped: neither team feels safe enough.\n";
        return;
    }
    double stake = min(50.0, getBalance() * 0.05);
    if (!placeBet(world, stake, home)) cout << getName() << " cannot place a bet.\n";
}
Entity* ConservativeBettor::clone() const {
    return new ConservativeBettor(*this);
}
