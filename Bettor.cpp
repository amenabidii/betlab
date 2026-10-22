#include "Bettor.h"
#include "World.h"
#include <iomanip>

Bettor::Bettor() : Entity() {
    balance = 1000; totalDeposits = 0;
    historySize = 0; historyCapacity = 2;
    history = new BetRecord[historyCapacity];
}
Bettor::Bettor(int id, string name, double balance) : Entity(id, name) {
    this->balance = balance >= 0 ? balance : 1000;
    totalDeposits = 0; historySize = 0; historyCapacity = 2;
    history = new BetRecord[historyCapacity];
}
void Bettor::copyHistory(const Bettor& other) {
    historyCapacity = other.historyCapacity;
    historySize = other.historySize;
    history = new BetRecord[historyCapacity];
    for (int i = 0; i < historySize; i++) history[i] = other.history[i];
}
Bettor::Bettor(const Bettor& other) : Entity(other) {
    balance = other.balance;
    totalDeposits = other.totalDeposits;
    copyHistory(other); // new memory, so changing a copy cannot change the original
}
Bettor& Bettor::operator=(const Bettor& other) {
    if (this != &other) { // needed in case someone assigns an object to itself
        BetRecord* newHistory = new BetRecord[other.historyCapacity];
        for (int i = 0; i < other.historySize; i++) newHistory[i] = other.history[i];
        Entity::operator=(other);
        delete[] history;
        history = newHistory;
        historyCapacity = other.historyCapacity;
        historySize = other.historySize;
        balance = other.balance;
        totalDeposits = other.totalDeposits;
    }
    return *this;
}
Bettor::~Bettor() { delete[] history; }
double Bettor::getBalance() const { return balance; }
double Bettor::getTotalDeposits() const { return totalDeposits; }
int Bettor::getHistorySize() const { return historySize; }
BetRecord Bettor::getRecord(int index) const {
    if (index < 0 || index >= historySize) return BetRecord();
    return history[index];
}
bool Bettor::setBalance(double value) {
    if (value < 0 || value > 1000000000) return false;
    balance = value; return true;
}
bool Bettor::deposit(double amount) {
    if (amount <= 0 || amount > 1000000 || balance + amount > 1000000000) return false;
    balance += amount;
    totalDeposits += amount;
    if (balance > 0) setActive(true);
    return true;
}
bool Bettor::placeBet(World& world, double stake) {
    if (!isActive() || stake <= 0 || stake > balance) return false;
    return world.requestBet(getId(), stake);
}
void Bettor::settle(double stake, double odds, bool won, int turn) {
    // Money is deducted only once the request is accepted by the world.
    if (stake <= 0 || stake > balance) return;
    if (historySize == historyCapacity) {
        historyCapacity *= 2;
        BetRecord* biggerHistory = new BetRecord[historyCapacity];
        for (int i = 0; i < historySize; i++) biggerHistory[i] = history[i];
        delete[] history;
        history = biggerHistory;
    }
    balance -= stake;
    if (won) balance += stake * odds;
    history[historySize++] = BetRecord(turn, stake, odds, won);
    if (balance < 0.01) setActive(false);
}
void Bettor::print() const {
    cout << "ID " << getId() << " | " << getName() << " | " << getType()
         << " | Balance " << fixed << setprecision(2) << balance
         << " | Bets " << historySize
         << " | " << (isActive() ? "Active" : "Inactive") << "\n";
}
