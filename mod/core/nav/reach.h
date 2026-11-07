// Where Sims can walk on a floor of the lot, and so which things they can get to (ported from the
// Sims 1 mod's tester feedback, 2026-11-06: a tester's Sim never reached the chair he had placed,
// and nothing told him). A flood from the lot's entrance (its edge on the ground, the stairs on an
// upper floor) over the tiles a Sim can stand on: floor (any tile on the ground), no furniture that
// blocks, through no wall, fence or railing except at a door. Measured from the entrance, not from
// where Sims stand (in the Sims 1 mod a Sim asleep in a corner made a chair that cut that corner
// off look harmless).
#pragma once

#include <string>
#include <utility>
#include <vector>

namespace nav::reach {

// Whether Sims can get to a thing to use it: 1 yes, 2 no (the floor around it is cut off), 0 unknown
// (nothing beside it a Sim could stand on: all taken by furniture, or it has no floor).
int CanUse(int objectId);
// The same, kept for a few seconds per lot (the lot's lists ask for every thing they show).
bool Unreachable(int objectId);
void Forget();  // the lot changed (something placed): read it again

// Whether Sims can use a door standing on this tile (front tile, facing `direction`): 0 usable; 1
// something stands in its way (`blocker` its name); 2 no way to it from the lot's entrance.
int CheckDoor(int x, int y, int direction, int level, std::string* blocker);

}  // namespace nav::reach
