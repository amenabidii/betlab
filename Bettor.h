#ifndef BETTOR_H
#define BETTOR_H
#include "Entity.h"
#include "BetRecord.h"
class World;

// Shared bettor details go here so the four strategies do not repeat them.
// Each bettor owns a separate dynamic array. This is why copying needs care.
class Bettor : public Entity {
private:
    double balance;
    double totalDeposits;
    BetRecord* history;
    int historySize;
    int historyCapacity;
    void copyHistory(const Bettor& other);
protected:
    bool placeBet(World& world, double stake);
public:
    Bettor();
    Bettor(int id, string name, double balance);
    // const reference means we read the original, not change it or copy it again.
    Bettor(const Bettor& other);
    Bettor& operator=(const Bettor& other);
    virtual ~Bettor();
    double getBalance() const;
    double getTotalDeposits() const;
    int getHistorySize() const;
    BetRecord getRecord(int index) const;
    bool setBalance(double newBalance);
    bool deposit(double amount);
    void settle(double stake, double odds, bool won, int turn);
    void print() const override;
};
#endif
