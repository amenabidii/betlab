#include "ConsoleApp.h"
#include "StatisticsReport.h"
#include "Bettor.h"
#include "Capabilities.h"
#include <iostream>
#include <sstream>
#include <cmath>
using namespace std;
ConsoleApp::ConsoleApp() {}
ConsoleApp::~ConsoleApp() {
    WorldSnapshot* old = nullptr;
    while (undoHistory.pop(old)) delete old; // stack deletes nodes; we delete the snapshots too
}
int ConsoleApp::readInt(string message, int minimum, int maximum) {
    while (true) {
        cout << message;
        string line; if (!getline(cin, line)) return minimum;
        stringstream input(line);
        int number; string extra;
        if (input >> number && !(input >> extra) && number >= minimum && number <= maximum) return number;
        cout << "Invalid input. Enter a number from " << minimum << " to " << maximum << ".\n";
    }
}
double ConsoleApp::readDouble(string message, double minimum, double maximum) {
    while (true) {
        cout << message;
        string line; if (!getline(cin, line)) return minimum;
        stringstream input(line);
        double number; string extra;
        if (input >> number && !(input >> extra) && isfinite(number) && number >= minimum && number <= maximum) return number;
        cout << "Invalid amount. Use a number from " << minimum << " to " << maximum << ".\n";
    }
}
string ConsoleApp::readName(string message) {
    while (true) {
        cout << message;
        string name;
        if (!getline(cin, name)) return "Unnamed";
        if (!name.empty() && name.find_first_not_of(' ') != string::npos && name.size() <= 60) return name;
        cout << "Name cannot be empty or more than 60 characters.\n";
    }
}
void ConsoleApp::saveUndo() { undoHistory.push(new WorldSnapshot(world)); }
void ConsoleApp::doUndo() {
    WorldSnapshot* previous = nullptr;
    if (!undoHistory.pop(previous)) { cout << "Nothing to undo.\n"; return; }
    previous->restore(world);
    delete previous;
    cout << "Last action undone.\n";
}
void ConsoleApp::createBettor() {
    cout << "1. Conservative  2. Aggressive  3. Analytical  4. Adaptive\n";
    int type = readInt("Choose type: ", 1, 4);
    string name = readName("Name: ");
    int defaults = readInt("Use default balance (1000)? 1.Yes 2.No: ", 1, 2);
    double money = defaults == 1 ? 1000 : readDouble("Starting balance: ", 0, 1000000000);
    saveUndo();
    world.add(type, name, money);
}
void ConsoleApp::modifyBettor() {
    int id = readInt("Entity ID: ", 1, 1000000000);
    Entity* e = world.find(id);
    if (e == nullptr) { cout << "ID not found.\n"; return; }
    int choice = readInt("1. Change name  2. Change balance  3. Change active status: ", 1, 3);
    if (choice == 1) {
        string name = readName("New name: ");
        saveUndo(); world.rename(id, name);
    } else if (choice == 2) {
        double amount = readDouble("New balance: ", 0, 1000000000);
        saveUndo(); world.changeBalance(id, amount);
    } else {
        int on = readInt("1. Active  2. Inactive: ", 1, 2);
        saveUndo(); e->setActive(on == 1);
    }
}
void ConsoleApp::queueEvent() {
    cout << "1. Deposit  2. Change market condition\n";
    int type = readInt("Choose event: ", 1, 2);
    if (type == 1) {
        int id = readInt("Bettor ID: ", 1, 1000000000);
        if (world.find(id) == nullptr) { cout << "Invalid ID.\n"; return; }
        double amount = readDouble("Deposit amount: ", 0.01, 1000000);
        saveUndo();
        world.addUserEvent(Event(DEPOSIT, id, amount, "Deposit"));
    } else {
        double chance = readDouble("New true probability (0.10 to 0.90): ", 0.10, 0.90);
        saveUndo();
        world.addUserEvent(Event(MARKET_CHANGE, 0, chance, "Market condition change"));
    }
    cout << "Queued for next turn.\n";
}
void ConsoleApp::specialAction() {
    cout << "1. Probability analysis  2. Adapt strategy\n";
    int choice = readInt("Choose capability: ", 1, 2);
    int id = readInt("Select ANY entity ID: ", 1, 1000000000);
    Entity* selected = world.find(id);
    if (selected == nullptr) { cout << "ID not found.\n"; return; }
    if (choice == 1) {
        ProbabilityAnalysis* analysis = dynamic_cast<ProbabilityAnalysis*>(selected);
        if (analysis == nullptr) cout << "This bettor cannot do advanced analysis.\n";
        else analysis->analyze(world.getOpportunity());
    } else {
        StrategyAdaptation* strategy = dynamic_cast<StrategyAdaptation*>(selected);
        if (strategy == nullptr) cout << "This bettor cannot adapt its strategy.\n";
        else { saveUndo(); strategy->adapt(); }
    }
}
void ConsoleApp::showMenu() const {
    cout << "\n=========== BETLAB ===========\n"
         << "1. Add bettor\n2. List bettors\n3. Inspect bettor\n"
         << "4. Modify bettor\n5. Duplicate bettor\n6. Delete bettor\n"
         << "7. Run one turn\n8. Run several turns\n9. Add event\n"
         << "10. View pending events\n11. Special capabilities\n"
         << "12. Statistics report\n13. Undo\n14. View market\n0. Exit\n";
}
void ConsoleApp::run() {
    cout << "BETLAB - A fictional betting market simulation using virtual credits.\n";
    while (true) {
        showMenu();
        int choice = readInt("Choice: ", 0, 14);
        if (!cin) { cout << "Input closed.\n"; return; }
        if (choice == 0) return;
        if (choice == 1) createBettor();
        else if (choice == 2) world.list();
        else if (choice == 3) world.inspect(readInt("ID: ", 1, 1000000000));
        else if (choice == 4) modifyBettor();
        else if (choice == 5 || choice == 6) {
            int id = readInt("ID: ", 1, 1000000000);
            if (world.find(id) == nullptr) { cout << "ID not found.\n"; continue; }
            saveUndo();
            if (choice == 5) world.duplicate(id);
            else world.remove(id);
        } else if (choice == 7 || choice == 8) {
            int turns = choice == 7 ? 1 : readInt("How many turns (1-100): ", 1, 100);
            saveUndo(); // snapshot for the whole run, so one Undo cancels that command
            world.runTurns(turns);
        } else if (choice == 9) queueEvent();
        else if (choice == 10) world.viewEvents();
        else if (choice == 11) specialAction();
        else if (choice == 12) StatisticsReport::show(world);
        else if (choice == 13) doUndo();
        else if (choice == 14) world.printMarket();
    }
}
