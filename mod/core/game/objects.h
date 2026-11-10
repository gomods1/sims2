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
    bool broken = false;         // what the game's repairman looks for (object variable 15)
    int dirt = 0;                // object variable 39: from addr::objvar::kDirtyFrom a Sim would clean it
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
// A Sim's current wants (or fears) by name, as the game words them ("Hacer ejercicios con
// máquina"), with their aspiration points, from the want simulator: the panel need not be open,
// nor the Sim selected.
// A Sim's inventory as its panel shows it: one item per object kind, with how many.
struct InventoryItem {
    std::string name;
    uint32_t guid = 0;
    int token = -1;  // the index of the stack's last token in the inventory
    int count = 1;
};
bool SimInventory(int objectId, std::vector<InventoryItem>* out);
struct Wish {
    std::string name;
    int points = 0;
};
bool SimWishes(int objectId, bool fears, std::vector<Wish>* out);
// A Sim's name by neighbor id (the neighborhood's Sims, on the lot or not); last may be null.
bool NeighborName(int neighborId, std::string* first, std::string* last);

// What a Sim looks like, as the game's outfit data says (the Lua scripts' getOutfitProperty): the
// values of game/addresses.h looks (0 when unknown), its age stage and the outfit it wears.
struct SimLooks {
    bool human = true;
    int ageDays = 0;
    bool youngAdult = false;
    int gender = 0;      // 0 male, 1 female
    int bodyFlags = 0;   // bit 0 fat, bit 4 fit
    int outfit = 0;      // person data 8: 0 everyday, 2 swimwear, 4 formal, 5 sleepwear...
    int hair = 0, skin = 0, eyes = 0;
    bool facialHair = false, glasses = false, makeup = false, fullMakeup = false, hat = false, jewelry = false;
};
bool SimLook(int objectId, SimLooks* out);

// The object an action queue icon's action is done with (its id), 0 when unknown.
int QueueIconObject(Ptr icon);

// The view: its zoom step (0 the farthest) of `steps`, and its compass direction 0 north, 1
// north-east ... 7 north-west: what the top of the screen looks toward. False when no lot's camera
// is found.
bool CameraView(int* zoom, int* steps, int* compass);
// The lot camera looks at a point of the lot (tiles; z keeps the camera's own height): at once, or
// gliding there as the game's Return does for the selected Sim. false when the camera is unknown.
bool CenterCamera(float x, float y, bool jump);

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
    bool walkThrough = false;  // a Sim can walk over it or through it (a rug, a door)
};
std::vector<PlacedPart> PlacedParts();
// Where Sims stand or sit to use an object: its routing slots (and its parts'), as the game places
// them (a bone of the model, or the SLOT's offset turned by the object's facing), in tiles of the
// lot. Empty when it has none (a computer: it is used from its desk's) or they cannot be read.
struct UseSpot {
    float x = 0, y = 0;
    int tileX = 0, tileY = 0;
    bool sitting = false;
};
std::vector<UseSpot> UseSpots(int objectId);
int ContainerOf(int objectId);  // the object it stands on (a computer's desk), 0 none
int ObjdField(int objectId, int index);  // a u16 field of the whole object's OBJD, -1 unknown
// The tiles a whole object covers (its own and its parts'), from PlacedParts.
std::vector<std::pair<int, int>> ObjectTiles(int id, const std::vector<PlacedPart>& parts);

// The world database service (nTSWorld::cWorldDB, its cIWorldDB interface at +0x1c): rooms,
// walls, floors, terrain of the lot (game/lotgrid.h).
Ptr WorldDB();

// The room at a spot of the lot (tiles, floor 0 = ground), -1 when unknown.
int RoomAt(float x, float y, int level);
// The light at a spot as the game scores it for the Room need (its light map: windows, lamps,
// daylight), about 0 dark to 1 bright; false when unknown. LightThresholds: below dark the game
// counts it dark, from lit fully lit (its tuning, 0.55 and 0.9 by default).
bool LightAt(float x, float y, int level, float* light);
void LightThresholds(float* dark, float* lit);

// The game is paused (the Pause command's state); false also when no lot is loaded.
bool GamePaused();

// Opens the interaction menu of an object for the selected Sim, as a click on it does: posts the
// message the live mode tool sends to the main toolbar (type 0xC0, data 1 = the object id).
bool ShowInteractions(int objectId);
// A Sim's person data (Lua's GetPersonData; the indices in addresses.h person::), false for no Sim.
bool PersonDatum(int objectId, int index, int* value);
// An object's attribute (its own variables, named by its STR# 0x100), or with semi its semi
// attribute (named by its semi-global's STR# 0x100); false when it has no such.
bool ObjectAttribute(int objectId, int index, int* value, bool semi = false);
// Posts a message to the game's message server (cRZMessage2Standard: type, data 1), as the
// accelerators do for their commands (accelerators.txt: "Rotate object clockwise" 0xacb3a629 +1).
bool PostGameMessage(uint32_t type, uint32_t data1);

}  // namespace game
