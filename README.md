# BETLAB — Turn-Based Betting Market Simulator

A C++17 console simulation using **virtual credits only**. Create four bettor strategies, run turns, queue events, duplicate bettors, undo changes, and compare strategy statistics.

## Build

From this folder (with MinGW g++ or a compatible compiler):

```sh
g++ -std=c++17 -Wall -Wextra *.cpp -o betlab
```

Run `./betlab` on Linux/macOS or `betlab.exe` on Windows.

**Visual Studio:** Add the `.cpp` and `.h` files to a C++ Console App project, enable C++17, and build. Don't add a second `main.cpp`.

## Notes

- The simulation does not use real money or real market data.
- Four strategies see the same fictional opportunity each turn.
- The hidden probability is used to generate one common outcome, while strategies see an imperfect public estimate.
- A user's deposit or market-change event waits in the queue until the next turn.
- `Undo` restores the last world-changing menu command, using independent deep copies. Running several turns is one command.
- Entity IDs are unique among live entities. Undo restores saved ID state.
- Each bettor owns a dynamically allocated bet history and copies it independently.
- Metrics include historical bets copied during duplication (as required by F3).
- `main.cpp` contains the design/feature map and editable AI usage disclosure. Replace the team placeholders before submission.

## Testing ideas

1. Create each bettor type both with default and custom balances.
2. Run a turn and inspect differing decisions and updated histories.
3. Duplicate a bettor, modify its balance, then verify the original remains unchanged.
4. Delete a bettor and Undo; inspect its original history and ID.
5. Add deposit and market-change events, inspect pending events, advance one turn.
6. Try special abilities on supported and unsupported bettor types.
7. Undo several consecutive operations, including a batch of turns.
8. Enter letters, negative amounts, missing IDs and out-of-range values.

