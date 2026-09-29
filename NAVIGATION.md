# Navigation: the one model every screen follows

The user's rule (2026-09-29): keyboard navigation follows what things **mean**, the way a screen
reader user moves through an application, never how the screen **looks**. Every screen, panel,
dialog and mode of the mod, today and later, uses this model and these keys (the same rule and
keys as the Sims 4 mod, `..\sims4\NAVIGATION.md`). A new feature that navigates anything adds
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
| Ctrl+Enter | walk the selected Sim to the thing last focused (Ctrl+Shift+Enter: run) |
| R | the item again, with its group |
| Ctrl+Up / Ctrl+Down, Ctrl+Left / Ctrl+Right | the review buffers (details, events) |

- Entering a group says its name and size ("objetos por tipo, 11 elementos. Sims, 1 elemento");
  moving inside says the item only; the position ("3 de 12, group") is in the details buffer.
- When the focused item goes away (a thing the Sim picked up), the focus stays in its group
  (the item now at its place). When an interaction menu closes, the focus returns to where it
  was opened from.

## Spatial tools (the only exception)

When the task is a place, the arrows move in space:

| Tool | Arrows | Tab / Shift+Tab | PageUp / PageDown | Other |
|---|---|---|---|---|
| Placing (buy mode, `nav/placement.cpp`) | one tile, Shift five | the spots where it fits | the rooms with spots | comma / period turn (the game's), Home back to the Sim, Enter puts it down, Delete sells, Escape stops |
| Build cursor (`nav/build.cpp`) | one tile, Shift five | (none yet) | the level above / below | Space marks, Enter builds, Ctrl+Enter removes, Home back to the Sim, Escape |
