#include "AnalyticalBettor.h"
#include "World.h"
#include <iomanip>
using namespace std;

// The analytical strategy: he compare possible returns before deciding whether either team is worth betting on.
AnalyticalBettor::AnalyticalBettor() : Bettor() {
}
AnalyticalBettor::AnalyticalBettor(int id, string name, double balance) : Bettor(id, name, balance) {
}
AnalyticalBettor::AnalyticalBettor(const AnalyticalBettor& other) : Bettor(other) {
}
string AnalyticalBettor::getType() const {
    return "Analytical";
}
void AnalyticalBettor::performTurn(World& world) {
    const Opportunity& match = world.getOpportunity();
    
    // He makes its own probability estimate
	// The estimate is based on the public chance, but it varies slightly with the turn number and bettor ID to avoid everyone making the same choice

    double estimate = match.publicChance + ((world.getTurn() + getId()) % 7 - 3) * 0.025;
    if (estimate < 0.05) estimate = 0.05;
    if (estimate > 0.95) estimate = 0.95;
    double homeReturn = estimate * match.homeOdds - 1;
    double awayReturn = (1 - estimate) * match.awayOdds - 1;
    if (homeReturn <= 0.02 && awayReturn <= 0.02) {
        cout << getName() << " [Analytical] skipped: neither selection meets the threshold.\n";
        return;
    }
    bool home = homeReturn >= awayReturn;
    if (!placeBet(world, getBalance() * 0.10, home)) cout << getName() << " cannot place a bet.\n";
}
void AnalyticalBettor::analyze(const Opportunity& match) const {
    cout << fixed << setprecision(2)
    << "Estimated " << match.homeTeam << " return per credit: "
    << match.publicChance * match.homeOdds - 1 << "\n"
    << "Estimated " << match.awayTeam << " return per credit: "
    << (1 - match.publicChance) * match.awayOdds - 1 << "\n";
}
Entity* AnalyticalBettor::clone() const {
    return new AnalyticalBettor(*this);
}
