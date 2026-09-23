# The Sims 2 Legacy Collection: screen reader accessibility mod

Playbook: `template_generic.md` (read it first; its rules apply). This mod is a port of the Sims 1
accessibility mod (`C:\Users\Nitropc\code\mods\sims`, same author, same architecture); its tools and
generic code were copied and retargeted, its game layer rewritten. A copy of the Sims 1 core is in
`re/sims1_core_ref` (gitignored) for reference and for `tools/restore_strings.py`. This file holds
what has been learned about *this* game. Anything marked **hypothesis** has not been verified.

Status (2026-09-23): the mod runs (`mod/`). Speech through NVDA (Prism in `SpeechHost.exe`).
Keyboard navigation covers every GZ-window screen and dialog generically (menus, neighborhood
chooser, dialogs, tutorials, game tips, the neighborhood panel, the lot info panel, the live mode
HUD with needs, wants and fears, speed, clock, funds), the neighborhood's lots as a list (Enter
opens the game's lot panel, "Entrar en solar" loads the lot), interaction (pie) menus, the lot
object browser (PageUp/PageDown by buy mode category or room, Enter opens the object's
interactions), live state and a narrator, and buy mode (catalog products; placing, moving and
selling objects through a list of spots where they fit).
Not done: see `ROADMAP.md`. Screen inventory: `SCREENS.md`. Targets: `PATCH_TARGETS.md`; layouts:
`STRUCTS.md`.

## Working on this machine (read first)

- **The user uses this PC while sessions run.** Never move the real mouse, steal focus, or grab
  the screen. Screenshots: `python tools/game.py shot <name>` (the dev core saves the D3D9 back
  buffer on the next Present; PrintWindow sees nothing of the game). Clicks: the mod's own virtual
  mouse (`game/mouse.h`, dev command `!click x y`), never the real cursor.
