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
constexpr int kFamily = 0x1a70;        // std::vector of the family's Sims (cTSPerson interface +752)   // the selected cTSPerson (the pie builder 0xa0621f50 uses it)
}  // namespace objmodule

// cTSObject virtual slots (as Lua's GetObjectName 0xa0566100 and GetObjectLocation 0xa0566030).
namespace object {
constexpr int kSlotName = 92;          // const char* name
constexpr int kSlotLocation = 33;      // -> {int x, int y}, 16.16 fixed point, in tiles
constexpr int kSlotLevel = 36;         // short: 0 the terrain, 1 the ground floor of a house on a
                                       // foundation (a patio set outside reads 0, the rooms 1)
constexpr int kDirection = 0xa8;       // short, SimAntics direction: 0 faces -x, 2 +y, 4 +x, 6 -y (a
                                       // dining set: each chair faces its table); the object tool's
                                       // rotation is the same number for the held object
constexpr int kDefinition = 0x238;     // -> cTSPersistResourceT<cEdithObjectDefinition> (OBJD)
}  // namespace object

// The object definition (OBJD, parsed), from the resource above.
namespace objd {
constexpr int kGuid = 0x20;
constexpr int kPrice = 0x28;
constexpr int kRoomFlags = 0x52;       // u16 buy mode rooms: 1 kitchen, 2 bedroom, 4 bathroom, 8 living,
                                       // 0x10 outside, 0x20 dining, 0x40 misc, 0x80 study, 0x100 kids
constexpr int kRatings = 0xa8;         // s16 [7]: hunger, comfort, hygiene, bladder, energy, fun, room
                                       // (OBJD fields 82..88; the data's u16 fields start at +4)
constexpr int kFunctionFlags = 0x54;   // u16 buy mode categories: 1 comfort, 2 surfaces, 4 appliances,
                                       // 8 electronics, 0x10 plumbing, 0x20 decorative, 0x40 misc,
                                       // 0x80 lighting, 0x100 hobbies (checked on known objects)
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
constexpr int kSlotRoomManager = 39;
constexpr int kRoomManagerSlotRoomAt = 80;
}  // namespace world

// The catalog's names by GUID, as Lua's GetCatalogName (0xa0561640): cTSGlobals slot 24 gives the
// object folder, its slot 14 the entry for a GUID, the entry's slot 48 fills a cIGZString* with
// the catalog name (in the game's language); the string's slot 5 is its text.
namespace catalog {
constexpr int kGlobalsSlotFolder = 24;
constexpr int kFolderSlotByGuid = 14;
constexpr int kEntrySlotName = 48;
constexpr int kStringSlotText = 5;
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
}  // namespace person

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
namespace stepbar {
constexpr int kValue = 0x6c;  // int, 0..kMax (a personality point is 100)
constexpr int kMax = 0x74;    // int (1000)
constexpr int kSteps = 0x80;  // int (10)
constexpr int kValueInterface = 8;  // the interface whose slot 8 is SetValue(int value, bool notify, int)
constexpr int kSlotSetValue = 8;
}  // namespace stepbar

// cGZWinTextEdit, from the cIGZWin pointer: the text as a cRZString (std::string at +0x100).
namespace edit {
constexpr int kText = 0xfc;
constexpr int kFlags = 0x160;        // cIGZWinTextEdit flags (IsFlagSet 0xa09504ec on the +0xf8
constexpr uint32_t kFlagEditable = 1;  // interface): 1 editable, as in SimCity 4's header
}  // namespace edit

}  // namespace game::addr
