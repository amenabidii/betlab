#include "World.h"
#include "Bettor.h"
#include "ConservativeBettor.h"
#include "AggressiveBettor.h"
#include "AnalyticalBettor.h"
#include "AdaptiveBettor.h"
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
using namespace std;

// Main simulation logic: bettor actions, match display, results, settlements, and changes to the world.

World::World() {
    nextId = 1;
    turn = 0;
    market.makeOpportunity(1);
}
void World::clearEntities() {
    for (int i = 0; i < (int)entities.size(); i++) delete entities[i];
    entities.clear();
}
void World::copyEntities(const World& other) {
    for (int i = 0; i < (int)other.entities.size(); i++)
    entities.push_back(other.entities[i]->clone());
}
World::World(const World& other) : pending(other.pending) {
    nextId = other.nextId;
    turn = other.turn;
    market = other.market;
    copyEntities(other);
}
World& World::operator=(const World& other) {
    if (this != &other) {
        // Clone first because we don't want to lose our old world if copying fails.
        vector<Entity*> copies;
        try {
            for (int i = 0; i < (int)other.entities.size(); i++) copies.push_back(other.entities[i]->clone());
            pending = other.pending;
        }
        catch (...) {
            for (int i = 0; i < (int)copies.size(); i++) delete copies[i];
            throw;
        }
        clearEntities();
        entities = copies;
        market = other.market;
        nextId = other.nextId;
        turn = other.turn;
    }
    return *this;
}