- **A minimized game neither draws nor simulates** (interaction menus never open, no ticks of the
  game's own loop). If the user minimized it: `python tools/game.py restore` un-minimizes it behind
  their windows without activating it.
- **Frida on this game**: `Interceptor.replace` of `SetCursorPos` + a pie menu's modal loop crashed
  the game when the script unloaded. Prefer the mod's own hooks (MinHook); keep Frida read-only.
- **The game runs in Spanish** (registry `HKCU\Software\Electronic Arts\The Sims 2 Ultimate
  Collection 25\1.0\language` = 5, Steam's numbering). Every mod string needs every language.
- **Save files are disposable** (the user wipes them before really playing). A backup of the
  neighborhoods from before the first mod run: `C:\Users\Nitropc\code\mods\sims-backups\2026-09-23-sims2-userdata`.
  User data: `Documents\EA Games\Los Sims 2 Legado` (Neighborhoods, logs in `Logs\`).
- NVDA runs on this PC; audible speech tests are allowed (keep them short).

## The mod (`mod/`)

| Part | Output | Notes |
|---|---|---|
| `mod/loader/` | `version.dll` (x86) next to `Sims2EP9.exe` | Proxy for the system DLL. Checks the exe hash, waits for the window (class `{0xdb637c87}`), subclasses it (ANSI), loads a **copy** of the core, 30 ms timer. Catches core crashes; always calls the core's shutdown on unload (so its hooks go). |
| `mod/core/` | `Sims2Access\Sims2Access.dll` (x86) | `main.cpp` wires `util/`, `lang/`, `speech/`, `game/` (all addresses in `game/addresses.h`), `nav/`, `input/`. |
| `mod/speechhost/` | `Sims2Access\SpeechHost.exe` (x64) + `prism.dll` | Unchanged from the Sims 1 mod. |

- The game folder is `S:\steam\steamapps\common\The Sims 2 Legacy Collection\EP9\TSBin` (the one exe
  runs the base game and every expansion). Build: `mod\build.bat` (MSVC 2022); `mod\build.bat deploy`.
- **Dev loop** (`Sims2Access\dev.txt`): the loader hot-reloads `Sims2Access.dll` within a second of a
  rebuild. Dev builds also take requests from files: `tools/game.py shot <name>` (screenshot),
  `tools/game.py cheat "<line>"` (a cheat console line through the game's cheat system) and
  `tools/game.py cheat "!<command> args"` (mod test commands in `game/cheats.cpp`: `!objects` logs
  the lot's objects, `!pie <id>` opens an object's interaction menu, `!lotinfo <lot> <family> 0`,
  `!click x y`, `!move x y`, `!release`, `!rooms`, `!check x y n` times n placement checks).
- A crash in the core is logged as `CRASH in tick: ... at <addr> (Sims2Access_<pid>_<n>.dll+0x....)`
  and swallowed; `python tools/symbolize.py --log` names function, file and line from the PDB.
  **MSVC 14.44 /O2 miscompiled an inlined helper once** (`FirstControl` in a loop read an unwritten
  stack slot): the helper is out of line now (`nav/model.h`).
- Launching: `python tools/game.py launch` (`steam://rungameid/3314070`; no arguments, so Steam asks
  nothing). `python tools/drive_to_lot.py [neighborhood] [family]` drives the menus by keyboard from
  the neighborhood chooser into a lot (about 3 minutes with the welcome dialogs).
- `python tools/keys.py tab down enter` posts keys (bit 25 of lParam marks them); the mod hooks
  `GetKeyState`/`GetAsyncKeyState` so the game also sees the tool's Ctrl/Shift/Alt as held
  (posted Ctrl+Shift+C opens the cheat console). Real keys are untouched.
- Mod keys (current):
  - Arrows move within a group, Tab/Shift+Tab change group, Home/End, Enter presses, R repeats,
    Ctrl+F1 the key help. Ctrl+Up/Down read the review buffers, Ctrl+Left/Right switch them.
  - Lot view: PageDown/PageUp the lot's objects (nearest the selected Sim first), Shift+PageUp/
    PageDown the category (Sims, then the buy mode categories with the game's names, then other
    objects), Ctrl+PageUp/PageDown the rooms; Enter opens the object's interaction menu (in buy
    mode: picks it up to move it), R repeats. (The game's PageUp/PageDown change floor; the
    control panel's floor buttons still do.) Ctrl+E the selected Sim's state, Ctrl+1..9 select a
    family member.
  - Buy mode (`nav/placement.cpp`): Enter on a catalog product picks it up; then PageDown/PageUp
    the spots where it fits (nearest the Sim first; a scan of the whole lot through the tool's own
    check, about 0.1 ms a tile), Ctrl+PageDown/PageUp by room, arrows move a tile (Shift five),
    Home back to the Sim, `,` `.` turn it (the game's keys; the facing is said), Enter buys (the
    game hands over another copy until Escape), Delete sells a picked-up object, Escape drops it
    or puts it back. How it works: the tools' ground pick is hooked to answer with the mod's tile,
    the "cursor left the view" handler is skipped, the virtual mouse sits in the 3D view and
    moves a pixel to make the tool re-pick; picking up an object hooks the object pick to answer
    with that object's scene node (STRUCTS.md "Buy mode").
  - A text field the player can edit takes letters, Space and Backspace (each echoed); Enter,
    Home/End and Left/Right go to it only while the mod's focus is on the field (Create a Family
    keeps its surname field focused all along). A field's name is the prompt it first showed.
  - Sliders (Create a Sim's personality): Left/Right change them, with the points left.
  - Neighborhood: choose a family in the family bin (F1 panel), then Enter on a lot in the lots
    list: the game asks whether to move the family in and at what cost.
  - Build mode (`nav/build.cpp`): Enter on a wall, floor, foundation or pool style starts a cursor;
    arrows move it (Shift five), PageUp/PageDown change level (0 terrain, 1 a foundation's floor),
    Space marks the start, Enter builds from the mark to the cursor, Ctrl+Enter removes; the cost is
    said. The mod drags the game's own tool, one mouse message per tick. After a wall is built, a
    family Sim indoors in a room without a door is announced ("cuidado: Carmen queda encerrado...
    Control+Z deshace la pared"; dev command `!shutin` runs the check).
  - Ctrl+Enter on the object browser: walk the selected Sim there (Ctrl+Shift+Enter runs).
  - Ctrl+L (lot view): the room's layout at the build cursor, else where the selected Sim stands:
    name, floor, tiles, bounds, doors and windows (the details buffer: each with its wall and the
    room behind). Doors and windows are objects whose parts stand on both sides of their wall
    (OBJD type 8 / 9, sub index 0 is the lead part, facing its direction from the front tile).
  - Cheat console (the game's Ctrl+Shift+C, the "Debug Console" window): its input reads
    "consola de trucos: <text>"; Enter on it clicks it (a click gives a text field the keyboard);
    typing is echoed; Enter runs the line (Enter goes to a game field after typing into it since the
    last navigation key); what the command writes to the output is spoken (`nav/console.cpp`; the
    output grows past 40 KB with the game's log lines, read by offset).
  - Family tree (Create a Family > Relaciones de familia): Enter on a Sim picks it up ("ana
    seleccionado: Intro sobre otro Sim..."), Enter on another drags it there with the virtual mouse
    (`game::DragWindow`, one message per tick), Enter on the same one puts it down; the game then asks
    how they are related.
  - Ctrl+N: the narrator speaks everything, warnings only (urgent needs, visitors), or nothing
    (`[mod] narrator=2/1/0` in `Sims2Access.ini`); every line still goes to the events buffer.
  - Balloons (`nav/balloons.cpp`, `game/simantics.cpp`; layouts in `STRUCTS.md` "SimAntics"): the
    hook on the effects primitive (opcode 0x70) sees every balloon and headline as it starts:
    "Carlos piensa: Energía" (the needs sheet), "Carlos dice: Política" (the six interest sheets),
    "Carlos piensa: Fitness" (Free Time's hobby sheets), "Carlos piensa: Silvio" (a Sim's face),
    "grita" for screams (warnings), "no X" for crossed-out icons, and the headlines ("Carlos: la
    relación mejora", "nuevo amigo", "primer beso"...). Names are the game's own (Live.package STR#
    130 needs, 0x8c interests, 0xf2 hobbies), so every language works. The family's are spoken, a
    visitor's only kept in the events buffer; the same line over the same Sim once in 15 s.
    Sheets named so far: needs, the 18 interests, hobbies, aspirations, misc (route failures:
    "Carlos no puede llegar"), star signs, turn-ons, money, pet topics; others (job, professor)
    are skipped. A thought repeats at most once a minute, a conversation topic every 15 s.
  - Furniture leaving the lot in live mode (a burglar, the repo man): "Sofá ... ya no está en el
    solar, valor $150" (`live.cpp` WatchFurniture: catalog objects worth $50 or more, every 2 s;
    buy and build mode start the list over).
  - Camera: in live mode the mod presses the game's Return ("Center on active Sim") when the
    selected Sim has moved four tiles or another Sim is selected (never while a dialog or a text
    field is up), because the game's sounds come from what is on screen. Ctrl+F turns it off/on.
    The mod's posted game keys carry lParam bit 26 so its own handler lets them through.
- Game key bindings: `extracted/accelerators.txt` (from `ui.package`, type 0xA2E3D533). Lot view:
  F1-F5 modes, P/Pause/`/NumPad0 pause, 1-3 speeds, Space/N next Sim, PageUp/PageDown floors,
  Home/End wall mode, Return centers on the Sim, H/E/R/F/J/M build tools, L day/night, Tab/Y camera
  mode, WASD camera, Z/X zoom, C snapshot, Ctrl+Shift+C cheat console. Neighborhood: F1 families,
  F2 lots, F3 decorations, F4 story, F5 options, O content browser, L day/night.

## Languages

- The Steam install script writes Steam's language number to `HKCU\...\The Sims 2 Ultimate
  Collection 25\1.0\language` (1 English, 2 French, 3 German, 4 Italian, 5 Spanish, 6 Swedish,
  7 Finnish, 8 Dutch, 9 Danish, 10 Brazilian Portuguese, 11 Czech, 14 Japanese, 15 Korean,
  16 Russian, 17 Simplified Chinese, 18 Traditional Chinese, 20 Polish, 21 Thai, 22 Norwegian,
  23 Portuguese, 25 Hungarian). `util/language.cpp` maps it to the mod's translation;
  `game::StringLanguageCode` maps it to the STR# language byte (1 English, 3 French, 4 German,
  5 Italian, 6 Spanish, 7 Dutch, 8 Danish, 9 Swedish, 10 Norwegian, 11 Finnish, 13 Russian,
  14 Portuguese, 15 Japanese, 16 Polish, 17/18 Chinese, 19 Thai, 20 Korean, 26 Czech,
  28 Hungarian, 35 Brazilian Portuguese; verified on UIText.package STR# 150).
- **All game text is UTF-8** (STR# resources, window captions, names). The game's font draws
  U+00A7 (§) as the simoleon sign: `util::CleanForSpeech` speaks it as the currency word.
- The mod's strings: English in `mod/core/util/strings_table.h`, translations in
  `mod/core/lang/<language>.cpp` (19 files: the Sims 1 set plus Czech, Hungarian, Russian), written
  and checked by `python tools/mod_strings.py` (`check`, `export <lang>`, `write <lang> <json>`).
  `python tools/restore_strings.py "<group>" Id...` brings back a Sims 1 string with all its
  translations. **Translation debt**: `check` lists what is missing; spoken in English until done.
- The game's own labels the mod needs (buy categories...): `game::GameString(file, instance,
  index)` reads STR# tables straight from the newest pack's `TSData\Res\Text\<file>`.

## RE workflow (all scripts in `tools/`)

1. `python tools/dump_exe.py` (game running): `dumps/Sims2EP9_1.18.0.187_24041f83_dump.exe`
   (SteamStub-decrypted `.text`, relocations undone to base 0xA0000000).
2. `tools\run_ghidra.bat` (about 6 min): Ghidra 12 headless into `re/ghidra/Sims2RE`, `PreAnalysis.java`
   first (**turns off "Non-Returning Functions - Discovered"**: it took the old `_EH_prolog` at
   0xa08248b0 for no-return and truncated every function using it), then `ExportRE.java`
   (`re/functions.tsv`, `re/string_xrefs.tsv`, `re/import_xrefs.tsv`) and `rtti.py`
   (`re/classes.tsv`: 6449 vtables; `re/vtables.tsv`).
3. `python tools/decompile.py <va>...` -> `re/decomp/`; `python tools/disasm.py <va> [n|--func]`;
   `python tools/vtdump.py <class> [this_offset] [n] [slots]` (every slot with its first
   instructions: getters show their field offsets); `python tools/xrefs.py <va>` (direct callers).
4. `python tools/memtool.py "<code>"` / `-f tools/wintree2.py` (the live window tree with procs,
   button flags and tooltips; `WINTREE_ROOT=0x...`). `G(va)` reads a global (already one deref).
5. `python tools/frida_run.py frida/x.js [s] [--drive x.py]`, `tools/trace_class.py`.
6. `python tools/dbpf.py` reads `.package` files (types, list, get, str, ui).
- SimCity 4's open gzcom headers (`re/leads/*.h`, from github.com/nsgomez/gzcom-dll) match this
  game's GZ framework closely (`cIGZWin` slots are SC4's +0..+17 depending on the region; see
  STRUCTS.md). Leads, not facts: every slot used was checked by disassembly.

## Target build

| Item | Value |
|---|---|
| Install | `S:\steam\steamapps\common\The Sims 2 Legacy Collection` (Steam app 3314070); `Base`, `EP1`-`EP9`, `SP1`-`SP7` data folders |
| Game | The Sims 2 + all expansions and stuff packs, 2025 re-release ("Legacy Collection") |
| `EP9\TSBin\Sims2EP9.exe` | FileVersion 1.18.0.187, 18,662,696 bytes, SHA-256 `24041f834e979cc088714713c3207540ff9093f51ff126450a4f7989432156b6`, built 2025-04-07 |
| Bitness | **x86**, MSVC linker 14.40, ASLR, preferred base 0xA0000000, SteamStub (`.bind`, encrypted `.text`) |
| Graphics | Direct3D 9 (system `d3d9.dll`), borderless 2560x1600; the UI is laid out in 1478x924 and scaled |
| Imports of note | `VERSION.dll` (the proxy), `d3d9`, `DSOUND`, `WININET`, `steam_api` |

## Reconnaissance findings

- **Engine**: Maxis' GZ framework (GZCOM: `cIGZUnknown` interfaces, system services by id, a
  message server) shared with SimCity 4; UI from UI scripts in `ui.package` (type 0 resources,
  text `<LEGACY clsid=GZWinBtn ... caption=... captionres={group,index} tiptext="Title|Body">`).
  SimAntics behaviour in object packages, plus **Lua 5.0** (`cGZLua5`) with simulator bindings
  (GetObjectName, GetCatalogName, GetPersonMotive, GetPersonData, RunTree...; the scripts in
  `TSData\Res\ObjectScripts` are compiled Lua).
- **UI**: every window is a `cGZWin` with a child list, area, flags, caption (`cRZString`, UTF-8),
  and optionally a window proc (`+0x12c`, a `cTSWinProc*` that owns the behaviour). Buttons keep a
  tooltip (`Title|Body`) and flags; hidden captions are English design-time names, shown ones are
  values (funds, clock). The window that owns input: the window manager's modal window
  (`wm+0x3c`), else the main window. Pressing = the window's own mouse handlers (enter, left
  down, left up); pie menu entries take a click through the window manager instead (virtual mouse).
- **Cheats**: the cheat console (Ctrl+Shift+C) runs lines through `cTSCheatSystem` (a system
  service); `game::RunCheat` does the same. `help -all` lists them; `loadLot <lotName> [family]`
  exists but is not in the public list.
- **Interaction menus**: `cTSWinPieMenu` (a modal window) with `cPieMenuItemWinProc` entries
  (ids 1000+n in order, each with a `cGZWinText`). Any object's menu opens by posting message 0xC0
  (data 1 = the object id) to the message server: the main toolbar builds it for the selected Sim.
- **Objects**: `cTSGlobals` (`[0xa10cc288]`) holds every manager; its object module has the lot's
  objects by id, names (internal, English), catalog names by GUID (in the game's language),
  positions in tiles (16.16), floors, buy categories (OBJD function flags).
- **Neighborhood**: `cTSNeighborhood` keeps its lots in a map by id (name, type, position);
  families (name, home lot, funds, members) in `cTSFamilyManager`. Lot types: 0 residential,
  1 community, 8 apartment building, 9 apartment unit (families of apartments live in units).
- **Edge scrolling**: the camera scrolls while the (real) cursor rests at a screen edge, even with
  the game behind other windows. **Open question** for blind players: turn it off (option?) or pin
  the cursor the game sees.

## Layout

- `mod/`: source and build script. `tools/`: RE and test scripts. `extracted/`, `dumps/`, `re/`,
  `shots/`: gitignored (copyrighted material, never shipped).
