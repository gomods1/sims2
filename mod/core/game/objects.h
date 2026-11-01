// The lot's objects and Sims (the object module, cEdithObjectModule) and the interaction menu
// for any of them without a click in the 3D view.
#pragma once

#include <string>
#include <vector>

#include "memory.h"

namespace game {

struct LotObject {
    int id = 0;            // the object id (SimAntics), what the interaction menu takes
    Ptr ptr = 0;
    std::string name;      // UTF-8, the game's name for it (as Lua's GetObjectName)
    float x = 0, y = 0;    // tiles
    int level = 0;         // floor, 0 = ground
    bool person = false;   // a Sim (cTSPerson)
    uint32_t guid = 0;     // its definition's GUID
    int price = 0;
    uint16_t functionFlags = 0;  // buy mode categories (game/addresses.h objd::kFunctionFlags)
    uint16_t roomFlags = 0;      // buy mode rooms (objd::kRoomFlags)
    int room = -1;               // the room the object stands in (RoomAt), -1 unknown
    int direction = 0;           // 0 faces -x (west), 2 +y (south), 4 +x (east), 6 -y (north)
    int type = 0;                // OBJD object type: 4 normal, 8 door, 9 window
    std::string catalogName;  // UTF-8, the name the catalog shows (empty for objects outside it)
};

// Every object of the lot, a multi-tile one once (as its lead part, the "- 0,0" one).
std::vector<LotObject> LotObjects();
bool ReadLotObject(int id, LotObject* out);
// The catalog's name of an object definition (in the game's language), empty when none.
std::string CatalogName(uint32_t guid);
// The object ids of the Sims of the lot's family.
std::vector<int> FamilySimIds();
// The Sim the player controls now, 0 when none.
Ptr SelectedSim();
int SelectedSimId();

// Motive n (0..15) of a Sim (object id), -100..100; false when it is no Sim.
bool SimMotive(int objectId, int index, float* value);

// A Sim's first and last name (UTF-8); false when it is no Sim.
bool SimName(int objectId, std::string* first, std::string* last);
// A Sim's name by neighbor id (the neighborhood's Sims, on the lot or not); last may be null.
bool NeighborName(int neighborId, std::string* first, std::string* last);

// The doors (type 8) and windows (type 9) of the lot, by their lead part: the tile in front of
// the wall (the part faces its direction from there; the tile behind is one step back).
struct WallObject {
    int type = 0;
    float x = 0, y = 0;  // the lead part's tile centre
    int level = 0;
    int direction = 0;   // 0 faces -x, 2 +y, 4 +x, 6 -y
    std::string name;    // UTF-8 catalog name
};
std::vector<WallObject> WallObjects();

// Every tile something of the lot stands on: each object, and each part of a multi-tile object
// (cEdithMTObject) with the lead part it belongs to (`lead`: its id; the lead part is what
// LotObjects lists for the whole object).
struct PlacedPart {
    int id = 0;          // the part's own object id
    int lead = 0;        // the whole object's id (the part's own id for a whole object)
    int tileX = 0, tileY = 0, level = 0;
    bool person = false;
    int type = 0;        // OBJD type (8 door, 9 window)
    int direction = 0;   // 0 faces -x (west), 2 +y (south), 4 +x (east), 6 -y (north)
    int sub = 0;         // the part's place in its object, (column << 8) | row; row 0 stands in front
    bool stairs = false; // a piece of a staircase ("Stair..." by its internal name)
};
std::vector<PlacedPart> PlacedParts();
// The tiles a whole object covers (its own and its parts'), from PlacedParts.
std::vector<std::pair<int, int>> ObjectTiles(int id, const std::vector<PlacedPart>& parts);

// The world database service (nTSWorld::cWorldDB, its cIWorldDB interface at +0x1c): rooms,
// walls, floors, terrain of the lot (game/lotgrid.h).
Ptr WorldDB();

// The room at a spot of the lot (tiles, floor 0 = ground), -1 when unknown.
int RoomAt(float x, float y, int level);

// The game is paused (the Pause command's state); false also when no lot is loaded.
bool GamePaused();

// Opens the interaction menu of an object for the selected Sim, as a click on it does: posts the
// message the live mode tool sends to the main toolbar (type 0xC0, data 1 = the object id).
bool ShowInteractions(int objectId);

}  // namespace game
