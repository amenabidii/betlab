#ifndef MARKET_H
#define MARKET_H
#include "Opportunity.h"
// Market decides one shared outcome for all bettors. The actual chance is hidden from them.
class Market {
private:
    double condition;
    Opportunity current;
public:
    Market();
    const Opportunity& getOpportunity() const;
    void makeOpportunity(int turn);
    bool getOutcome() const;
    bool changeCondition(double newCondition);
    double getCondition() const;
};
#endif
