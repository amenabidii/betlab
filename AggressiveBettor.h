#ifndef AGGRESSIVEBETTOR_H
#define AGGRESSIVEBETTOR_H
#include "Bettor.h"
// Risks a larger portion of the balance for each opportunity.
class AggressiveBettor : public Bettor {
public:
    AggressiveBettor();
    AggressiveBettor(int id, string name, double balance);
    AggressiveBettor(const AggressiveBettor& other);
    string getType() const override;
    void performTurn(World& world) override;
    Entity* clone() const override;
};
#endif
