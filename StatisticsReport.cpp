#include "StatisticsReport.h"
#include "Bettor.h"
#include <iostream>
#include <iomanip>
using namespace std;

// Totals and performance numbers calculated from completed bets.
void StatisticsReport::show(const World& world) {
    cout << "\n=== BETTING STATISTICS ===\n";
    bool any = false;
    for (int id = 1; id < world.getNextId(); id++) {
        Bettor* bettor = dynamic_cast<Bettor*>(world.find(id));
        if (bettor == nullptr) continue;
        any = true;
        double totalStake = 0, totalProfit = 0, worstLoss = 0;
        int wins = 0, count = bettor->getHistorySize();
        for (int i = 0; i < count; i++) {
            BetRecord record = bettor->getRecord(i);
            totalStake += record.getStake();
            totalProfit += record.getProfit();
            if (record.getWon()) wins++;
            if (record.getProfit() < worstLoss) worstLoss = record.getProfit();
        }
        cout << bettor->getName() << " (" << bettor->getType() << ") | Bets " << count
        << " | Profit " << fixed << setprecision(2) << totalProfit;
        if (count > 0 && totalStake > 0) {
            cout << " | Win rate " << 100.0 * wins / count << "%"
            << " | ROI " << 100.0 * totalProfit / totalStake << "%"
            << " | Avg stake " << totalStake / count
            << " | Biggest loss " << worstLoss;
        } else cout << " | Win rate N/A | ROI N/A";
        cout << "\n";
    }
    if (!any) cout << "No bettors to report.\n";
    cout << "Note: copied bettors keep their earlier betting history. Deposits are not betting profit.\n";
}
