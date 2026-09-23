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
  (1 = ground) [Lua GetObjectName 0xa0566100, GetObjectLocation 0xa0566030]. +0x238 -> OBJD
  resource: +0x20 GUID, +0x28 price, +0x54 function flags (buy categories, u16).
- Catalog name by GUID [Lua GetCatalogName 0xa0561640]: globals slot 24 (object folder) -> slot 14
  (guid) -> entry; entry slot 48 fills a `cIGZString*`; its slot 5 is the text. Internal objects'
  names start with `*`.
- Interaction menu builder 0xa0621f50 (thiscall(objectId, second)): the main toolbar calls it for
  message 0xC0 (handler 0xa061b99c) with the object in data 1, the second id in data 2.
