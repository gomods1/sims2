# The Sims 2 Legacy Collection: screen reader accessibility mod

Playbook: `template_generic.md` (read it first; its rules apply). This mod is a port of the Sims 1
accessibility mod (`C:\Users\Nitropc\code\mods\sims`, same author, same architecture); its tools and
generic code were copied and retargeted, its game layer rewritten. A copy of the Sims 1 core is in
`re/sims1_core_ref` (gitignored) for reference and for `tools/restore_strings.py`. This file holds
what has been learned about *this* game. Anything marked **hypothesis** has not been verified.

**Navigation follows meaning, not the screen's layout (the user's rule, 2026-09-29): read
`NAVIGATION.md` before touching any key.** A ring of groups (Tab), items (arrows), levels (Enter /
Backspace); groups in the window tree's order, never sorted by position; one key grammar for every
screen and mode; only the spatial tools (placing, the build cursor) use the arrows for space.
Since 2026-10-29 (the user: "an overuse of arrow keys", same as the Sims 1 mod) the lot's things
are on PageDown/PageUp in every mode, Shift+ the kinds, Ctrl+ the rooms, as placing's spots are.

Status (2026-09-23): the mod runs (`mod/`). Speech through NVDA (Prism in `SpeechHost.exe`).
Keyboard navigation covers every GZ-window screen and dialog generically (menus, neighborhood
chooser, dialogs, tutorials, game tips, the neighborhood panel, the lot info panel, the live mode
HUD, the Sim panel's pages read column by column), the neighborhood's lots as a list (moving
families in), Create a Family / Sim / Pet (with the family tree by keyboard), interaction (pie)
menus, the lot object browser (PageUp/PageDown by category or room, Enter the interactions,
Ctrl+Enter walks there), live state and a narrator (actions, needs, visitors, money, furniture
leaving, the balloons and relationship headlines named from the game's own texts), buy mode
(placing, moving and selling objects through the spots where they fit, in any direction), build
mode (walls, floors, pools, foundations, doors, windows, stairs, auto roofs), travel and saving.
Since 2026-11-01 (the Sims 1 mod's commits up to v0.2.0 ported): Alt+letter groups, the focus
lit on screen for a helper, Enter speaks a read-only item's hover help, text fields take the
keyboard when focused and speak the caret, placing says its size, what it runs into and faces and
goes beside the lot's things, the lot map (Ctrl+M), sound cues for every cursor from the lot's real
build data (floors by material, walls, fences, railings, doors, windows, stairs, slopes), the
player's manual in 20 languages (Ctrl+Shift+F1, a "mod help" group on the main screen), release
packaging. Multi-tile furniture (beds, sofas, tables) is listed now (it was missing).
Since 2026-11-02 (the Sims 1 mod's commit of that day ported): Ctrl+F finds a thing by name, the
kinds "problems" and "doors, windows and stairs", Delete on a door or window, the check after
placing a door, the camera's zoom and facing, a tool leading to its styles, actions naming the
other Sim.
Since 2026-11-07 (the Sims 1 mod's commits of 2026-11-06 ported, each through its Sims 2
equivalent, found by RE and checked live on the Contreras lot): the map's layers and floor names,
walls stopping the map's and placing's arrows (Alt goes through), reach ("no Sim can reach it"),
seats for what is used sitting (routing slots), Ctrl+T hours, Ctrl+Shift+N conversation scope,
Ctrl+Shift+E a Sim's looks, interrupted actions, skills and promotion needs, the camera and pointer
on the focused thing (Ctrl+Shift+V), the GitHub release workflow.
Since 2026-11-08 (Sims 2 tester feedback, each found by RE and checked live on the Contreras lot
unless said): balloons look their icon sheet up as the game does (the object's own group, its
semi-global, then the global one: the food objects' "foodchainicons" sheet was read as the needs
sheet, so "a counter is needed" was spoken as "hygiene"; the counter case itself not seen live),
"$Subject" in a memory's name is "someone" (not seen live); a multi-tile object placed since the
lot loaded is listed (its lead's +0x2f4 is 0 until the lot loads again: the bought bed); the
options panel's sliders (`cGZWinScrollbar2`: Left/Right, the game's own SetValue with notify) and
choices named by the heading above ("Sombras: Alto"), the mute buttons by their row, a station's
songs as one group; turning a held object no longer speaks the camera at every compass point (the
game turns the camera with it), and says what it faces further off ("hacia Sofá, a 3 casillas");
NPC roles from person data 155 ("Ravi, cartero"; the numbers confirmed in the game's own NPC
generator and template behaviors, STRUCTS.md "People"), a visitor waiting at the door to be
greeted (greet status 34 = 0 beside a door), wants fulfilled and fears come true (reward points
161 x10, the wants panel's tooltips), the aspiration meter in the Sim panel, pet bowls' food
(semi attribute 0, empty ones among the problems), the taxi and cars at the curb in the lists,
the lots' size in the neighborhood, the camera gliding to the focused thing. The music volume
set through the mod was saved (userProps.xml MusicVolume 35 after quitting).
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
| `mod/speechhost/` | `Sims2Access\SpeechHost.exe` (x64) + `prism.dll` | The Sims 1 mod's (pipe `Sims2AccessSpeech-<pid>`): `say`, `stop`, and `play <name> <pan> <pitch> <volume> <delay>` for the sound cues (`Sims2Access\sounds\<name>.wav` through XAudio2). It caches the files: after changing one, kill `SpeechHost.exe` (the core relaunches it). |

- The game folder is `S:\steam\steamapps\common\The Sims 2 Legacy Collection\EP9\TSBin` (the one exe
  runs the base game and every expansion). Build: `mod\build.bat` (MSVC 2022, then the manual through
  pandoc); `mod\build.bat deploy` also copies `sounds\*.wav` and `docs\*.html`.
- **Keys and docs never drift** (the user's rule in the Sims 1 mod, 2026-11-01): a change to a key or
  a feature edits, in the same change, the code, the `Keys*` strings Ctrl+F1 speaks (English and all
  19 languages), the manual `docs/manual/en.md` and its 19 translations, and `NAVIGATION.md` when the
  grammar changes. `python tools/build_docs.py --check` compares each translation with en.md
  (headings, table rows, list items, the key combinations of each row: an ASCII "+" joins keys).
- **Release**: `python tools/package.py vX.Y.Z` checks the manual, builds, writes
  `mod\out\Sims2Access-vX.Y.Z.zip` (+ `.map`); the shared installer (`..\installer`) has no Sims 2
  entry yet (ROADMAP). Pushing a tag (`git tag vX.Y.Z && git push origin vX.Y.Z`) runs
  `.github/workflows/release.yml` (ported from the Sims 1 mod, 2026-11-07): a Windows runner
  downloads Prism (`PRISM_VERSION`, `PRISM` for `build.bat` and `package.py`), finds MSVC through
  vswhere (`build.bat`, when `VCVARS` is not there), builds and publishes the zip and its map;
  Actions > Release > Run workflow builds without releasing. Not run yet (the repo is private).
- **Dev loop** (`Sims2Access\dev.txt`): the loader hot-reloads `Sims2Access.dll` within a second of a
  rebuild. Dev builds also take requests from files: `tools/game.py shot <name>` (screenshot),
  `tools/game.py cheat "<line>"` (a cheat console line through the game's cheat system) and
  `tools/game.py cheat "!<command> args"` (mod test commands in `game/cheats.cpp`: `!objects` logs
  the lot's objects, `!pie <id>` opens an object's interaction menu, `!lotinfo <lot> <family> 0`,
  `!click x y`, `!move x y`, `!release`, `!rooms`, `!check x y n` times n placement checks, `!doors`
  logs the check after placing a door for every door of the lot; the lot
  grid: `!probe x y level` floor, room, water, heights and each side's wall layers, `!walls level`
  every edge by its layer signature, `!parts x y level` what stands on a tile with its lead part,
  `!water x y level`, `!world` the world database's fields, `!dump <addr> <n>`).
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
  - NAVIGATION.md: Tab/Shift+Tab groups (said with name and size on entering, even of one
    item), arrows items, Home/End, Enter presses (a kind or room opens its things), Backspace back
    a level, R repeats, Ctrl+F1 the key help, Ctrl+Shift+F1 the manual. Ctrl+Up/Down read the review buffers, Ctrl+Left/Right
    switch them. Groups keep the window tree's order (the pixel sort of groups was removed
    2026-09-29).
  - Lot view: the groups needs, action queue, "objetos por tipo" (Sims, then the buy mode
    categories with the game's names, then other objects) and "objetos por habitación" (the
    Sim's room first) come first (`nav/world.cpp` KindsGroup/RoomsGroup, keys kKindsGroup /
    kRoomsGroup); Enter on a kind or room lists its things nearest the selected Sim first,
    Backspace goes back to it; Enter on a thing opens its interaction menu (in buy mode: picks it
    up to move it); after the menu closes the focus is back on the thing. From anywhere in the
    lot view (live, buy, build) PageDown/PageUp go through the things of the kind or room chosen
    last, Shift+ the kinds, Ctrl+ the rooms (`nav::BrowseThings`, since 2026-10-29, verified
    2026-11-01); the game's own PageUp/PageDown (floors) are taken again, the control panel's floor
    buttons still change floor. Ctrl+E the selected Sim's state, Ctrl+1..9 select a
    family member. Verified 2026-09-29 (Contreras lot: kinds, rooms, Backspace, the menu,
    Ctrl+Enter walking).
  - Buy mode (`nav/placement.cpp`): Enter on a catalog product picks it up; then PageDown/PageUp
    (or Tab) the spots where it fits (nearest the Sim first; a scan of the whole lot through the tool's own
    check, about 0.1 ms a tile), Ctrl+PageDown/PageUp by room, arrows move a tile (Shift five),
    Home back to the Sim, `,` `.` or Shift+PageDown/PageUp turn it (the game's keys; the facing is said), Enter buys (the
    game hands over another copy until Escape), Delete sells a picked-up object, Escape drops it
    or puts it back. The arrows' line: fits or not, what it runs into, the room when it changes
    ("sales de X, entras en Y"), the walls around it, its facing, what it faces ("hacia ..."), the
    tile; a multi-tile object's size is said first. Ctrl+Shift+PageDown/PageUp put it beside the
    lot's things nearest first, in their room, facing them when it fits (verified 2026-11-01: a
    chair beside a patio table). How it works: the tools' ground pick is hooked to answer with the mod's tile,
    the "cursor left the view" handler is skipped, the virtual mouse sits in the 3D view and
    moves a pixel to make the tool re-pick; picking up an object hooks the object pick to answer
    with that object's scene node (STRUCTS.md "Buy mode").
  - A text field the player can edit takes letters, Space and Backspace (each echoed); Enter,
    Home/End and Left/Right go to it only while the mod's focus is on the field (Create a Family
    keeps its surname field focused all along). A field's name is the prompt it first showed.
    Focusing a field gives it the keyboard (`game::SetKeyboardFocus`, the window manager's
    GZSetFocus, slot 38; Enter does the same, a click only when that is refused); Left/Right/Home/End
    then say the character at the caret (`edit::kCaret`, the +0xf8 interface's insertion index),
    Delete the one it removes (2026-11-01, not yet tried on a name field).
  - Alt+letter (any screen): the next group whose spoken name starts with that letter (accents
    folded; a Cyrillic name by its key on the Russian layout); WM_SYSKEYDOWN, its WM_SYSCHAR
    swallowed; verified (Alt+S solares, Alt+A ayuda del mod). Ctrl+Shift+V: the focused control
    is lit on screen for a sighted helper (its mouse-enter slot 161; `show_focus` in the ini).
    Enter on a read-only item with a tooltip description (a skill, an interest, a job field)
    speaks it. The main screen ends with the group "mod help" (`kModHelpGroup`, `Role::ModManual`):
    "keys for this screen" (Ctrl+F1's) and the manual (Ctrl+Shift+F1, `nav/manual.cpp`, opens
    `Sims2Access\docs\<tag>.html`, `util::DocTag`).
  - Ctrl+F (lot view): find a thing by typing part of its name, accents and case aside (`tel`:
    teléfono; `nav::Find*`, `world::SetFindQuery`, `util::FoldForSearch`). Names starting with the
    text come first, then a word starting with it, then containing it, each nearest first. Moved
    from the camera's following on 2026-11-02 (that is Ctrl+Shift+F now).
  - Things kinds (`nav/world.cpp`): **problems** right after Sims (object variable 15 above 0 broken;
    39 from 200 dirty, garden plants left out because it is also how overgrown they are; messes by
    internal name, `MessName`, since they have no catalog name) and **doors, windows and stairs**
    last (OBJD types 8 and 9, "Stair..." by name).
  - Delete on a door or window of the lot's things (`world::RemoveFocusedPortal`,
    `placement::BeginRemove`): the hand picks it up and the game's Delete sells it, refunded; only
    in buy mode or with build mode's hand. Unlike the Sims 1 game the hand takes doors and windows,
    so Enter on one moves it (verified 2026-11-02 in buy mode; build mode's hand now takes things too).
    **A minimized game refuses every pick-up silently**: `tools/game.py windows` shows it at -32000.
  - After a door is placed the mod says whether Sims can use it (`CheckDoor` in `nav/placement.cpp`:
    what stands on either side, then a flood from it to the lot's edge, or on an upper floor to the
    stairs; `!doors` gave no false alarm on the Contreras lot's 15 doors).
  - The camera's zoom and facing are said when they change and are an item of the control panel
    (`live.cpp` WatchView, `game::CameraView`). Zoom is one of 6 steps (keys Z, X), its word a third
    of them; facing is the compass direction the view looks toward. Posted zoom keys:
    `tools/keys.py z`. Facing changes were not heard in the game: the panel's Rotar buttons do
    nothing to a press (they may want holding).
  - A build tool's press moves the focus to its styles (`nav::FinishToolPress`, NAVIGATION.md).
  - Actions with another Sim name it everywhere the queue is read (`live::QueueGroup`, the queue icon's
    interaction's object; the field verified on an action with an object, not yet with a Sim).
  - Lot map (Ctrl+M, `nav/map.cpp`, verified 2026-11-01 on the Contreras lot): a tile cursor from
    the selected Sim; arrows (Shift five) say what stands there and which way it faces, the room
    ("sales de Exterior, entras en Cocina") and the floor when they change, walls, fences, railings,
    doors and windows by side, stairs, slopes; Home, Ctrl+Shift+PageUp/Down floors, Enter the menu
    of what stands there, Ctrl+Enter walks there, R, Ctrl+L that room, Escape or Ctrl+M leaves. The
    Page keys still browse the lot's things and the map follows them.
  - Sound cues (`nav/cues.cpp`, the Sims 1 mod's sounds in `mod/sounds`): the build cursor,
    placing's arrows and the map play the floor (by its catalog material), each side's wall, fence,
    railing, door or window (panned and pitched by side), stairs, a Sim or object, a slope, a room
    change, the lot's edge, fits / does not fit. Ctrl+Shift+S off/on (`[mod] cues`, `cue_volume`).
    Read from the world database (`game/lotgrid.cpp`, STRUCTS.md "The lot grid").
  - Sliders (Create a Sim's personality): Left/Right change them, with the points left.
  - Neighborhood: choose a family in the family bin (F1 panel), then Enter on a lot in the lots
    list: the game asks whether to move the family in and at what cost.
  - Build mode (`nav/build.cpp`): Enter on a wall, floor, foundation or pool style starts a cursor;
    arrows move it (Shift five), PageUp/PageDown change level (0 terrain, 1 a foundation's floor),
    Space marks the start, Enter builds from the mark to the cursor, Ctrl+Enter removes; the cost is
    said. The mod drags the game's own tool, one mouse message per tick. After a wall is built, a
    family Sim indoors in a room without a door is announced ("cuidado: Baño no tiene puerta y Carmen
    está dentro..."; dev command `!shutin` runs the check).
  - Ctrl+Enter: walk the selected Sim to the thing last focused (Ctrl+Shift+Enter runs).
  - Ported 2026-11-07 from the Sims 1 mod's tester feedback (keys in `docs/manual/en.md`; the RE in
    STRUCTS.md "Routing slots", "Looks", "Job panel", "Camera"; verified live on the Contreras lot
    unless said): the map's comma / period layers (everything, objects, floors, walls, rooms;
    floors by catalog name, `game::FloorName`: the XFLR's STR# `stringsetgroupid`/`stringsetid`
    in `catpatterns.bundle.package`, "Pavimento de piedra Lecho de río"); walls, fences, railings
    and windows stop the map's and placing's arrows (`cues::WallStops`), Alt+arrows go through
    (`OnSysKeyDown`); the map's Enter takes what stands on a surface before it. Reach
    (`nav/reach.cpp`, the door check's flood from the lot's edge or the stairs, now shared):
    "no Sim can reach it" in the lists and after placing, else "Sims can reach it" (0 false
    alarms on 238 things; a real unreachable thing not yet seen); use tiles are the object's
    routing slots (`game::UseSpots`, a desk's eight around it, the fridge's in front), else the
    tiles beside it. Seats: a thing used sitting is electronics, appliances or misc on a desk or a
    table (`+0xaa`, the container's OBJD field 94 sub-sort: a microwave on a counter was offered a
    chair before that); its seat tile is the container's sitting slot, else its front spot (desks
    flag none); a held one-tile comfort object's spots serving one come first, turned to face it
    (verified: the desk chair picked up, "mirando al oeste, para usar Ordenador Espectrus sentado,
    1 de 837", put back: "colocado. para usar Ordenador Espectrus sentado"); a placed computer
    says "se usa desde Silla de oficina" (`!reach`). A moved object's
    spots are nearest where it stood. Ctrl+T each hour ("son las 8:02", `[mod] hours`).
    Ctrl+Shift+N whose conversations (`[mod] social`; balloons with the tree's runner or stack
    object in the family count as with the family: the stack object read not yet seen in a
    conversation with a visitor). Interrupted actions: the one that ran waits in the queue behind
    the new one (not yet heard). Names starting with "*" are no names. Ctrl+Shift+E a Sim's looks
    in the game's words ("Carlos Contreras: Anciano, Hombre, Cabello canoso, Tono de piel: moreno,
    Color de ojos: Castaño, Sombreros, en forma, Ropa: Cotidiano"; his cap is the hat), also the
    Sim's details. Job panel: each skill label takes the bar it overlaps most (they overlap two;
    the values matched the bars 7/1/6/9/2/4/1), and the promotion mark (bar child 5) gives "the next
    promotion needs N" (not yet seen: Carlos is at his career's top). Ctrl+Shift+V also centres the
    camera on the focused lot thing (`game::CenterCamera`, verified) and moves the real pointer to
    the window's middle (live mode, game in front: not tried here, the user works with the mouse).
    Dev commands: `!slots <id>`, `!look <id>`, `!reach`, `!camera x y [jump]`, `!objd <id>` (the whole object's
    OBJD fields), `!pd <index>` (that person data of every Sim on the lot), `!lots` (the
    neighborhood's lots with their size), `!msg <type hex> <data>` (a message to the game's
    message server; careful: "rotate object" 0xacb3a629 also spins the camera until stopped).
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
    field is up), because the game's sounds come from what is on screen. Ctrl+Shift+F turns it off/on.
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
- **Multi-tile objects** (beds, sofas, tables, wide windows, doors) are only their parts
  (`cEdithMTObject`): each points at its lead part ("- 0,0") through +0x2f4 (the lead's interface at
  +0x2f0); the whole object's definition (catalog name, price, categories) is the part selector's
  master's (+0x240 -> +0x68 -> +0x60). Before 2026-11-01 the mod skipped every part, so such
  furniture was missing from the lists.
- **The lot's build data**: the world database (`nTSWorld::cWorldDB`, the service the mod already
  used for rooms) holds the floor patterns per quarter tile, their names (catalog GUIDs in hex for
  bought floors; the floors' "surfacetype" and "subsort" come from the patterns packages' XFLR
  property sets), the walls by partition layer, terrain heights; found through the Lua bindings'
  table at 0xa1016700 (STRUCTS.md "The lot grid").
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
