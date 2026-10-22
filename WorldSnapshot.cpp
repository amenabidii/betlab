#include "WorldSnapshot.h"
WorldSnapshot::WorldSnapshot(const World& world) : savedWorld(world) {}
void WorldSnapshot::restore(World& world) const { world = savedWorld; }
