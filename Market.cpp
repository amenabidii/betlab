#include "Market.h"
#include <cstdlib>
Market::Market() { condition = 0.50; }
const Opportunity& Market::getOpportunity() const { return current; }
void Market::makeOpportunity(int turn) {
    current.title = "Match " + std::to_string(turn) + ": Team Alpha wins";
    // We add a little uncertainty to public information. It isn't the true chance.
    int change = rand() % 21 - 10;
    current.actualChance = condition;
    current.publicChance = condition + change / 100.0;
    if (current.publicChance < 0.10) current.publicChance = 0.10;
    if (current.publicChance > 0.90) current.publicChance = 0.90;
    current.odds = 0.97 / current.publicChance;
    if (current.odds < 1.05) current.odds = 1.05;
}
bool Market::getOutcome() const {
    int number = rand() % 10000;
    return number < current.actualChance * 10000;
}
bool Market::changeCondition(double value) {
    if (value < 0.10 || value > 0.90) return false;
    condition = value; return true;
}
double Market::getCondition() const { return condition; }
