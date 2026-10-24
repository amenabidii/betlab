#ifndef AGGRESSIVEBETTOR_H
#define AGGRESSIVEBETTOR_H

#include "Bettor.h"

using namespace std;

// Declares the aggressive bettor. It usually chooses higher odds and risks a bigger part of the balance.

// This bettor takes higher risks by betting a larger part of their balance.
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
