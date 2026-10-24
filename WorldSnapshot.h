#ifndef WORLDSNAPSHOT_H
#define WORLDSNAPSHOT_H
#include "World.h"

// A saved copy of the world for Undo. It must keep its own state instead of sharing the old objects.
// Keeps a separate copy of my whole world so the Undo restore all histories andevents aswell
class WorldSnapshot {
private:
    World savedWorld;
public:
    // I pass by reference so making a snapshot does not copy the world twice.
    WorldSnapshot(const World& world);
    void restore(World& world) const;
};
#endif
