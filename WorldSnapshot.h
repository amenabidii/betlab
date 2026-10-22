#ifndef WORLDSNAPSHOT_H
#define WORLDSNAPSHOT_H
#include "World.h"
// Keeps a separate copy of the whole world so Undo restores all histories/events too.
class WorldSnapshot {
private:
    World savedWorld;
public:
    // I pass by reference so making a snapshot does not copy the world twice.
    WorldSnapshot(const World& world);
    void restore(World& world) const;
};
#endif
