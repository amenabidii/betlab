#include "World.h"
#include "Bettor.h"
#include "ConservativeBettor.h"
#include "AggressiveBettor.h"
#include "AnalyticalBettor.h"
#include "AdaptiveBettor.h"
#include <iostream>
#include <iomanip>
using namespace std;

World::World() { nextId = 1; turn = 0; market.makeOpportunity(1); }
void World::clearEntities() {
    for (int i = 0; i < (int)entities.size(); i++) delete entities[i];
    entities.clear();
}
void World::copyEntities(const World& other) {
    for (int i = 0; i < (int)other.entities.size(); i++)
        entities.push_back(other.entities[i]->clone());
}
World::World(const World& other) : pending(other.pending) {
    nextId = other.nextId; turn = other.turn; market = other.market;
    copyEntities(other);
}
World& World::operator=(const World& other) {
    if (this != &other) {
        // Clone first because we don't want to lose our old world if copying fails.
        vector<Entity*> copies;
        try {
            for (int i = 0; i < (int)other.entities.size(); i++) copies.push_back(other.entities[i]->clone());
            pending = other.pending;
        } catch (...) {
            for (int i = 0; i < (int)copies.size(); i++) delete copies[i];
            throw;
        }
        clearEntities();
        entities = copies; market = other.market;
        nextId = other.nextId; turn = other.turn;
    }
    return *this;
}
World::~World() { clearEntities(); }
int World::getTurn() const { return turn; }
int World::getNextId() const { return nextId; }
const Opportunity& World::getOpportunity() const { return market.getOpportunity(); }
Market& World::getMarket() { return market; }
Entity* World::find(int id) const {
    for (int i = 0; i < (int)entities.size(); i++)
        if (entities[i]->getId() == id) return entities[i];
    return nullptr;
}
bool World::add(int type, string name, double balance) {
    if (type < 1 || type > 4 || name.empty() || balance < 0 || balance > 1000000000) return false;
    Entity* created = nullptr;
    if (type == 1) created = new ConservativeBettor(nextId, name, balance);
    else if (type == 2) created = new AggressiveBettor(nextId, name, balance);
    else if (type == 3) created = new AnalyticalBettor(nextId, name, balance);
    else created = new AdaptiveBettor(nextId, name, balance);
    entities.push_back(created);
    cout << "Created " << name << " with ID " << nextId << "\n";
    nextId++;
    return true;
}
bool World::remove(int id) {
    for (int i = 0; i < (int)entities.size(); i++) {
        if (entities[i]->getId() == id) {
            delete entities[i];
            entities.erase(entities.begin() + i);
            cout << "Deleted ID " << id << "\n";
            return true;
        }
    }
    return false;
}
bool World::duplicate(int id) {
    Entity* source = find(id);
    if (source == nullptr) return false;
    Entity* copy = source->clone(); // This also copies the dynamic history independently.
    copy->setId(nextId);
    entities.push_back(copy);
    cout << "Duplicated ID " << id << " as ID " << nextId << "\n";
    nextId++; return true;
}
bool World::rename(int id, string name) {
    Entity* found = find(id);
    if (found == nullptr) return false;
    return found->setName(name);
}
bool World::changeBalance(int id, double amount) {
    Entity* entity = find(id);
    Bettor* bettor = dynamic_cast<Bettor*>(entity);
    if (bettor == nullptr) return false;
    // Balance editing is a management action; deposits go through the event queue.
    if (!bettor->setBalance(amount)) return false;
    bettor->setActive(amount > 0.01); return true;
}
bool World::requestBet(int id, double stake) {
    Entity* entity = find(id);
    Bettor* bettor = dynamic_cast<Bettor*>(entity);
    if (bettor == nullptr || !bettor->isActive() || stake <= 0 || stake > bettor->getBalance()) return false;
    pending.enqueue(Event(BET_REQUEST, id, stake, "Bet request"));
    cout << bettor->getName() << " requested " << fixed << setprecision(2) << stake << " credits.\n";
    return true;
}
bool World::addUserEvent(Event event) {
    if (event.type == DEPOSIT) {
        if (find(event.entityId) == nullptr || event.amount <= 0 || event.amount > 1000000) return false;
    } else if (event.type == MARKET_CHANGE) {
        if (event.amount < 0.10 || event.amount > 0.90) return false;
    } else return false;
    pending.enqueue(event); return true;
}
void World::viewEvents() const {
    if (pending.isEmpty()) { cout << "No pending events.\n"; return; }
    int number = 1;
    pending.forEach([&number](const Event& event) {
        cout << number++ << ". " << event.description << " | ID " << event.entityId
             << " | Amount " << event.amount << "\n";
    });
}
void World::processEvents() {
    // We process only the events that were pending at this point.
    int count = pending.size();
    for (int i = 0; i < count; i++) {
        Event event;
        if (!pending.dequeue(event)) break;
        if (event.type == DEPOSIT) {
            Bettor* b = dynamic_cast<Bettor*>(find(event.entityId));
            if (b != nullptr && b->deposit(event.amount)) cout << "Deposit applied to ID " << event.entityId << "\n";
            else cout << "Deposit skipped (invalid or deleted bettor).\n";
        } else if (event.type == MARKET_CHANGE) {
            if (market.changeCondition(event.amount)) cout << "Market condition updated.\n";
        } else if (event.type == BET_REQUEST) {
            // Normal bet requests are settled after all bettors have acted.
            pending.enqueue(event);
        }
    }
}
void World::runTurns(int number) {
    if (number < 1 || number > 100) return;
    for (int t = 0; t < number; t++) {
        turn++;
        cout << "\n========== TURN " << turn << " ==========\n";
        processEvents();
        market.makeOpportunity(turn);
        printMarket();
        // We don't check each bettor type here: virtual behavior chooses the strategy.
        for (int i = 0; i < (int)entities.size(); i++)
            if (entities[i]->isActive()) entities[i]->performTurn(*this);
        bool won = market.getOutcome();
        cout << "Outcome: " << (won ? "Team Alpha WON" : "Team Alpha LOST") << "\n";
        int count = pending.size();
        for (int i = 0; i < count; i++) {
            Event event;
            if (!pending.dequeue(event)) break;
            if (event.type == BET_REQUEST) {
                Bettor* bettor = dynamic_cast<Bettor*>(find(event.entityId));
                if (bettor != nullptr && event.amount <= bettor->getBalance()) {
                    bettor->settle(event.amount, market.getOpportunity().odds, won, turn);
                    cout << bettor->getName() << " bet settled. New balance: " << bettor->getBalance() << "\n";
                } else cout << "Bet request skipped because bettor is unavailable.\n";
            } else { // Any newly queued operator event waits until the next turn.
                pending.enqueue(event);
            }
        }
    }
}
void World::list() const {
    if (entities.empty()) cout << "No bettors yet.\n";
    for (int i = 0; i < (int)entities.size(); i++) entities[i]->print();
}
void World::inspect(int id) const {
    Entity* entity = find(id);
    if (entity == nullptr) { cout << "ID not found.\n"; return; }
    entity->print();
    Bettor* bettor = dynamic_cast<Bettor*>(entity);
    if (bettor == nullptr) return;
    cout << "History (" << bettor->getHistorySize() << " settled bets):\n";
    for (int i = 0; i < bettor->getHistorySize(); i++) {
        BetRecord bet = bettor->getRecord(i);
        cout << "Turn " << bet.getTurn() << " | Stake " << bet.getStake()
             << " | Odds " << bet.getOdds() << " | " << (bet.getWon() ? "Win" : "Loss")
             << " | Profit " << bet.getProfit() << "\n";
    }
}
void World::printMarket() const {
    const Opportunity& opp = market.getOpportunity();
    cout << opp.title << " | Odds: " << fixed << setprecision(2) << opp.odds
         << " | Published chance: " << opp.publicChance * 100 << "%\n";
}
