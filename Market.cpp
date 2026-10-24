#include "Market.h"
#include <cstdlib>
using namespace std;

// Match setup and result generation. Fictional games and odds change across turns.
Market::Market() {
    condition = 0.50;
}
const Opportunity& Market::getOpportunity() const {
    return current;
}
void Market::makeOpportunity(int turn) {
    // Fictional fixtures rotate so the console does not repeat one match forever.
    const string home[] = {
        "Cairo Falcons", "Tunis Stars", "Nile Titans", "Alexandria Waves",
        "Casablanca Lions", "Giza Comets", "Beirut Cedars", "Luxor Suns",
        "Rabat Royals", "Amman Wolves", "Sfax Sharks", "Dubai Meteors",
        "Cairo Eagles", "Marrakesh Foxes", "Aswan Raptors", "Tripoli Storm"
    };
    const string away[] = {
        "Tunis Eagles", "Carthage Warriors", "Cairo Kings", "Giza Rockets",
        "Nile Panthers", "Tunis Phoenix", "Alexandria Knights", "Amman Bears",
        "Beirut Falcons", "Rabat Thunder", "Casablanca Bulls", "Cairo Hawks",
        "Luxor Giants", "Nile Cobras", "Beirut Waves", "Marrakesh Stars"
    };
    const string venues[] = {
        "Cairo Arena", "Tunis Sports Hall", "Nile Dome", "Alexandria Arena",
        "Atlantic Court", "Giza Arena", "Cedars Stadium", "Luxor Indoor Hall",
        "Rabat Arena", "Amman Sports City", "Sfax Dome", "Desert Court",
        "Cairo Arena", "Marrakesh Arena", "Aswan Hall", "Mediterranean Arena"
    };
    const string leagues[] = {
        "BETLAB Basketball League", "North Africa Basketball Cup",
        "BETLAB International League", "Mediterranean Basketball Series"
    };
    int index = (turn - 1) % 16;
    current.homeTeam = home[index];
    current.awayTeam = away[index];
    current.venue = venues[index];
    current.competition = leagues[index % 4];
    current.format = "Four quarters; overtime if tied";
    current.actualChance = condition;
    current.publicChance = condition + (rand() % 21 - 10) / 100.0;
    if (current.publicChance < 0.10) current.publicChance = 0.10;
    if (current.publicChance > 0.90) current.publicChance = 0.90;
    // Decimal odds use a small house margin; both choices are available.
    current.homeOdds = 0.96 / current.publicChance;
    current.awayOdds = 0.96 / (1.0 - current.publicChance);
    if (current.homeOdds < 1.05) current.homeOdds = 1.05;
    if (current.awayOdds < 1.05) current.awayOdds = 1.05;
    current.homeScore = 0;
    current.awayScore = 0;
}
bool Market::getOutcome() const {
    return rand() % 10000 < current.actualChance * 10000;
}
void Market::makeFinalScore(bool homeWon) {
    int first = 73 + rand() % 33;
    int second = 73 + rand() % 33;
    if (first == second) first++;
    if (homeWon && first < second) {
        int temp = first;
        first = second;
        second = temp;
    }
    if (!homeWon && first > second) {
        int temp = first;
        first = second;
        second = temp;
    }
    current.homeScore = first;
    current.awayScore = second;
}
bool Market::changeCondition(double value) {
    if (value < 0.10 || value > 0.90) return false;
    condition = value;
    return true;
}
double Market::getCondition() const {
    return condition;
}
