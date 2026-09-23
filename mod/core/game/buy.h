// Buy mode: the catalog's products as the thumbnails hold them, and the object placement tool
// (cTSUserToolObject) driven without the mouse: while the mod places, the tools' pick of the
// ground (cTSUserToolBase slot 31) answers with the mod's spot, and the tool's verdict on that
// spot (slot 86: fits or not) is recorded.
#pragma once

#include <cstdint>
#include <string>

#include "memory.h"

namespace game {

constexpr int kRatingCount = 7;  // hunger, comfort, hygiene, bladder, energy, fun, room

struct Product {
    std::string name;         // UTF-8, the catalog name
    std::string description;  // UTF-8
    std::string internalName;
    int price = 0;
    uint32_t guid = 0;
    int ratings[kRatingCount] = {};  // what the hover popup shows ("Comodidad: 2")
};

// The product a catalog thumbnail window (proc cTSWinProcCatalogThumbnail) shows.
bool ReadProduct(Ptr thumbnail, Product* out);

void InitBuyHooks();  // from InitHooks (MinHook ready)

// While set, the tools' object pick (what is under the cursor) answers with this lot object's
// scene node: a click then acts on it (buy mode's hand picks it up). False when the object has
// no node (found by scanning memory once, then cached).
bool ForceObjectPick(Ptr object);
void ClearObjectPick();

// The main toolbar's current user tool (the object tool while a bought object is held), 0 if none.
Ptr CurrentTool();
// The class of the toolbar's current UI mode ("cTSUIModeBuy"...), empty when unknown.
std::string CurrentModeClass();

// While on, every ground pick of a tool returns this spot (tiles; level 1 = ground floor, as
// game::LotObject::level).
void SetPickOverride(bool on, float x = 0, float y = 0, int level = 0);
// The tool that last picked while the override was on (0 when none yet).
Ptr PickTool();
// The object tool's last verdict: 1 fits, 0 does not, -1 none yet. The counter grows with each.
int PlacementVerdict(uint32_t* counter = nullptr);

struct HeldObject {
    bool holding = false;  // an object is on the cursor now
    Ptr object = 0;        // it (cTSObject)
    uint32_t guid = 0;     // the last held object's GUID (kept after it is put down)
    int direction = 0;   // 0, 2, 4, 6 (the tool's rotation, wrapped)
    float x = 0, y = 0;  // the snapped spot (tile centres)
    int level = 0;       // 1 = ground floor
    bool overLot = false;
};
// The object placement tool's state; false when the tool is no object tool.
bool ReadHeldObject(Ptr tool, HeldObject* out);

// Moves the held object to a spot right away (the tool's own mouse-move handling, with the pick
// answering this spot) and returns the tool's verdict: 1 fits, 0 not, -1 no verdict (the tool
// only judges a spot that differs from the last one; the caller keeps the last verdict then),
// -2 off the lot.
int CheckSpot(Ptr tool, float x, float y, int level);

}  // namespace game
