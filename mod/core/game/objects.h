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
    std::string catalogName;  // UTF-8, the name the catalog shows (empty for objects outside it)
};

// Every object of the lot that is a whole object (multi-tile parts are left out).
std::vector<LotObject> LotObjects();
bool ReadLotObject(int id, LotObject* out);
// The catalog's name of an object definition (in the game's language), empty when none.
std::string CatalogName(uint32_t guid);
// The Sim the player controls now, 0 when none.
Ptr SelectedSim();
int SelectedSimId();

// Motive n (0..15) of a Sim (object id), -100..100; false when it is no Sim.
bool SimMotive(int objectId, int index, float* value);

// The game is paused (the Pause command's state); false also when no lot is loaded.
bool GamePaused();

// Opens the interaction menu of an object for the selected Sim, as a click on it does: posts the
// message the live mode tool sends to the main toolbar (type 0xC0, data 1 = the object id).
bool ShowInteractions(int objectId);

}  // namespace game
