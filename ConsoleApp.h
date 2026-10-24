#ifndef CONSOLEAPP_H
#define CONSOLEAPP_H
#include "World.h"
#include "CustomStack.h"
#include "WorldSnapshot.h"
#include <string>
using namespace std;

// Console menu and input handling. The simulation itself stays inside World.

// The console handles input and menu commands, while World handles simulation rules.
class ConsoleApp {
private:
    World world;
    CustomStack<WorldSnapshot*> undoHistory;
    int readInt(string message, int minimum, int maximum);
    double readDouble(string message, double minimum, double maximum);
    string readName(string message);
    void saveUndo();
    void doUndo();
    void createBettor();
    void modifyBettor();
    void queueEvent();
    void specialAction();
    void showMenu() const;
public:
    ConsoleApp();
    ~ConsoleApp();
    void run();
};
#endif
