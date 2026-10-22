#ifndef ADAPTIVEBETTOR_H
#define ADAPTIVEBETTOR_H
#include "Bettor.h"
#include "Capabilities.h"
// Changes its stake percentage using its previous results.
class AdaptiveBettor : public Bettor, public StrategyAdaptation {
private:
    double stakePercent;
public:
    AdaptiveBettor();
    AdaptiveBettor(int id, string name, double balance);
    AdaptiveBettor(const AdaptiveBettor& other);
    string getType() const override;
    void performTurn(World& world) override;
    void adapt() override;
    void print() const override;
    Entity* clone() const override;
};
#endif
