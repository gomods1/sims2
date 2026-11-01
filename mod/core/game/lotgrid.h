// The lot's build data, read from the world database (nTSWorld::cWorldDB, game::WorldDB(); layouts
// in STRUCTS.md "The lot grid"): floors and what they are made of, walls, fences, water, terrain
// heights. Tiles are the lot's (x east, y south); `level` is game::LotObject::level (0 the
// terrain, 1 a foundation's floor or the floor above the ground).
#pragma once

#include <cstdint>
#include <string>

namespace game {

// What a floor is made of, as its catalog definition says (XFLR "surfacetype": cpet, wood,
// wdeck, marble, lino, cment, gravel, grass) or, for the lot's built-in floors, its name.
enum class Surface { Ground, Grass, Wood, Tile, Carpet, Stone, Gravel, PoolRim, Water, NoFloor };
Surface SurfaceAt(int level, int x, int y);
int FloorPatternAt(int level, int x, int y);  // the floor pattern id there, 0 none
std::string FloorPatternName(int id);         // its name: a catalog GUID in hex, or "sidewalk"...

// A wall of a kind between two neighbouring tiles' centres (the game's IsWallBetweenPoints):
// the partition layer 0 holds every wall, fence and railing, layer 4 the load-bearing walls.
bool WallBetween(int level, int x0, int y0, int x1, int y1, int layer = 0);
bool WaterOnTile(int x, int y);           // a pool or pond (the ground floor)
bool TerrainHeight(int x, int y, float* out);  // at a tile's north-west corner
bool LotSize(int* width, int* depth);      // the lot's tiles (the world's own extent)

}  // namespace game
