# Memory layouts (Sims2EP9.exe 1.18.0.187, SHA-256 24041f83...)

Preferred-base VAs (0xA0000000). Offsets are from the pointer named. Evidence in brackets.
The code's single source of truth is `mod/core/game/addresses.h`.

## Framework and services

- `cCTGFramework` static at `0xa10fdb50`. Slot 5 `GetSystemService(serviceId, iid, void**)`,
  thiscall, ret 0xc [disasm 0xa01428e2; console uses it at 0xa01d0979].
- Window manager service: id `0xa417445e`, iid `0x5a4` -> `cGZWinMgrW32` [live: returns the
  manager]. Also cached at `[0xa10f51ac]` (getter 0xa09c5bd6).
- Cheat system service: id `0x2a4915ea`, iid `0x4a490e00`; slot 7 `Execute(const char* line)`
  -> bool [0xa0cbe460; the console calls it with the typed line; `help -all` output verified].
- `cTSGlobals` at `[0xa10cc288]` (getter 0xa020ddfb): +0x20 simulator (`cTSSimulator` +4),
  +0x2c `cTSLot`, +0x30 current `cTSNeighborhood`, +0x38 `cTSNeighborhoodManager`, +0x40
  `cTSCatalog`, +0x44 `cTSUIBridge`, +0x5c `cTSGameStateController`, +0x60 `cTSFamilyManager`,
  +0x64 `cTSNeighborManager`, +0x70 `cTSObjectFolder` (+4), +0x78 `cEdithObjectModule`,
  +0x8c `cTSInventory`, +0x94 `cTSWeatherInfo` [memtool, vtable getters slots 3-33].
- Message server `cGZMessageServer2` at `[0xa10f519c]` (getter 0xa09c5a89); slot 4 posts
  (msg, 0). `cRZMessage2Standard` (0x2c bytes, ctor 0xa01c033d, allocated by 0xa01be3b0):
  +0xc type (slot 5 sets), +0x10/+0x14/+0x18/+0x1c data 1-4 (slots 11/16/21/26 set, 9/14/19/24
  get, 8/13/18/23 "has"), +8 flags of which are set.

## GZ windows (cIGZWin pointer = what child lists hold)

| Offset | Meaning |
|---|---|
| +0x04 | window manager |
| +0x10 | id (slot 78) |
| +0x54 | -> child container: `[container]` = std::list head; node: next +0, prev +4, cIGZWin* +8 (front to back) |
| +0x58 | parent (slot 11) |
| +0x60 | caption `cRZString`; its std::string (UTF-8) at +0x64 (slot 89) |
| +0xc8 | area l, t, r, b in the parent's coordinates (slots 51-54) |
| +0xe8 | flags: 1 visible (slot 86), 2 enabled (slot 88) |
| +0x12c | window proc (`cTSWinProc*`) for windows that have one [live: proc classes match the screens] |

