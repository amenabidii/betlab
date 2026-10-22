#ifndef ANALYTICALBETTOR_H
#define ANALYTICALBETTOR_H
#include "Bettor.h"
#include "Capabilities.h"
// Compares estimated chance with the published odds. It does not see the hidden chance.
class AnalyticalBettor : public Bettor, public ProbabilityAnalysis {
public:
    AnalyticalBettor();
    AnalyticalBettor(int id, string name, double balance);
    AnalyticalBettor(const AnalyticalBettor& other);
    string getType() const override;
    void performTurn(World& world) override;
    void analyze(const Opportunity& opportunity) const override;
    Entity* clone() const override;
};
#endif
