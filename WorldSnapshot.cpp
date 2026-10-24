#include "WorldSnapshot.h"

// Saving and restoring the world for Undo.
WorldSnapshot::WorldSnapshot(const World& world) : savedWorld(world) {
}
void WorldSnapshot::restore(World& world) const {
    world = savedWorld;
}
