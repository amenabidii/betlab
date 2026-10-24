#ifndef WORLD_H
#define WORLD_H
#include <vector>
#include "Entity.h"
#include "Market.h"
#include "Event.h"
#include "CustomQueue.h"
using namespace std;

// World manages the bettors, upcoming match, pending events, and the turn number.

// World owns all entities in ONE collection, plus market data and pending events.
// When it is copied for undo, we clone every entity into new memory.
class World {
private:
    vector<Entity*> entities;
    CustomQueue<Event> pending;
    Market market;
    int nextId;
    int turn;
    void clearEntities();
    void copyEntities(const World& other);
    void processEvents();
public:
    World();
    World(const World& other);
    World& operator=(const World& other);
    ~World();
    int getTurn() const;
    int getNextId() const;
    const Opportunity& getOpportunity() const;
    Market& getMarket();
    Entity* find(int id) const;
    bool add(int type, string name, double balance);
    bool remove(int id);
    bool duplicate(int id);
    bool rename(int id, string name);
    bool changeBalance(int id, double amount);
    bool requestBet(int id, double stake, bool homeSelection);
    bool addUserEvent(Event event);
    void viewEvents() const;
    void list() const;
    void inspect(int id) const;
    void runTurns(int number);
    void printMarket() const;
};
#endif
