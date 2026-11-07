# Navigation: the one model every screen follows

The user's rule (2026-09-29): keyboard navigation follows what things **mean**, the way a screen
reader user moves through an application, never how the screen **looks**. Every screen, panel,
dialog and mode of the mod, today and later, uses this model and these keys (the same rule and
keys as the Sims 1 mod, `..\sims\NAVIGATION.md`; the Sims 4 mod is someone else's now). A new
feature that navigates anything adds
groups or items to this model; it never invents its own keys for moving.

## The model

- What the player moves through is a ring of **groups** (sections): a panel of the game (the
  control panel, the Sim panel, a dialog's buttons), a list the mod builds (needs, the action
  queue, the lots, the lot's things by kind and by room, an interaction menu).
- A group holds **items**. An item may open a deeper **level**: a kind or a room of the lot's
  things opens its things (`Role::Level`, `nav/world.cpp`).
- What belongs together comes from the game's own structure: the nearest panel (a window with a
  proc) or container around a control (`collect.cpp` `GroupOf`), or the mod's own data.
  **Groups keep the window tree's order** (`Walker::order`); screen position never decides what
  belongs together nor the order of groups. Position only orders the items inside one group (a
  panel's buttons in rows, `TopToBottom`; a column of values before the next, `ColumnsFirst`).
- The mod's own groups come first where they matter: the lot view's needs, action queue, things
  by kind, things by room; the neighborhood's lots.

## The keys

| Key | What it does |
|---|---|
| Tab / Shift+Tab | next / previous group (wraps); says the group's name and size, then the item |
| Up / Down | previous / next item of the group; no wrap (the end item is said again) |
| Left / Right | the same as Up / Down; on a slider, its value; in a text field, the caret |
| Home / End | first / last item of the group |
| Enter | press; on a kind or a room, open its things; on a thing, its interactions (buy mode: pick it up) |
| Backspace | back up a level (a kind's or room's things to the kinds or rooms); a text field's is the game's |
| PageDown / PageUp (lot view, every mode) | next / previous thing of the kind or room chosen last, from anywhere in the lot view; focus goes to it in its group (the game's own floor keys: the control panel's floor buttons still change floor) |
| Shift+PageDown / Shift+PageUp | next / previous kind of things, on its nearest thing |
| Ctrl+PageDown / Ctrl+PageUp | next / previous room (the first press: the selected Sim's), on its nearest thing |
| Ctrl+Enter | walk the selected Sim to the thing last focused (Ctrl+Shift+Enter: run) |
| Ctrl+F (lot view, every mode) | find a thing by typing part of its name: the matches are a list of "things by kind", best first; Up / Down (Page keys, Home / End) go through them, Enter acts, Ctrl+Enter walks, Delete removes a door or window, Tab leaves with the list kept, Escape goes back to where the focus was |
| Delete (buy mode, build mode's hand) | on a door or window of the lot's things: picks it up and sells it |
| R | the item again, with its group |
| Alt+letter | the next group whose spoken name starts with that letter (so the letters follow the game's language) |
| Ctrl+Up / Ctrl+Down, Ctrl+Left / Ctrl+Right | the review buffers (details, events) |
| Ctrl+F1 / Ctrl+Shift+F1 | this screen's keys / the manual; both are also the buttons of the main screen's last group, "mod help" |

- Entering a group says its name and size ("objetos por tipo, 11 elementos. Sims, 1 elemento");
  moving inside says the item only; the position ("3 de 12, group") is in the details buffer.
- **The lot's things on the Page keys** (the user's rule, 2026-10-29: "there is an overuse of
  arrow keys", the same in the Sims 1 mod): PageDown/PageUp walk the list of things, Shift+ and
  Ctrl+ choose what it holds (a kind, a room). They drive the same "objetos por tipo / por
  habitación" groups (`nav::BrowseThings`), so Tab, the arrows, Enter and Backspace go on from
  there. Any later list of things on the lot follows this: Page for the items, a modifier for
  the filter.
- **Find owns the keyboard while it is on** (the user's request in the Sims 1 mod, 2026-11-02): what
  types goes to the search, so letters reach neither the game's hotkeys nor R. Its results are
  ordinary items of the kinds group, so everything after it (Page keys, Backspace, Tab) goes on as
  usual.
- **A build tool leads to its styles**: the build panel and the catalog are two groups (by their
  procs); pressing a tool that changes the catalog moves the focus to its first style, in the
  catalog group named after the tool (`nav::FinishToolPress`); Tab goes back to the tools. Tools
  that change no style (day and night, undo) leave the focus alone.
- When the focused item goes away (a thing the Sim picked up), the focus stays in its group
  (the item now at its place). When an interaction menu closes, the focus returns to where it
  was opened from.

## Spatial tools (the only exception)

When the task is a place, the arrows move in space:

| Tool | Arrows | PageDown / PageUp | Ctrl+PageDown / PageUp | Shift+PageDown / PageUp | Tab / Shift+Tab | Other |
|---|---|---|---|---|---|---|
| Placing (buy mode, `nav/placement.cpp`) | one tile, Shift five; walls, fences and windows stop it, Alt+arrows go through | the spots where it fits (a seat: where something is used sitting first; a moved object: nearest where it stood) | the rooms with spots | turn it (the game's period / comma) | the spots too | Ctrl+Shift+PageDown/Up beside the lot's things, comma / period turn (the game's), Home back to the Sim, Enter puts it down, Delete sells, Escape stops |
| Build cursor (`nav/build.cpp`) | one tile, Shift five | the level above / below | | | (none yet) | Space marks, Enter builds, Ctrl+Enter removes, Home back to the Sim, Escape |
| Lot map (`nav/map.cpp`, Ctrl+M) | one tile, Shift five; walls, fences and windows stop it, Alt+arrows go through | the lot's things (the map goes to each) | the rooms, as the lot's things | the kinds, as the lot's things | the groups as usual | comma / period the layer each step says (everything, objects, floors, walls, rooms), Ctrl+Shift+PageUp/Down the floors, Enter the menu of what stands there, Ctrl+Enter walks there, Home back to the Sim, R, Escape or Ctrl+M leaves |

A spatial tool that walks the house stops at walls (a Sims 1 tester's "Zones" idea, 2026-11-06: a
room was left by accident); Alt with the arrows goes through, since Ctrl+arrows are the review
buffers everywhere. The build cursor keeps crossing walls: walls are its work. Placing's comma /
period stay the game's turn; the map's are its layers (from Oxygen Not Included's overlays: one kind
of thing per step on a furnished lot).

Every spatial tool also plays the sound cues of `nav/cues.h` on each step (Ctrl+Shift+S turns them
off): the floor, each side's wall, fence, railing, door or window, stairs, what stands on the tile.
