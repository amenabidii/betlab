#ifndef CAPABILITIES_H
#define CAPABILITIES_H
#include "Opportunity.h"

// Extra actions that only certain bettors can perform. The runtime casting checks whether the selected bettor supports an action.
// Only some bettors can do these extra actions. Normal turns do not use 

class ProbabilityAnalysis {
public:
    virtual void analyze(const Opportunity& opportunity) const = 0;
    virtual ~ProbabilityAnalysis() {
    }
};
class StrategyAdaptation {
public:
    virtual void adapt() = 0;
    virtual ~StrategyAdaptation() {
    }
};
#endif