Virtual slots: 6 GetWindowManager, 10 GetMainWindow, 11 GetParentWin, 13 GetChildCount,
49/50 width/height, 78 GetID, 86 IsVisible, 89 GetCaption, 151 key down, 155 mouse down left
(x, y, flags in window coordinates), 157 mouse up left (the button's click fires here), 161 mouse
enter, 162 mouse exit [vtable 0xa0e52f54; button overrides decompiled 0xa09357ef, 0xa09359f1].

### cGZWinMgrW32
+0x34 capture (slot 29), +0x38 focus (slot 37), +0x3c **the innermost modal window**
(DoModalWin 0xa0972f48 saves/restores it; 0 when none), +0x40 window under the mouse, +0x44 main
window (slot 3), +0x190 modal nesting count (slot 44).

### cGZWinBtn (complete object; the cGZWin part is at +4)
+0xfc button flags (GetBtnFlag 0xa0936a5c): 1 on, 2 toggles on click, 0x1000 held down;
+0x124 non-zero while the caption is drawn (hypothesis from comparing shown and hidden captions);
+0x160 style; +0x188 tooltip `cRZString` (std::string at +0x18c). `cTSWinTextLabel` is a button
subclass used for labels showing a value (tooltip names it).

### cGZWinTextEdit (from the cIGZWin pointer)
+0xfc text `cRZString` (std::string at +0x100); +0x160 flags (cIGZWinTextEdit at +0xf8, IsFlagSet
0xa09504ec): 1 editable (SimCity 4's TextEditFlags).

### cTSWinProcGradientFillBar (a meter; the window's proc)
+0x70 value, +0x74 min, +0x78 max (needs: -100..100) [live: matched the needs panel].

## Neighborhood

- `cTSNeighborhood` +0x5c: `std::map<int lotId, cTSLotInfo*>` head node (MSVC tree: left +0,
  parent +4, right +8, isnil byte +0xd, key +0x10, value +0x14); the value points at the
  cTSLotInfo interface at +8. Slot 57 looks a lot up by id.
- `cTSLotInfo`: +0x18/+0x1c position on the neighborhood grid, +0x24 resource name ("E001_Lot68"),
  +0x44 lot id, +0x80 -> `cTSBaseLotInfo` (+8).
- `cTSBaseLotInfo`: +0x18/+0x1c size (hypothesis: 10-tile units), +0x20 type (0 residential,
  1 community, 8 apartment building, 9 apartment unit; getter 0xa04c9c20 maps 5 to 1 without the
  pack), +0x28 name `cRZString` (+0x2c text), +0x48 description, +0x68 flags (0x1000/0x2000/0x4000
  lot class, 0x10 on apartment units; hypothesis), +0x9c value (hypothesis).
- `cTSFamilyManager` +0x14/+0x18: `std::vector<cTSFamily*>`. `cTSFamily`: +0x14 name (+0x18 text),
  +0x34 description (+0x38), +0x54 id, +0x58 home lot (0 = not moved in), +0x70 funds,
  +0x7c/+0x80 members (4 bytes each).
- Lot info panel: window id 0xcdc02de3, proc `cTSWinProcLotInfoDialog`; its interface at +12,
  `Show(lotId, familyId, mode)` = 0xa07b7b27 (slot 4; mode 3 keeps the previous mode).

## Lot

- `cEdithObjectModule` (globals +0x78): +0x1a24/+0x1a28 `std::vector<cTSObject*>` by id - 1
  (slot 29, 0xa04281f0); +0x1a70 the family's Sims (cTSPerson interface at +752); slot 39 the
  selected Sim (first with flag 2), as that interface. Multi-tile parts are `cEdithMTObject`.
- `cTSObject` slots: 92 name (const char*, internal English), 33 -> {x, y} 16.16 tiles, 36 floor
  [Lua GetObjectName 0xa0566100, GetObjectLocation 0xa0566030]: 0 the terrain, 1 the ground floor
  of a house on a foundation (a patio set reads 0, the rooms 1). +0xa8 short SimAntics direction:
  0 faces -x, 2 +y, 4 +x, 6 -y (a dining set: each chair faces its table; +0x98 held the same on
  loaded objects but not on new ones). +0x238 -> OBJD resource: +0x20 GUID, +0x28 price, +0x52
  room flags, +0x54 function flags (buy categories, u16), +0xa8 s16[7] ratings (OBJD fields 82..88:
  hunger, comfort, hygiene, bladder, energy, fun, room; the data's u16 fields start at +4).
- Catalog name by GUID [Lua GetCatalogName 0xa0561640]: globals slot 24 (object folder) -> slot 14
  (guid) -> entry; entry slot 48 fills a `cIGZString*`; its slot 5 is the text. Internal objects'
  names start with `*`.
- Scene graph nodes (`nTSSG::cObject`, `cAnimatable`, `cLocomotable`, `cPerson`): +0x88 the lot
  object. The tools' object pick returns the node's 135-slot interface, at +324 / +432 / +860 /
  +912 by class. No pointer back from the object was found: the mod scans memory for a node
  whose +0x88 is the object (about 0.4 s) and caches it.
- Interaction menu builder 0xa0621f50 (thiscall(objectId, second)): the main toolbar calls it for
  message 0xC0 (handler 0xa061b99c) with the object in data 1, the second id in data 2.

## Buy mode

- The main toolbar (`cTSMainToolbar`, system service 0xe92db716, iid 1 gives its +4 interface):
  +0x74 the UI mode (`cTSUIModeLive` / `cTSUIModeBuy` / ...), +0x90 the `cTSUserToolHandler` (its
  interface at +8), whose +0x28 is the current user tool. The object tool is replaced when a
  catalog product is pressed: read it there each time.
- Catalog: `cTSWinProcCatalog` (+0x10 vector of thumbnail windows and procs; +0x11c..+0x128 the
  previous/next page buttons) holds `cTSWinProcCatalogThumbnail` windows; a thumbnail proc +0x14
  -> `cTSProductObject` interface (+0x10c into the object). The product: cRZStrings (vtable, then
  std::string) at +0x0c internal name, +0x2c catalog name, +0x4c description; +0xac price;
  +0xfc `cTSObjectSelector`, whose +0x60 is the OBJD resource. The thumbnail's button (child id 3)
  has the price as tooltip. While a product or lot object is held, `cTSWinProcSellPanel` (the sell
  button, with the game's "click or press Delete to sell for ..." text) replaces the buy panel.
- `cTSUserToolObject` (the buy mode hand and placement tool): +0x50 u8 the last pick is on the
  lot; +0x54 float x, y, int level: the last pick; +0x60 u8 a pick is stored; +0x80 float x, y,
  int level: where the held object stands (tile centres); +0xa0 int rotation (steps of 2, not
  wrapped; the game's , and . add -2 / +2; the object's direction); +0xa4 the last held GUID;
  +0xd0 the held object's scene node. 0xa067be89 (thiscall) is "holding", 0xa067cb69 the held
  cTSObject. Mouse messages reach the tool's dispatcher 0xa078dc91: 3 move (slot 42 pick, then
  slot 45 move: the held object goes to the pick and is judged, slot 71; the verdict tints it
  through slot 86), 4 down, 5 up, 6 the cursor left the view (slot 48 clears +0x50).
- Placement errors: UIText.package STR# 0x89 ("Can't intersect other objects", "Must be against
  wall" ...), the tooltip the game shows while a spot is refused. Where the tool keeps the
  current error was not found yet.

## Build mode

- The dragged build tools (`cTSUserToolWall` for walls, fences and rooms, `cTSUserToolFloorPattern`,
  `cTSUserToolDeckFoundation`, `cTSUserToolCurvedPool`) share the object tool's dispatcher (slot 8,
  0xa078dc91: thiscall (int buttons, int message, int pos[2], bool handled); buttons 1 = left, checked
  on a down against the tool's +0x20). Messages: 3 move, 4 down, 5 up. The first pick goes through
  slot 31; once a pick is stored the tools use slot 32 (0xa078d6f0, the pick at a fixed level):
  both must answer for a drag. The wall tool rounds a pick to the nearest corner (0xa07dd870), keeps
  the drag start at +0x84 and the end at +0x110 (floats x, y, int level), +0x78 its mode (0 wall; 1
  is set while Ctrl is held: removal), +0x11c "dragging". Its handlers: slot 55 position, 56 down,
  58 drag, 57 up (the up builds). A real drag spans frames: the mod sends one message per tick.
- Pick levels: 0 is the terrain, 1 the floor of a house on a foundation (the same numbers as the
  objects' level).
- Every catalog product derives from `cTSProduct` (the complete object through RTTI; interface
  offsets differ: `cTSProductObject` +0x10c, `cTSProductWall` +360...): +0x30 name, +0x50
  description (std::strings behind cRZString vtables), +0xac price.

## Neighborhood tools

- Choosing a family in the family bin makes `cTSUserToolNeighborhoodMoveIn` the toolbar's tool (the
  same `cTSMainToolbar` handler as on lots). Its dispatcher (slot 8, 0xa079a1d5, thiscall (0,
  message, int pos[2])) handles 3/4/5 only while the Windows cursor shows (`cRZCursorManager`
  slot 16 asks `GetCursorInfo`); on a down it asks slot 24 (0xa079a5a4: thiscall (int pos[2], int*
  lot, cIGZString* name) -> bool, a ray against the scene's "LotData"/"LotId" nodes) for the lot,
  then asks "move the family here, cost ..." in a `cTSWinPictureDialog` that runs its loop inside
  the call.
- The lot info dialog (`Show(lot, family, mode)`, 0xa07b7b27) keeps mode +0x54, family +0x58, lot
  +0x5c; it offers no move-in button ("select a family to move in").
- Cheats: `familyFunds <surname> <amount>` works in the neighborhood.

## Create a Sim

- No panel procs: containers keep design-time English captions ('Name / Genetics Step 1', 'Age',
  'Gender', 'full wizard', 'TAE Aspirations', 'Zodiac', 'CAS PUC', 'CreateSim Controls').
- Heads, hair, clothes: `cGZWinGrid` cells of buttons captioned with resource keys
  ('ac598eac-7f61f940-1'); no names.
- Personality: `cTSWinProcStepFillBar` procs ('nice', 'playful', 'active', 'outgoing', 'sloppy
  neat', 'total'): +0x6c value (100 a point), +0x74 max (1000), +0x80 steps (10); the proc's +8
  interface slot 8 is SetValue(value, notify, 0) (what its step message calls); the 'total' bar's
  value is the points left, updated a moment later. The traits' names are `cTSWinTextLabel`s on
  the bar's row.
- Buttons: bit 0 of the flags is "on". Options (No/Si, sizes) 0x604/0x605; check boxes 0x6d4;
  plain buttons 0x780/0x781 (a chosen aspiration or star sign is 0x781 among 0x780s; Accept and
  Cancel are 0x781 at times too); mode buttons 0x580/0x581; a lone dialog button can be 0x605.
- Text fields show their prompt as their text ("Ponle un nombre"); the first key replaces it.
- Turn-ons (Nightlife): three unnamed slot buttons under the texts "Me pone" / "Me corta"; each opens
  a modal list ("Haz una selección") of traits.

## List boxes

- `cTSWinListBox` (complete object; `cIGZWin` at +4): +0xfc `std::vector<cWinListBoxRow*>` all rows in
  order (5 shown at a time in the travel dialog), +0x13c `std::vector<int>` the selected indices.
  A row (`cWinListBoxRow`, a window) +0xf8 is a vector of 0x68-byte cells: +0x04 the drawn text
  (cut with "..."), +0x24 the whole text (std::strings behind cRZString vtables), +0x44 a data
  object (`cTSLotInfo` in the travel dialog). A press on a row selects it, scrolled out of view too.
