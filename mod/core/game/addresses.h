// Every address and offset the mod uses in Sims2EP9.exe 1.18.0.187 (SHA-256 24041f83..., The Sims 2
// Legacy Collection, Steam). Preferred-base VAs (image base 0xA0000000); game::Va() relocates them.
// Evidence for each is in PATCH_TARGETS.md and STRUCTS.md.
#pragma once

#include <cstdint>

namespace game::addr {

// The GZ framework singleton (cCTGFramework, static in .data). Slot 5 is
// GetSystemService(serviceId, iid, void** out), thiscall, ret 0xc (0xa01428e2).
constexpr uintptr_t kFramework = 0xa10fdb50;
constexpr int kFrameworkGetSystemService = 5;

// The window manager service (cGZWinMgrW32): service id at wm+0x10, QueryInterface accepts 0x5a4.
constexpr uint32_t kWinMgrServiceId = 0xa417445e;
constexpr uint32_t kWinMgrIid = 0x5a4;

namespace wm {
constexpr int kCapture = 0x34;     // GZGetCapture (slot 29)
constexpr int kFocus = 0x38;       // GZGetFocus (slot 37): the window keys go to
constexpr int kModal = 0x3c;       // the innermost modal window (DoModalWin saves and restores it); 0 when none
constexpr int kMouseWin = 0x40;    // the window under the mouse (slot 25)
constexpr int kMainWindow = 0x44;  // GetMainWindow (slot 3)
constexpr int kModalCount = 0x190; // GetModalNestCount (slot 44)
constexpr int kSlotDoModal = 42;   // DoModalWin(cIGZWin*), thiscall (0xa0972f48)
// GZSetFocus(cIGZWin*), thiscall (0xa09734cb): keys go to that window (+0x38), what a click on a
// text field ends in; the caret stays where it was.
constexpr int kSlotSetFocus = 38;
}  // namespace wm

// cGZWin, through the cIGZWin interface pointer (the one child lists hold). Fields:
namespace win {
constexpr int kManager = 0x04;
constexpr int kId = 0x10;
constexpr int kChildren = 0x54;    // -> container; [container] = std::list head; node: next, prev, cIGZWin*
constexpr int kParent = 0x58;
constexpr int kCaption = 0x60;     // cRZString: vtable, then std::string (UTF-8) at +0x64
constexpr int kArea = 0xc8;        // l, t, r, b in the parent's coordinates
constexpr int kFlags = 0xe8;       // 1 visible, 2 enabled
constexpr int kProc = 0x12c;       // the window proc (a cTSWinProc*), for windows that have one
constexpr uint32_t kFlagVisible = 1;
constexpr uint32_t kFlagEnabled = 2;
// Virtual slots (cIGZWin, matching SimCity 4's gzcom headers from GetWindowManager on; verified
// by disassembly of cGZWin's vtable 0xa0e52f54).
constexpr int kSlotMouseDownL = 155;  // (x, y, flags): window coordinates
constexpr int kSlotMouseUpL = 157;    // (x, y, flags); a button notifies its click here
constexpr int kSlotMouseEnter = 161;
constexpr int kSlotMouseExit = 162;
constexpr int kSlotKeyDown = 151;     // (vk, modifiers)
}  // namespace win

// The cheat system service (cTSCheatSystem), as the console gets it (0xa01d0979); slot 7 of that
// interface runs a line (const char*) and returns bool (0xa0cbe460).
namespace cheat {
constexpr uint32_t kServiceId = 0x2a4915ea;
constexpr uint32_t kIid = 0x4a490e00;
constexpr int kSlotExecute = 7;
}  // namespace cheat

// cTSWinProcGradientFillBar: a meter (needs, aspiration). Value and range, ints.
namespace fillbar {
constexpr int kValue = 0x70;
constexpr int kMin = 0x74;
constexpr int kMax = 0x78;
}  // namespace fillbar

// The simulator's interface (cTSSimulator +4) cached at [0xa10cc2ac] (getter 0xa020df9a): +0x3e
// a word, non-zero while the game is paused (slot 26; the Pause command toggles it); slot 15 reads
// the SimAntics globals (words at +0x1c: 0 hour, 5 minute...).
namespace sim {
constexpr uintptr_t kInstance = 0xa10cc2ac;
constexpr int kPaused = 0x3e;
constexpr int kGlobals = 0x1c;
}  // namespace sim

// cTSGlobals: the game's hub of managers ([0xa10cc288], created by 0xa020ddfb).
namespace globals {
constexpr uintptr_t kInstance = 0xa10cc288;
constexpr int kSimulator = 0x20;     // cTSSimulator (interface +4)
constexpr int kLot = 0x2c;           // cTSLot
constexpr int kNeighborhood = 0x30;  // cTSNeighborhood (the current one)
constexpr int kFamilyManager = 0x60; // cTSFamilyManager
constexpr int kNeighborManager = 0x64;
constexpr int kObjectModule = 0x78;  // cEdithObjectModule
}  // namespace globals

// cTSNeighborManager (globals +0x64).
namespace neighbors {
constexpr int kSlotFind = 16;  // thiscall (neighbor id, 1) -> the cTSNeighbor
constexpr int kSelector = 0x14;  // cTSNeighbor -> cTSObjectSelector (person::kNeighborSlot*Name)
}  // namespace neighbors

// cTSNeighborhood: its lots are a std::map<int lotId, cTSLotInfo* (interface +8)> (MSVC tree:
// node left 0, parent 4, right 8, isnil byte 0xd, key 0x10, value 0x14); head node at +0x5c.
namespace nhood {
constexpr int kLotMap = 0x5c;
}  // namespace nhood

// cTSLotInfo (from the object start): position on the neighborhood grid, its base info.
namespace lot {
constexpr int kInterface = 8;        // what the neighborhood's map holds
constexpr int kX = 0x18;
constexpr int kY = 0x1c;
constexpr int kBase = 0x80;          // -> cTSBaseLotInfo (interface +8)
}  // namespace lot

// cTSBaseLotInfo (from the object start).
namespace baselot {
constexpr int kInterface = 8;
constexpr int kWidth = 0x18;         // lot size in 10-tile units (hypothesis)
constexpr int kDepth = 0x1c;
constexpr int kType = 0x20;          // 0 residential, 1 community, 8 apartment building, 9 apartment
                                     // unit (the getter 0xa04c9c20; order as changeLotZoning's list)
constexpr int kName = 0x28;          // cRZString (std::string at +0x2c)
constexpr int kDescription = 0x48;   // cRZString (std::string at +0x4c)
constexpr int kFlags = 0x68;         // 0x1000/0x2000/0x4000 lot class low/middle/high (hypothesis)
}  // namespace baselot

// cTSFamilyManager: std::vector<cTSFamily*> at +0x14.
namespace familymgr {
constexpr int kBegin = 0x14;
constexpr int kEnd = 0x18;
}  // namespace familymgr

// cTSFamily.
namespace family {
constexpr int kName = 0x14;          // cRZString (std::string at +0x18)
constexpr int kDescription = 0x34;   // cRZString (std::string at +0x38)
constexpr int kId = 0x54;
constexpr int kLot = 0x58;           // home lot id, 0 when not moved in
constexpr int kFunds = 0x70;
constexpr int kMembersBegin = 0x7c;  // 4 bytes per member
constexpr int kMembersEnd = 0x80;
}  // namespace family

// cEdithObjectModule (globals +0x78): the lot's objects.
namespace objmodule {
constexpr int kById = 0x1a24;          // std::vector<cTSObject*>, index = object id - 1 (slot 29,
                                       // 0xa04281f0); multi-tile parts are cEdithMTObject
constexpr int kSlotSelectedSim = 39;
constexpr int kPeople = 0x1a70;        // std::vector of every Sim on the lot (cTSPerson interface +752)
constexpr int kFamily = 0x1a7c;        // std::vector of the family's Sims, same interface (on a community
                                       // lot, the visiting family's: verified with townies around)   // the selected cTSPerson (the pie builder 0xa0621f50 uses it)
}  // namespace objmodule

// cTSObject virtual slots (as Lua's GetObjectName 0xa0566100 and GetObjectLocation 0xa0566030).
namespace object {
// A part of a multi-tile object (cEdithMTObject: a table's "- 1,0", a window's "- 0,1") points at
// its lead part's interface at +0x2f0 (the "- 0,0" part, itself one of the parts), and the parts
// chain through +0x2f8. A multi-tile object is its parts only: no whole object stands for it
// (verified on a dining table and a 2-tile window, 2026-11-01).
constexpr int kLeadPart = 0x2f4;
constexpr int kLeadInterface = 0x2f0;
constexpr int kSelector = 0x240;        // cTSObjectSelector of the object
constexpr int kSelectorMaster = 0x68;   // selector: a multi-tile part's master selector, whose OBJD
                                        // (+0x60, product::kSelectorObjd) is the whole object's
constexpr int kSlotName = 92;          // const char* name
// Attributes (SimAntics "My attribute"; Lua's GetObjectAttribute 0xa0573b60 calls slot 15
// 0xa052f0b0): int16 array at +0x88, its count (int16) at +0x8c. Semi attributes (the ones a
// semi-global's STR# 0x100 names; GetObjectSemiAttribute 0xa0573e60, slot 17 0xa0530a20): +0x90,
// count +0x94. A pet food dish's semi attribute 0 is its food units (PetFoodDishGlobals: "Food
// Units", "Eat Loops Played", "Current Dirty State", "Snapping?"; read live: 0, then 200 once
// Carlos had filled it).
constexpr int kAttributes = 0x88, kAttributeCount = 0x8c, kSemiAttributes = 0x90, kSemiAttributeCount = 0x94;
constexpr int kSlotLocation = 33;      // -> {int x, int y}, 16.16 fixed point, in tiles
constexpr int kSlotLevel = 36;         // short: 0 the terrain, 1 the ground floor of a house on a
                                       // foundation (a patio set outside reads 0, the rooms 1)
// Routing slots (where a Sim stands or sits to use the object; found by disassembly 2026-11-07,
// STRUCTS.md "Routing slots"): slot 250 int count (0xa0533fe0), slot 251 cITSRoutingSlot* (int i)
// (0xa05309d0); the slot's object is the interface - 0x20. Slot 81 int (slot object, bool) the
// positions it has (a bone slot names several bones, 0xa05432f0); slot 83 float3* (float3* out,
// slot object, int index, float radius) its offset from the object (0xa0545cf0: a bone's place in
// the model, else the SLOT's x, y). The interface: +4 standing, +8 sitting, +0x10 flags (bits 0-7 the
// sides, bit 9 absolute: the object's facing is not applied), +0x20 the optimal proximity (tiles).
constexpr int kSlotRoutingCount = 250, kSlotRoutingSlot = 251, kSlotSlotPositions = 81, kSlotSlotOffset = 83;
constexpr uintptr_t kRoutingCount = 0xa0533fe0, kRoutingSlot = 0xa05309d0, kSlotPositions = 0xa05432f0,
                    kSlotOffset = 0xa0545cf0;
constexpr int kContainer = 0xaa;       // short: the object id it stands on (the desk of a computer), 0 none
constexpr int kDirection = 0xa8;       // short, SimAntics direction: 0 faces -x, 2 +y, 4 +x, 6 -y (a
                                       // dining set: each chair faces its table); the object tool's
                                       // rotation is the same number for the held object
constexpr int kDefinition = 0x238;     // -> cTSPersistResourceT<cEdithObjectDefinition> (OBJD)
}  // namespace object

// SimAntics object variables: shorts from +0xa6 (variable 1 is object::kDirection, 11 the object's
// id: read live on every object). The global trees say what two of them mean: "Repair - X is
// Broken?" (0x207) tests 15 > 0; "Dirty - Set To Dirty" (0x265) and "Set To Clean" (0x2e0) write 39,
// and "Clean - Interaction TEST" (0x125) offers cleaning from G - Cleaning Tuning's 200 up.
namespace objvar {
constexpr int kBase = 0xa6;
constexpr int kBroken = 15;
constexpr int kDirt = 39;
constexpr int kDirtyFrom = 200;
// Variable 8, the flags: any of these and a Sim walks over or through it (a rug 0x4, a door 0x2490,
// a staircase 0x90; a chair 0, a table 0x100).
constexpr int kFlags = 8;
constexpr int kWalkThrough = 0x1c;
}  // namespace objvar

// The object definition (OBJD, parsed), from the resource above.
namespace objd {
constexpr int kGuid = 0x20;
constexpr int kPrice = 0x28;
constexpr int kType = 0x16;            // u16 object type (OBJD field 9): 4 normal, 8 door, 9 window
                                       // (a door's parts stand on both sides of its wall: the lead
                                       // part in front, facing its direction, the other behind)
constexpr int kMasterId = 0x18;        // u16 multi-tile group (OBJD field 10): its parts and master share it, 0 one tile
constexpr int kSubIndex = 0x1a;        // u16 multi-tile part (OBJD field 11): (column << 8) | row, 0 the lead
constexpr int kRoomFlags = 0x52;       // u16 buy mode rooms: 1 kitchen, 2 bedroom, 4 bathroom, 8 living,
                                       // 0x10 outside, 0x20 dining, 0x40 misc, 0x80 study, 0x100 kids
constexpr int kRatings = 0xa8;         // s16 [7]: hunger, comfort, hygiene, bladder, energy, fun, room
                                       // (OBJD fields 82..88; the data's u16 fields start at +4)
constexpr int kFunctionFlags = 0x54;   // u16 buy mode categories: 1 comfort, 2 surfaces, 4 appliances,
                                       // 8 electronics, 0x10 plumbing, 0x20 decorative, 0x40 misc,
                                       // 0x80 lighting, 0x100 hobbies (checked on known objects)
constexpr uint16_t kFnComfort = 0x1, kFnSurfaces = 0x2, kFnAppliances = 0x4, kFnElectronics = 0x8,
                   kFnMiscellaneous = 0x40;
// OBJD field 94: the buy catalog's sub-sort within the function. Surfaces (read live 2026-11-07 on the
// whole objects' OBJDs): 1 counter, 2 dining table, 8 desk, 16 coffee table.
constexpr int kFieldFunctionSubsort = 94;
constexpr uint16_t kSurfaceCounter = 1, kSurfaceTable = 2, kSurfaceDesk = 8, kSurfaceCoffeeTable = 16;
}  // namespace objd

// Buy mode catalog: each cTSWinProcCatalogThumbnail (a thumbnail window's proc, win::kProc) holds
// the cTSProductObject it shows; the product's strings are cRZStrings (vtable, then std::string).
namespace product {
constexpr int kThumbnailProduct = 0x14;  // thumbnail proc -> product interface
constexpr int kInterfaceOffset = 0x10c;  // that interface inside a cTSProductObject (other products:
                                         // other offsets; the mod finds the object through RTTI)
constexpr int kInternalName = 0x10;      // std::string ("Chair - Dining - Patio Plastic")
constexpr int kName = 0x30;              // std::string, the catalog name in the game's language
constexpr int kDescription = 0x50;       // std::string
constexpr int kPrice = 0xac;             // int
constexpr int kSelector = 0xfc;          // cTSObjectSelector
constexpr int kSelectorObjd = 0x60;      // selector -> the OBJD resource (objd:: offsets apply)
constexpr int kThumbnailButton = 0x10;   // thumbnail proc -> its cGZWinBtn interface (the click)
}  // namespace product

// Light (found 2026-11-10 by static analysis; STRUCTS.md "Light"): the scene graph system
// (nTSSG::cTSSGSystem, [0xa10cc458], also cached at [0xa10cc29c], both filled lazily) keeps the
// lighting manager (nTSSG::cLightingManager, vtable 0xa0db5e90) at +0xa4. Its slot 34 (0xa02ded50,
// thiscall float (const Spot* {float x, y; int level}), the level as RoomAt's) is the light at a
// spot from that floor's light map, 0.30 R + 0.59 G + 0.11 B: what the environment score
// (cTSEnvironmentScoreManager slot 8, 0xa045a930) reads at a Sim for the Room need. That manager
// ([cTSGlobals +0x50]) keeps its tuning at +0x14 (lighting's share of the score, 0.2), +0x18 (the
// light counted as dark, 0.55) and +0x1c (fully lit, 0.9).
namespace light {
constexpr uintptr_t kSceneGraph = 0xa10cc458, kSceneGraphCached = 0xa10cc29c;
constexpr int kLightingManager = 0xa4;
constexpr uintptr_t kLightingManagerVtable = 0xa0db5e90;
constexpr int kSlotLightAt = 34;
constexpr int kGlobalsEnvironment = 0x50;
constexpr int kEnvDark = 0x18, kEnvLit = 0x1c;
}  // namespace light

// The Sim panel's rewards catalogs (cTSWinProcAspirationRewardsCatalog, cTSWinProcCareerRewardsCatalog:
// the same layout, found 2026-11-10 by static analysis of 0xa066a618, which fills the list from
// cTSGlobals +0x40's products whose OBJD function flags have 0x400, and 0xa066def7, which sets up a
// cell). The catalog proc keeps a std::vector of product interfaces (as product::kThumbnailProduct
// holds) at +0x164; each cell's proc (cTSWinProc*RewardThumbnail) keeps its item's index into it,
// pages included. An aspiration reward's price is in aspiration points: affordable when it is not
// above the Sim's points (person data 161 x 10).
namespace reward {
constexpr int kCatalogItems = 0x164;   // catalog proc: begin, end
constexpr int kThumbnailIndex = 0x20;  // cell proc: int index into kCatalogItems
}  // namespace reward

// The main toolbar (cTSMainToolbar), a system service: the current UI mode and the current user tool.
namespace toolbar {
constexpr uint32_t kServiceId = 0xe92db716;  // (kept by the service at +0xc)
constexpr uint32_t kIidUnknown = 1;          // cIGZUnknown: the service object itself
constexpr int kMode = 0x74;                  // cTSUIModeLive / cTSUIModeBuy / cTSUIModeBuild ...
constexpr int kToolHandler = 0x90;           // cTSUserToolHandler, its interface at +8
constexpr int kHandlerInterface = 8;
constexpr int kHandlerTool = 0x28;           // the handler's current tool (cTSUserTool*)
}  // namespace toolbar

// User tools (cTSUserToolBase and subclasses: the object placement tool cTSUserToolObject, walls,
// floors...). Mouse moves reach the tool's handler (0xa078eb62), which picks the ground under the
// cursor through slot 31 and keeps the spot; the object tool then moves the held object there,
// checks it (slot 71) and tints it (slot 86: fits or not).
namespace tool {
constexpr uint32_t kPick = 0xa078d6c3;          // cTSUserToolBase slot 31: thiscall (sx, sy, float out[3], flag)
                                            // -> bool; out = {x, y, level (1 = ground)} in tiles
constexpr uint32_t kPickAtLevel = 0xa078d6f0;    // slot 32: thiscall (sx, sy, int level, float out[3]) -> bool;
                                                 // used instead of slot 31 once a pick is stored
constexpr uint32_t kObjectVerdict = 0xa067f31a;  // cTSUserToolObject slot 86: thiscall (bool fits)
constexpr int kSlotDispatch = 8;             // thiscall (int buttons, int message, int pos[2], bool handled):
                                             // the message dispatcher 0xa078dc91 (buttons: 1 left,
                                             // checked against the tool's +0x20 on a down)
constexpr uint32_t kObjectPick = 0xa078d789;     // cTSUserToolBase slot 29: thiscall (sx, sy, 1, a, b)
                                                 // -> the scene node under the cursor (the node's
                                                 // 135-slot interface, see scenenode::), or 0
constexpr uint32_t kMouseLeave = 0xa078ec6d;     // cTSUserToolBase slot 48: thiscall (); tool message 6,
                                                 // the cursor left the 3D view: clears kOverLot
constexpr int kOverLot = 0x50;              // u8: the last pick is on the lot (slots 59/60)
constexpr int kPicked = 0x54;               // float x, y, int level: the last pick
constexpr int kSnapped = 0x80;              // float x, y, int level: where the held object stands
constexpr int kHeldGuid = 0xa4;             // the last held object's GUID (kept after it is put down)
constexpr uint32_t kIsHolding = 0xa067be89;  // cTSUserToolObject: thiscall () -> bool, an object is held
constexpr uint32_t kHeldObject = 0xa067cb69; // cTSUserToolObject: thiscall () -> the held cTSObject or 0
// Why the held object does not fit (found 2026-11-10 by static analysis, STRUCTS.md "Buy mode"):
// the judge (slot 71, 0xa06815d7) empties and refills a std::set of placement errors at +0x118
// (head node; +0x11c the count) on each check. A node's value at +0x10: int code, float pos[3],
// cTSObject*, int kind (0 the game's own text, UIText STR# 0x89; 1 the object's own; 2 the list of
// places it may go). The first node (the head's left) is the one the game shows on a click.
// 0xa07f0c00: thiscall bool (value*, cIGZString** out) gives its text, as the game shows it.
constexpr int kErrorSet = 0x118;
constexpr int kErrorCount = 0x11c;
constexpr int kErrorValue = 0x10;
constexpr uint32_t kErrorText = 0xa07f0c00;
// kRotation is not the object's facing (read live 2026-11-10 on a chair and a sofa: the game's
// , and . turned rotation 0, 2, 4, 6 into the object's direction variable 2, 0, 6, 4, and three
// sofas put down at rotation 4, 2, 2 faced 6, 0, 0): the object faces (2 - rotation) mod 8. The
// held object's own direction variable follows only the game's turning keys, not a rotation the
// mod writes (it read 0 at rotations 4 and 2 after the mod's turns), so the mod uses the formula.
constexpr int kRotation = 0xa0;             // int, the held object's direction: steps of 2 (8 = a turn),
                                            // not wrapped (the game's , and . add -2 / +2)
constexpr int kSlotMouseMessage = 42;       // thiscall (int message, int pos[2] screen): 3 = move; runs
                                            // the pick (slot 31) and keeps it
constexpr int kSlotMouseMove = 45;          // thiscall (int pos[2]): after 42 for a move; the object
                                            // tool moves the held object to the pick and judges it
}  // namespace tool

// Neighborhood tools (cTSUserToolNeighborhoodMoveIn: a family chosen in the family bin, waiting
// for a click on a lot). Their dispatcher (slot 8, 0xa079a1d5: thiscall (0, message, int pos[2]))
// takes 3 move / 4 down / 5 up; on a down the move-in tool asks slot 24 for the lot under the cursor.
namespace nhoodtool {
constexpr uint32_t kLotAtScreen = 0xa079a5a4;  // slot 24: thiscall (int pos[2], int* lot, cIGZString* name)
                                               // -> bool; a ray against the scene's "LotData" nodes
constexpr int kSlotDispatch = 8;
constexpr int kChosenFamily = 0x7c;            // the move-in tool: the family chosen in the bin, 0 none
}  // namespace nhoodtool

// Scene graph nodes (nTSSG::cObject and subclasses): what the tools' object pick returns.
namespace scenenode {
constexpr int kObject = 0x88;  // -> the lot object (cTSObject / cTSPerson, the object module's entry)
// The interface the pick returns sits at a class-dependent offset (re/classes.tsv, 135 slots).
constexpr struct {
    const char* cls;
    int offset;
} kPickInterface[] = {{"nTSSG::cPerson", 912}, {"nTSSG::cLocomotable", 860},
                      {"nTSSG::cAnimatable", 432}, {"nTSSG::cObject", 324}};
}  // namespace scenenode

// Rooms, as Lua's GetRoomAtLocation (0xa05673c0): the world service (framework GetSystemService
// 0x4a037ee3 / iid 0x49ff7d6f, cached by the game at [0xa10cc45c]); its slot 39 gives the room
// manager, whose slot 80 takes ({float x, float y, int level}, 2, 1) and returns the room id (short).
namespace world {
constexpr uint32_t kServiceId = 0x4a037ee3;
constexpr uint32_t kIid = 0x49ff7d6f;
constexpr int kSlotRoomManager = 39;   // [iface + 0x70]: the nTSWorld::cPartitionManager
constexpr int kRoomManagerSlotRoomAt = 80;
}  // namespace world

// The lot's build data in the world database (nTSWorld::cWorldDB; the service hands out its
// cIWorldDB interface at +0x1c). Found from the Lua bindings' table at 0xa1016700 (name, function
// pairs: IsWallBetweenPoints 0xa0569840, IsLoadWallBetweenPoints 0xa0563740, GetTerrainElevationForXY
// 0xa0568bf0, IsWaterOnTile 0xa056be30,). Verified on the Contreras lot
// (2026-11-01): see STRUCTS.md "The lot grid".
namespace grid {
constexpr int kInterface = 0x1c;         // cIWorldDB inside cWorldDB
constexpr int kFloorPatterns = 0x94;     // cWorldDB: c3DArray<cFloorPatternStorageTile>*: +8 x count,
                                         // +0xc y count, +0x10 levels, +0x14 data; [level+1][x][y],
                                         // 4 u16 pattern ids a tile (its quarters; 0 none)
constexpr int kPatternNames = 0xc0;      // cWorldDB: cStringMap<u16>*: pattern id <-> name
constexpr int kPatternById = 0x18;       //   its std::map<u16, std::string*> (head pointer)
constexpr int kPartitionSlotLayer = 40;  // cPartitionManager: thiscall (int layer) -> the walls of
                                         // a layer: 0 every wall, fence, railing; 4 load-bearing
constexpr int kLayerSlotBetween = 19;    // layer: thiscall (Point* a, Point* b, 0, 0, 0) -> bool,
                                         // Point {float x, y; int level}
constexpr int kSlotElevation = 64;       // cIWorldDB: thiscall (int x, int y) -> float, the
                                         // terrain height at a corner
constexpr uint32_t kWaterOnTile = 0xa07e2400;  // cdecl bool (int tile[3] {x, y, 0}): pool or pond water
}  // namespace grid

// The catalog's names by GUID, as Lua's GetCatalogName (0xa0561640): cTSGlobals slot 24 gives the
// object folder, its slot 14 the entry for a GUID, the entry's slot 48 fills a cIGZString* with
// the catalog name (in the game's language); the string's slot 5 is its text.
namespace catalog {
constexpr int kGlobalsSlotFolder = 24;
constexpr int kFolderSlotByGuid = 14;
constexpr int kEntrySlotName = 48;
constexpr int kStringSlotText = 5;
// The folder (that interface) keeps every object definition it loaded as cTSObjectSelectors: a
// vector at +0x50 (begin, end) and a second one through [+0x6c] (begin, end), as its slot 13
// (0xa04d3570, by index across both) reads them, valid while the byte at +0xc is set. Its slot 22
// (0xa04d3450) finds a part's master the same way: the selector of the same resource file
// (selector +0x90, its slot 66) whose OBJD has the same field 10 (objd::kMasterId).
constexpr int kFolderLoaded = 0xc;
constexpr int kFolderSelectors = 0x50;
constexpr int kFolderMoreSelectors = 0x6c;
constexpr int kSelectorFile = 0x90;
}  // namespace catalog

// A Sim's motives, as Lua's GetPersonMotive (0xa0574040): the object's QueryInterface (slot 0)
// for this iid gives the person interface; its slot 13 returns motive n (0..15) as a float.
namespace person {
// The Sim's name (as the interaction menu's title, 0xa0622163): on the person interface at
// +752 of the object, slot 8 gives an object whose slot 61 is the neighbor; the neighbor's slot 49
// fills a cRZString with the first name, slot 51 with the last name.
constexpr int kInterface = 752;
constexpr int kSlotSim = 8;
constexpr int kSimSlotNeighbor = 61;
constexpr int kNeighborSlotFirstName = 49;
constexpr int kNeighborSlotLastName = 51;
constexpr uintptr_t kRZStringVtable = 0xa0d9b470;
constexpr uint32_t kIid = 0x5a69d3d5;
constexpr int kSlotMotive = 13;
constexpr int kMotiveCount = 16;
// Person data (Lua's GetPersonData, 0xa0573f20, calls this slot): short (int index, int 1), ret 8;
// it reads iface + 0x356 + 2 * index. Indices (the Lua PersonData table, ObjectScripts 0xFF10C589):
// 8 current outfit, 31 neighbor id, 58 age (days), 65 gender (0 male, 1 female), 81 body flags
// (bit 0 fat, bit 4 fit: the body morphs at 0xa065d2fe), 175 young adult, 186 species (0 human).
constexpr int kSlotPersonData = 10;
constexpr int kDataOutfit = 8, kDataNeighborId = 31, kDataAge = 58, kDataGender = 65, kDataBodyFlags = 81,
              kDataYoungAdult = 175, kDataSpecies = 186;
// 34 greet status (0 not greeted yet: a visitor knocks and waits at the door, "Controller - Visitor"
// 0x7f92c86e "CT - Wait to be Greeted"; 1 greeted, set by the greet social and "Start Visiting";
// 2 leaving, set by goodbye and shoo), 155 NPC type (the global BCON 0x132 "NPC Types": the code
// compares with its entries, constant data 0x4000 | 0x32 << 7 | index), 160 aspiration score
// (-100..100, the live panel's aspiration bar), 162 aspiration reward points / 10 not spent.
// The Lua table names 161 "Aspiration Reward Points Spent (/10)", but it holds the points the
// rewards panel shows (read live: 1010, the panel's "10.100").
constexpr int kDataGreetStatus = 34, kDataNpcType = 155, kDataAspirationScore = 160, kDataRewardPoints = 161;
}  // namespace person

// A neighbor's looks (Lua's getOutfitProperty, 0xa0566b20): cTSNeighbor slot 44 (0xa04b7fc0),
// uint16 thiscall (uint16 outfit flag, uint16 property), ret 8: the outfit data's answer (cached by
// the game per flag and property). Properties (Lua OutfitProperty, 0xFFB4A6D8): 0 hair (1 custom,
// 2 black, 3 brown, 4 blond, 5 red, 6 grey, 7 dyed), 1 skin (1 custom, 2 light, 3 medium, 4 medium
// dark, 5 dark, 6 alien, 7 zombie), 2 eyes (1 custom, 2 green, 3 brown, 4 light blue, 5 dark blue,
// 6 grey, 7 alien), then 1 when present: 3 facial hair, 4 glasses, 5 makeup, 6 full-face makeup,
// 7 hat, 11 jewelry. The outfit flag from person data 8: 0xa023f55f's table (outfitFlagOf below).
namespace looks {
constexpr int kNeighborSlotOutfitProperty = 44;
constexpr uintptr_t kOutfitProperty = 0xa04b7fc0;  // checked before the call
enum Property { kHair = 0, kSkin = 1, kEyes = 2, kFacialHair = 3, kGlasses = 4, kMakeup = 5, kFullMakeup = 6,
                kHat = 7, kJewelry = 11 };
}  // namespace looks

// An action queue icon (cTSWinActionQueueIcon, from the cIGZWin pointer) holds its cTSInteraction,
// whose +0x0e is the short id of the object it is done with (its slot 16 finds the object by it).
namespace interaction {
constexpr int kIconInteraction = 0xfc;
constexpr int kObjectId = 0x0e;
}  // namespace interaction

// The camera (cTSCameraSmoothSims1Controller; nTSSG::cCameraLive its leaf). The lot's own is the one
// with the most zoom steps (other instances keep 2 or 5, at the origin): 70, 40, 25, 15, 8, 3. Keys Z
// and X go through the list. It stays the one in use in buy and build mode (read live: cCameraBuild
// stays at the origin).
namespace camera {
constexpr uint32_t kLiveVtable = 0xa0dba6fc;
constexpr int kOrientation = 0x38;  // float x, y, z, w: the view's direction is this turning +y
constexpr int kZoomSteps = 0x70;    // std::vector<float>, the distances, farthest first
constexpr int kZoomTarget = 0xe0;   // float: the distance it is going to
// Centring it (found by disassembly 2026-11-07; the live panel's "Center on active Sim" 0xa07787a4 ->
// the main toolbar's slot 38 0xa061b36c): an event to the camera's event interface E = camera +
// 0x4a0 (the vbtable at +4 holds 0x49c at its +4): bool thiscall E->vt[4](id, 0, event*), the event
// {vtable 0xa0db5e70 (nGZEvent::cVectorEvent<cVector<3,float>>), float x, y, z} in world units
// (tiles). 0xbb661105 glides there, 0xbb661106 jumps; both stop the camera following the Sim. The
// look-at: +0x16c now, +0x178 the target.
constexpr int kVbtable = 4, kEventBaseOffset = 0x49c, kEventInterface = 0x4a0;
constexpr uint32_t kEventGlide = 0xbb661105, kEventJump = 0xbb661106;
constexpr uintptr_t kVectorEventVtable = 0xa0db5e70;
constexpr int kLookAt = 0x16c;
}  // namespace camera

// Messages (cRZMessage2Standard, 0x2c bytes, constructor 0xa01c033d) posted to the game's message
// server ([0xa10f519c], cGZMessageServer2, slot 4 = post(msg, 0)). The main toolbar
// (cTSMainToolbar, handler 0xa061b99c) opens the interaction menu of the object in data 1 for
// type 0xC0 (0xa0621f50 builds it).
namespace message {
constexpr uintptr_t kAlloc = 0xa01be3b0;  // the game's operator new (cdecl, size)
constexpr uintptr_t kCtor = 0xa01c033d;
constexpr size_t kSize = 0x2c;
constexpr uintptr_t kServer = 0xa10f519c;
constexpr int kSlotPost = 4;
constexpr int kSlotSetType = 5;
constexpr int kSlotSetData1 = 11;
constexpr uint32_t kShowPieMenu = 0xC0;
}  // namespace message

// The neighborhood's lot info dialog (cTSWinProcLotInfoDialog): the window with this id holds
// the proc; its cITSWinProcLotInfoDialog interface (+12) has Show(lotId, ?, mode) in slot 4.
namespace lotinfo {
constexpr uint32_t kWindowId = 0xcdc02de3;
constexpr int kInterface = 12;
constexpr uintptr_t kShow = 0xa07b7b27;
}  // namespace lotinfo

// cGZWinBtn, from the complete object (the cIGZWinBtn interface is at +0, cGZWin at +4).
namespace btn {
constexpr int kFlags = 0xfc;    // GetBtnFlag (0xa0936a5c): 1 on, 2 toggles on click, 0x800 stays on
                                // (radio), 0x1000 held down by the mouse
constexpr uint32_t kFlagOn = 1;
constexpr uint32_t kFlagToggle = 2;
constexpr uint32_t kCheckBoxBits = 0xd4;  // check boxes (0x6d4): "Forzar reloj 12 hr."
constexpr uint32_t kFlagChoice = 4;  // one of a set of options (the options pages' No / Si / Pequeno...
                                     // 0x604 / 0x605 when chosen; check boxes 0x6d4); plain buttons
                                     // 0x780, mode buttons 0x580 (hypothesis from the options pages)
constexpr int kStyle = 0x160;
constexpr int kCaptionExtent = 0x124;  // non-zero while the caption is drawn (hidden captions are
                                       // design-time names such as "video capture"; hypothesis
                                       // from comparing shown and hidden captions)
constexpr int kTip = 0x188;     // cRZString tooltip ("Title|Description"), std::string at +0x18c
}  // namespace btn

// cTSWinProcStepFillBar (Create a Sim's personality bars; the window's proc). Its message handler
// (0xa0624391) steps the value on message 3 / 0x287259f6 (data 1 down, 2 up) through SetValue.
// cGZWinScrollbar2 (the options' volume and quality sliders, lists' scroll bars): the window is the
// complete object, its cIGZWinScrollbar2 interface at +0xf8 (vtable 0xa0e57ca4): +0xfc value, +0x100
// minimum, +0x104 maximum (the value goes to maximum - page), +0x108 page. Slot 6 (0xa095b015)
// SetValue(int value, bool notify), ret 8: clamps, redraws, and with notify sends the parent the
// scroll notification 0x4b42dcc7 as a drag does (the sound panel applies the volume). Read live
// 2026-11-08 on the audio options: Música 40 (0..105, page 5), Calidad de audio 3 (1..4, page 1).
namespace scrollbar {
constexpr int kInterface = 0xf8;
constexpr int kValue = 0xfc, kMin = 0x100, kMax = 0x104, kPage = 0x108;
constexpr int kSlotSetValue = 6;
}  // namespace scrollbar

namespace stepbar {
constexpr int kValue = 0x6c;  // int, 0..kMax (a personality point is 100)
constexpr int kMax = 0x74;    // int (1000)
constexpr int kSteps = 0x80;  // int (10)
constexpr int kValueInterface = 8;  // the interface whose slot 8 is SetValue(int value, bool notify, int)
constexpr int kSlotSetValue = 8;
}  // namespace stepbar

// cTSWinListBox (from the complete object; the cIGZWin interface is at +4): rows are
// cWinListBoxRow windows (a press selects one), each with cells of 0x68 bytes.
namespace listbox {
constexpr int kRows = 0xfc;        // std::vector<cWinListBoxRow*>, in order (all rows, scrolled out too)
constexpr int kSelected = 0x13c;   // std::vector<int>: the selected rows' indices
constexpr int kRowCells = 0xf8;    // row: std::vector of cells, 0x68 bytes each
constexpr int kCellSize = 0x68;
constexpr int kCellText = 0x04;    // cell: std::string (behind a cRZString vtable) as drawn (may be cut "...")
constexpr int kCellFullText = 0x24;  // cell: std::string, the whole text
}  // namespace listbox

// cGZWinTextEdit, from the cIGZWin pointer: the text as a cRZString (std::string at +0x100).
namespace edit {
constexpr int kText = 0xfc;
constexpr int kFlags = 0x160;        // cIGZWinTextEdit flags (IsFlagSet 0xa09504ec on the +0xf8
constexpr uint32_t kFlagEditable = 1;  // interface): 1 editable, as in SimCity 4's header
// The insertion index (the caret) of that +0xf8 interface: its slot 73 (0xa094f5e9,
// GetInsertionIndex, SC4's 67 + 6) returns [iface + 0x5c]. Bytes of the UTF-8 text (hypothesis:
// only checked on ASCII names).
constexpr int kCaret = 0xf8 + 0x5c;
}  // namespace edit

namespace simantics {
constexpr uint32_t kBalloon = 0xa05805c0;  // opcode 0x70 handler: thiscall (thread, frame, operands)
constexpr uint32_t kBalloonThumbnail = 0xa06c1092;  // cTSUIPaintingService slot 6
}  // namespace simantics

}  // namespace game::addr
