// Sound cues for the cursors (the build cursor, placing's tile cursor, the live-mode map): besides
// speech, a short sound whenever the cursor's tile changes, so the space is heard as it is crossed
// (ported from the Sims 1 mod, where a tester asked for them). SpeechHost plays them
// (Sims2Access\sounds\*.wav, made with genny from mod\sounds\batch.json), mixed beside the screen
// reader. On a step:
//   at once       the floor: grass or ground, wood, tile, carpet, stone, gravel, the pool's edge,
//                 water, no floor upstairs (the floor's catalog definition, game/lotgrid.h); a
//                 room change
//   then          each side with a wall, fence, railing, door or window: west panned left, east
//                 right, north higher, south lower
//   last          stairs, a Sim or an object on the tile, a slope
// Ctrl+Shift+S turns them off and on (Sims2Access.ini [mod] cues=1, cue_volume=80 in percent).
#pragma once

#include <string>

#include "../game/lotgrid.h"
#include "../util/strings.h"

namespace nav::cues {

enum class Edge { None, Wall, Fence, Railing, Door, Window };  // a railing: a half wall, or a fence upstairs
enum Side { kNorth, kEast, kSouth, kWest, kSides };

struct TileSense {
    game::Surface surface = game::Surface::Ground;
    Edge sides[kSides] = {};
    bool stairs = false;
    bool object = false;  // furniture or another listed object (doors and windows are sides)
    bool person = false;  // a Sim or a pet
    bool sloped = false;
    bool offLot = false;
    int room = -1;
};

// One read of the tile (tiles, level as game::LotObject::level). `ignore`: an object id left out
// (the one being placed stands on the tile it is tried on, with its parts).
TileSense Sense(int level, int x, int y, int ignore = 0);

bool Enabled();
void Toggle();  // Ctrl+Shift+S
void Reset();   // forgets the last room (a new cursor starts)

// The tile cursors: the floor, the four sides, what stands there, the room when it changed.
void Tile(int level, int x, int y, int ignore = 0);
// The wall cursor's corner (x, y is the corner north-west of tile x, y): the walls leaving it,
// the floor south-east of it.
void Corner(int level, int x, int y);
void LotEdge();       // the cursor could not move further
void Blocked();       // a wall, fence or window stopped the cursor
void Fit(bool fits);  // placing: the held object fits here or not

// Words for speech.
std::string SurfaceWord(game::Surface s);
Str SideWord(int side);                     // north, east, south, west
std::string EdgesLine(const TileSense& t);  // "pared al norte, puerta al este"; empty when none
// The walls, fences, doors and windows on the four grid lines leaving a corner (the wall tools).
std::string CornerEdgesLine(int level, int x, int y);
// A step of one tile (dx, dy: -1, 0 or 1, not both) from the tile: whether a wall, fence, railing
// or window on that side stops it (a door does not); `what` gets its words ("pared al este").
// The map's and placing's arrows stop there (a tester of the Sims 1 mod left rooms by accident);
// Alt with the arrows goes through.
bool WallStops(int level, int x, int y, int dx, int dy, std::string* what);

}  // namespace nav::cues
