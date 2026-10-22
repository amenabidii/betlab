#ifndef OPPORTUNITY_H
#define OPPORTUNITY_H
#include <string>
using namespace std;

// The same opportunity is shown to every bettor in a turn.
class Opportunity {
public:
    string title;
    double odds;
    double publicChance;
    double actualChance;
    Opportunity() {
        title = "Team Alpha wins"; odds = 2.0;
        publicChance = 0.50; actualChance = 0.50;
    }
};
#endif
