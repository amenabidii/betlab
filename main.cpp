/*
BETLAB - CSCE 1101 SimWorld mini project
Team members: Mohamed Amen Allah Abidi 900244027

World: an imaginary betting market where four strategies compete using virtual credits.
Each turn creates one opportunity. Bettors make decisions automatically, their requests
enter a custom FIFO queue, and all accepted bets share one outcome.

Inheritance in the project:
Entity (common identity, virtual actions)
  -> Bettor (balance + owned history)
       -> ConservativeBettor / AggressiveBettor / AnalyticalBettor / AdaptiveBettor
This separates what all bettors share from the different strategies.
*/

#include "ConsoleApp.h"
#include <cstdlib>
#include <ctime>
int main() {
    srand((unsigned int)time(nullptr));
    ConsoleApp app;
    app.run();
    return 0;
}



