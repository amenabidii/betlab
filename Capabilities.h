#ifndef CAPABILITIES_H
#define CAPABILITIES_H
#include "Opportunity.h"

// Only some bettors can do these extra actions. Normal turns do not use casts.
class ProbabilityAnalysis {
public:
    virtual void analyze(const Opportunity& opportunity) const = 0;
    virtual ~ProbabilityAnalysis() {}
};
class StrategyAdaptation {
public:
    virtual void adapt() = 0;
    virtual ~StrategyAdaptation() {}
};
#endif
