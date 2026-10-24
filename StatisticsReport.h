#ifndef STATISTICSREPORT_H
#define STATISTICSREPORT_H
#include "World.h"

// Functions used to show how the bettors performed.
// Reports are calculated from histories instead of maintaining duplicate totals.
class StatisticsReport {
public:
    static void show(const World& world);
};
#endif
