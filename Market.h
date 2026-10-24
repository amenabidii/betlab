#ifndef MARKET_H
#define MARKET_H
#include "Opportunity.h"

// The market stores the upcoming match and controls the simulated outcome.
class Market {
private:
    double condition;
    Opportunity current;
public:
    Market();
    const Opportunity& getOpportunity() const;
    void makeOpportunity(int turn);
    bool getOutcome() const;
    void makeFinalScore(bool homeWon);
    bool changeCondition(double newCondition);
    double getCondition() const;
};
#endif
