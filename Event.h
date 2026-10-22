#ifndef EVENT_H
#define EVENT_H
#include <string>
using namespace std;
// An event is stored first, then handled in the same order it was added.
enum EventType { BET_REQUEST = 1, DEPOSIT = 2, MARKET_CHANGE = 3 };
class Event {
public:
    EventType type;
    int entityId;
    double amount;
    string description;
    Event() { type = BET_REQUEST; entityId = 0; amount = 0; }
    Event(EventType t, int id, double value, string text) {
        type = t; entityId = id; amount = value; description = text;
    }
};
#endif
