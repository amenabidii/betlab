# BETLAB — Betting Market Simulator

BETLAB is a C++ console application that simulates a fictional basketball
betting market using virtual money.

The user acts as the market operator: creating bettors, managing their
attributes, running matches, and comparing how different betting
strategies perform over time.

I built this project for my CS2 coursework at the American University
in Cairo and developed its match presentation and statistics features
to make the simulation more engaging.

## Features

- Create, inspect, modify, duplicate, and delete bettors.
- Simulate individual matches or multiple matches.
- Display fictional teams, venues, betting odds, scores, and results.
- Show each bettor's selection, stake, potential payout, and settlement.
- Maintain an individual betting history for every bettor.
- Queue deposits and market condition changes.
- Use special probability-analysis and strategy-adaptation capabilities.
- Compare bettor performance through a statistics report.
- Undo recent changes using saved world snapshots.
- Validate console input and handle invalid selections.

## Betting Strategies

| Strategy | Behaviour |
|----------|-----------|
| Conservative | Uses smaller stakes and prefers opportunities that meet its confidence threshold. |
| Aggressive | Uses larger stakes and selects the team offering higher odds. |
| Analytical | Uses an imperfect probability estimate to evaluate expected returns. |
| Adaptive | Adjusts its stake percentage based on previous results. |

All bettors participate in the same simulated matches. Their decisions
differ, but their bets are settled against the same match outcome.

## How a Simulation Works

1. Pending deposits and market changes are processed.
2. The market generates a fictional basketball match and its odds.
3. Each active bettor decides whether and how to bet.
4. Bet requests enter the custom event queue.
5. The match outcome and a corresponding final score are generated.
6. Bets are settled, balances change, and histories are updated.

Short pauses make individual matches easier to follow. Longer runs skip
these pauses.

## C++ Concepts Used

- **Inheritance:** Entity → Bettor → four concrete bettor types.
- **Polymorphism:** Each bettor implements its own turn behaviour.
- **Encapsulation:** Bettor state is managed through class operations.
- **Dynamic memory:** Betting histories use dynamically allocated arrays.
- **Deep copying:** Duplicated bettors have independent histories.
- **Rule of Three:** Resource-owning classes manage copying, assignment,
  and destruction.
- **Templates:** Custom stack and queue implementations.
- **Linked nodes:** FIFO event processing and LIFO undo history.
- **Runtime type checking:** Checked dynamic_cast operations for
  special capabilities.

## Main Components

| Component | Responsibility |
|-----------|----------------|
| ConsoleApp | Menus, user input, commands, and undo history |
| World | Entity management and simulation coordination |
| Entity | Shared identity and virtual behaviour |
| Bettor | Balance, betting history, and shared betting operations |
| Concrete bettor classes | Individual betting strategies |
| Market | Match generation, odds, outcomes, and scores |
| Opportunity | Information about the current match |
| BetRecord | Information about one completed bet |
| Event | A pending market operation |
| CustomQueue | Processes events in arrival order |
| CustomStack | Stores undo snapshots in reverse chronological order |
| WorldSnapshot | Saves and restores world state |
| StatisticsReport | Displays bettor performance |

## Build and Run

### Visual Studio

1. Clone or download this repository.
2. Open betlab.vcxproj in Visual Studio with C++ development tools installed.
3. Select Debug and x64.
4. Build the project.
5. Run with Ctrl + F5.

The included project currently targets the v145 platform toolset.
If your installation uses another toolset, select an installed compatible
toolset in the project properties.

### GCC

From the folder containing the source files:

    g++ -std=c++17 -Wall -Wextra *.cpp -o betlab

Run:

    ./betlab

Compile only one implementation of each class. Obsolete duplicate
source files should not be included.

## Try It

1. Create one bettor of each strategy with the same starting balance.
2. Simulate one match to follow their decisions and results.
3. Run several matches and view the statistics report.
4. Inspect an individual bettor's history.
5. Duplicate or delete a bettor, then try undo.

## What I Learned

The most challenging part was making the different systems work together:
bettor decisions, queued events, match results, dynamic histories, and undo.

Deep copying was especially important. Duplicating a bettor or saving a
world snapshot needed separate memory so that later changes would not
affect the original state.

The project also helped me understand how polymorphism allows different
strategies to run through the same simulation loop.

## Limitations

BETLAB uses fictional matches and a simplified probability model.
It does not use live sports data or real money. Results illustrate
behaviour within this simulation and do not establish that a strategy
will succeed in real-world betting.

## Author

Mohamed Amen Allah Abidi  
Data Science student, Computer Science minor  
The American University in Cairo