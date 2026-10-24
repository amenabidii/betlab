#ifndef OPPORTUNITY_H
#define OPPORTUNITY_H
#include <string>
using namespace std;

// Information for one upcoming fictional match, including both teams, their odds and the final scores.

// this stores match details, betting odds, win probabilities, and scores
// publicChance is the estimated chance of a home win shown to bettors
// actualChance is used by the simulation to determine the outcome

class Opportunity {
public:
    string competition, homeTeam, awayTeam, venue, format;
    double homeOdds, awayOdds;
    double publicChance, actualChance;
    int homeScore, awayScore;

    Opportunity() {
        competition = "betlab Basketball League";
        homeTeam = "Cairo Falcons";
        awayTeam = "Tunis Eagles";
        venue = "Cairo Arena";
        format = "Four quarters; overtime if tied";
        homeOdds = 1.85;
        awayOdds = 2.05;
        publicChance = 0.52;
        actualChance = 0.52;
        homeScore = 0;
        awayScore = 0;
    }
};
#endif
