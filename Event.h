#ifndef EVENT_H
#define EVENT_H
#include <string>
using namespace std;

// this is for one pending action, such as a bet request, deposit or market change..
enum EventType {
    BET_REQUEST = 1, DEPOSIT = 2, MARKET_CHANGE = 3
};
class Event {
public:
    EventType type;
    int entityId;
    double amount;
    bool homeSelection;
    string description;
    Event() {
        type = BET_REQUEST;
        entityId = 0;
        amount = 0;
        homeSelection = true;
    }
    Event(EventType t, int id, double value, string text, bool home = true) {
        type = t;
        entityId = id;
        amount = value;
        description = text;
        homeSelection = home;
    }
};
#endif