// all entity pointers belong to World so they are deleted when the world is destroyed
World::~World() {
    clearEntities();
}
int World::getTurn() const {
    return turn;
}
int World::getNextId() const {
    return nextId;
}
const Opportunity& World::getOpportunity() const {
    return market.getOpportunity();
}
Market& World::getMarket() {
    return market;
}
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
    
    cout << "\033[32mCreated " << name << " with ID " << nextId << "\033[0m\n";
    nextId++;
    return true;
}
bool World::remove(int id) {
    for (int i = 0; i < (int)entities.size(); i++) {
        if (entities[i]->getId() == id) {
            delete entities[i];
            entities.erase(entities.begin() + i);
            cout << "\033[32mDeleted ID " << id << "\033[0m\n";
            return true;
        }
    }
    cout << "\033[31mID not found.\033[0m\n";
    return false;
}
bool World::duplicate(int id) {
    Entity* source = find(id);
    if (source == nullptr) {
        cout << "\033[31mID not found.\033[0m\n";
        return false;
    }
    Entity* copy = source->clone();
    // This also copies the dynamic history independently.
    copy->setId(nextId);
    entities.push_back(copy);
    cout << "\033[32mDuplicated ID " << id << " as ID " << nextId << "\033[0m\n";
    nextId++;
    return true;
}
bool World::rename(int id, string name) {
    Entity* found = find(id);
    if (found == nullptr) {
        cout << "\033[31mID not found.\033[0m\n";
        return false;
    }
    return found->setName(name);
}
bool World::changeBalance(int id, double amount) {
    Entity* entity = find(id);
    Bettor* bettor = dynamic_cast<Bettor*>(entity);
    
    if (bettor == nullptr) {
        cout << "\033[31mID not found.\033[0m\n";
        return false;
    }
    // Balance editing is a management action; deposits go through the event queue.
    
    if (!bettor->setBalance(amount)) {
        cout << "\033[31mInvalid balance amount.\033[0m\n";
        return false;
    }
    bettor->setActive(amount > 0.01);
    cout << "\033[32mBalance updated for ID " << id << "\033[0m\n";
    return true;
}
bool World::requestBet(int id, double stake, bool homeSelection) {
    Entity* entity = find(id);
    Bettor* bettor = dynamic_cast<Bettor*>(entity);
    
    if (bettor == nullptr || !bettor->isActive() || stake <= 0 || stake > bettor->getBalance()) {
        cout << "\033[31mInvalid bet request.\033[0m\n";
        return false;
    }
    
    pending.enqueue(Event(BET_REQUEST, id, stake, "Bet request", homeSelection));
    
    const Opportunity& match = market.getOpportunity();
    
    double odds = homeSelection ? match.homeOdds : match.awayOdds;
    
    cout << "\n\033[36m" << bettor->getName() << " [" << bettor->getType() << "]\033[0m\n"
    << "Selection: " << (homeSelection ? match.homeTeam : match.awayTeam) << "\n"
    << "Stake: " << stake << "\n"
    << "Odds: " << odds << "\n";
    return true;
}
bool World::addUserEvent(Event event) {
    if (event.type == DEPOSIT) {
        if (find(event.entityId) == nullptr || event.amount <= 0 || event.amount > 1000000) return false;
    } else if (event.type == MARKET_CHANGE) {
        if (event.amount < 0.10 || event.amount > 0.90) return false;
    } else return false;
    pending.enqueue(event);
    return true;
}
void World::viewEvents() const {
    if (pending.isEmpty()) {
        cout << "No pending events.\n";
        return;
    }
    int number = 1;
    pending.forEach([&number](const Event& event) {
        cout << number++ << ". " << event.description << " | ID " << event.entityId
        << " | Amount " << event.amount << "\n";
    }
    );
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

    if (number < 1 || number > 100) {
        return;
    }

    for (int t = 0; t < number; t++) {

        turn++;

        cout << "\n========== TURN " << turn << " ==========" << endl;

        processEvents();

        market.makeOpportunity(turn);
        printMarket();

        cout << "\n---------- BETTOR DECISIONS ----------" << endl;

        // Virtual performTurn lets each bettor use its own strategy.
        for (int i = 0; i < (int)entities.size(); i++) {

            if (entities[i]->isActive()) {
                entities[i]->performTurn(*this);
            }
        }

        bool homeWon = market.getOutcome();

        market.makeFinalScore(homeWon);

        const Opportunity& match = market.getOpportunity();

        // I added 2 seconds pauses so the match does not finish immediately when printing
        // The pauses also work when simulating several matches together
        // endl makes sure the text appears before the program waits

        cout << "\n========== MATCH STARTED ==========" << endl;

        this_thread::sleep_for(chrono::seconds(2));

        cout << "First quarter underway..." << endl;

        this_thread::sleep_for(chrono::seconds(2));

        cout << "Half-time..." << endl;

        this_thread::sleep_for(chrono::seconds(2));

        cout << "Final quarter..." << endl;

        this_thread::sleep_for(chrono::seconds(2));

        cout << "\n=========== FINAL RESULT ==========" << endl;

        cout << match.homeTeam << " " << match.homeScore << " - "
            << match.awayScore << " " << match.awayTeam << endl;

        cout << "Winner: "
            << (homeWon ? match.homeTeam : match.awayTeam) << endl;

        cout << "\n---------- BET SETTLEMENT ----------" << endl;

        int count = pending.size();

        for (int i = 0; i < count; i++) {

            Event event;

            if (!pending.dequeue(event)) {
                break;
            }

            if (event.type == BET_REQUEST) {

                Bettor* bettor = dynamic_cast<Bettor*>(find(event.entityId));

                if (bettor != nullptr &&
                    event.amount > 0 &&
                    event.amount <= bettor->getBalance()) {

                    bool betWon = (event.homeSelection == homeWon);

                    double odds = event.homeSelection
                        ? match.homeOdds
                        : match.awayOdds;

                    string selectedTeam = event.homeSelection
                        ? match.homeTeam
                        : match.awayTeam;

                    bettor->settle(
                        event.amount,
                        odds,
                        betWon,
                        turn,
                        selectedTeam
                    );

                    double payout = betWon
                        ? event.amount * odds
                        : 0;

                    double profit = betWon
                        ? event.amount * (odds - 1)
                        : -event.amount;

                    cout << bettor->getName() << ": "
                        << (betWon ? "WON" : "LOST")
                        << " | " << selectedTeam << endl;

                    cout << "Payout: "
                        << fixed << setprecision(2) << payout
                        << " credits | Net profit: "
                        << showpos << profit << noshowpos
                        << " | Balance: "
                        << bettor->getBalance() << endl << endl;
                }
                else {
                    cout << "Bet skipped (bettor not found or insufficient balance)." << endl;
                }
            }
            else {
                pending.enqueue(event);
            }
        }

        cout << "===================================" << endl;

        // Wait before starting the next match or returning to the menu.
        this_thread::sleep_for(chrono::seconds(2));
    }
}
void World::list() const {
    if (entities.empty()) cout << "No bettors yet.\n";
    for (int i = 0; i < (int)entities.size(); i++) entities[i]->print();
}
void World::inspect(int id) const {
    Entity* entity = find(id);
    if (entity == nullptr) {
        cout << "ID not found.\n";
        return;
    }
    entity->print();
    Bettor* bettor = dynamic_cast<Bettor*>(entity);
    if (bettor == nullptr) return;
    cout << "History (" << bettor->getHistorySize() << " settled bets):\n";
    for (int i = 0; i < bettor->getHistorySize(); i++) {
        BetRecord bet = bettor->getRecord(i);
        cout << "Turn " << bet.getTurnNumber() << " | Team " << bet.getSelectedTeam()
        << " | Stake " << bet.getAmountBet()
        << " | Odds " << bet.getOdds() << " | " << (bet.getWon() ? "Win" : "Loss")
        << " | Profit " << bet.getNetProfit() << "\n";
    }
}
void World::printMarket() const {
    const Opportunity& match = market.getOpportunity();
    cout << "\n \033[1;36m========== UPCOMING MATCH ==========\033[0m\n"
    << "Competition: " << match.competition << "\n"
    << "Match:       " << match.homeTeam << " vs " << match.awayTeam << "\n"
    << "Venue:       " << match.venue << "\n"
    << "Format:      " << match.format << "\n"
    << "\n \033[1;36m---------- BETTING ODDS -----------\033[0m\n"
    << fixed << setprecision(2)
    << match.homeTeam << " to win: " << match.homeOdds << "\n"
    << match.awayTeam << " to win: " << match.awayOdds << "\n"
    << "Published home chance: " << match.publicChance * 100 << "%\n";
}
