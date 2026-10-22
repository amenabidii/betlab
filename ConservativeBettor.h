#ifndef CONSERVATIVEBETTOR_H
#define CONSERVATIVEBETTOR_H
#include "Bettor.h"
// Uses small bets and skips opportunities with poor odds.
class ConservativeBettor : public Bettor {
public:
    ConservativeBettor();
    ConservativeBettor(int id, string name, double balance);
    ConservativeBettor(const ConservativeBettor& other);
    string getType() const override;
    void performTurn(World& world) override;
    Entity* clone() const override;
};
#endif
