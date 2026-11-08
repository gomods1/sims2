---
title: Sims 2 Accessibility manual
lang: en
toc-title: Contents
---

Sims 2 Accessibility makes The Sims 2 Legacy Collection playable with a screen reader and the
keyboard alone. It speaks every screen, dialog and panel through your screen reader (NVDA, JAWS,
Narrator and others), lets you move through them with the same keys everywhere, and adds tools a
blind player needs: a list of the lot's things, a map of the lot, a narrator for what the game
only shows, spots where furniture fits, and a cursor for building.

This manual is opened by **Control+Shift+F1** in the game, in the game's language. **Control+F1**
says the keys of the screen you are on.

## Getting started

1. Copy `version.dll` and the `Sims2Access` folder into the game's `EP9\TSBin` folder (the one
   with `Sims2EP9.exe`), inside `The Sims 2 Legacy Collection` in your Steam library.
2. Start your screen reader, then start The Sims 2 Legacy Collection from Steam.
3. When the game window opens you hear "Sims 2 accessibility loaded", and where the help is.

The mod speaks in the game's language. To change it, open Steam, then the game's Properties,
General, Language. The mod speaks English, French, German, Italian, Spanish, Dutch, Danish,
Swedish, Norwegian, Finnish, Portuguese, Japanese, Polish, Simplified and Traditional Chinese,
Thai, Korean, Czech, Hungarian and Russian.

The mod works with the current Steam version of the game. If the game updates and the mod says
"unsupported game version", install the mod's next version.

## Moving around: one set of keys everywhere

Every screen, dialog, panel and mode works the same way. What you move through is a ring of
**groups** (a dialog's buttons, the needs, the family, the catalog...). A group holds **items**,
and some items open a deeper **level**.

| Key | What it does |
|---|---|
| Tab, Shift+Tab | Next or previous group. The group's name and number of items are said first. |
| Up, Down (or Left, Right) | Previous or next item in the group. |
| Home, End | First or last item in the group. |
| Enter | Press the item. On a level (a kind or a room of the lot's things) open it. |
| Backspace | Back up a level. |
| R | Say the item again, with its group. |
| Alt+letter | The next group whose name starts with that letter. |
| Control+F1 | The keys of the screen you are on. |
| Control+Shift+F1 | This manual. |

Controls say what they are after their name, as a screen reader does: "Accept, button",
"Options, button, selected", "Shy to Outgoing, slider, 6 of 10".

- **Sliders** (Create a Sim's personality): Left and Right change the value and say the points
  left.
- **Text fields** (family and Sim names, the cheat console): arrow onto the field and type: it
  gets the keyboard as soon as the focus is on it. Each letter is echoed, and Backspace says what
  it deletes. Left, Right, Home and End move in the field and say the character there; Delete
  says the character it removes.
- **Read-only items** that the game explains on hover (a skill, an interest, a job field): Enter
  speaks the explanation.
- **Disabled controls** say "unavailable" and are not pressed.

The last group of the main screen is **mod help**: its buttons say the keys of that screen (as
Control+F1 does) and open this manual (as Control+Shift+F1 does).

## Reading more: the review buffers

Not everything fits in one spoken line. The mod keeps two buffers you can read line by line:

- **details**: everything about what was just spoken: a product's description, a control's
  tooltip, a spot's tile and facing, a thing's price, and its place in its group ("3 of 12").
- **events**: what happened: purchases, sales, dialog texts, everything the narrator said.

| Key | What it does |
|---|---|
| Control+Up | The next line, deeper into the buffer. |
| Control+Down | Back toward the top. |
| Control+Left, Control+Right | Switch between the buffers. |

## The neighborhood

The neighborhood's first group is **lots**: each says its name, its kind, who lives there
(apartments with their tenants) and its size ("40 by 30 tiles"). Enter on a lot opens the game's lot panel.

To move a family in, choose the family in the family bin (the families panel), then press Enter
on a lot: the game asks whether to move them in and what it costs.

## Creating a family and Sims

Create a Family, Create a Sim and Create a Pet are groups like any screen:

- Type the family name and each Sim's names in their text fields.
- Age, sex, skin, body and the grids of heads, hair and clothes ("3 of 6"; pet breeds by name).
- **Personality**: Left and Right add or take a point from each pair of traits ("6 of 10, 4
  points left").
- Aspiration, star sign and turn-ons are buttons.
- **The family tree** (Family relationships): Enter on a Sim picks it up, Enter on another Sim
  joins them (the game then asks how they are related), Enter on the same Sim puts it down.

## The lot: live mode

On a lot the screen is a ring of groups like any other. Tab goes through them in this order:

1. **needs** of the selected Sim, each with its level.
2. **action queue**: what the selected Sim will do; Enter cancels an action.
3. **things by kind** and **things by room**: the lot's things (see below).
4. The game's panels: the control panel, the Sim panel (needs, relationships, job, skills,
   personality, interests...), the family, wants and fears, and any panel you open.
5. **mod help**.

The Sim panel says the Sim's aspiration ("Aspiration: 24 percent"); the rewards panel (Rewards, then Aspiration rewards) says the aspiration points to spend.

The game's options (the control panel's Options button) are read the same way: each choice says its setting ("Shadows: High") and the sliders, such as the music volume, change with Left and Right ("Music, slider, 35 of 100"). A radio station's songs are a group of their own; Enter switches a song on or off.

The job page says the Sim's hours; Enter on them adds when the carpool comes. To quit a job, call work on the phone. Each skill says its level and, when the next promotion needs more, how much: "Cooking, 2 of 10, the next promotion needs 4".

### Selecting Sims and the camera

| Key | What it does |
|---|---|
| Control+1 to Control+9 | Select the first to ninth Sim of the family. |
| Control+E | The selected Sim in one line: where, what it is doing, urgent needs. |
| Control+Shift+E | What a Sim looks like: age, hair, skin, eyes, glasses, body and outfit, in the game's words. The Sim focused among the lot's things, else the selected one. A Sim's details (Control+Up on it) say it too. |
| Control+T | Say each new hour of the clock ("it is 14:00"), or stop. Useful at the fastest speed. |
| Control+Shift+F | The camera follows the selected Sim, or stays free. |

The camera follows the selected Sim, because the game's sounds come from what is on screen. When the focus is shown on screen (Control+Shift+V), the camera glides to each thing you browse rather than jumping.

The zoom and the way the camera faces are said when they change (Z and X zoom), and the control panel has them too: "camera: close view, 5 of 6, facing north-east". The facing is what the top of the screen looks toward.

### The lot's things

Everything on the lot is in two groups: **things by kind** (Sims, then the buy mode categories:
seating, surfaces, appliances..., then other) and **things by room** (the selected Sim's room
first, then the others, each with its floor). Multi-tile furniture (beds, sofas, tables) counts
once. Enter on a kind or a room lists its things,
nearest first, as "Fridge, kitchen, 6 tiles north-east" from the selected Sim. Backspace goes
back to the kinds or rooms. Room names are the mod's guess from what is in them.

You do not need to Tab there: the Page keys browse the things from anywhere on the lot, in live,
buy and build mode.

| Key | What it does |
|---|---|
| Page Down, Page Up | Next or previous thing of the kind or room chosen last. |
| Shift+Page Down, Shift+Page Up | Next or previous kind of things. |
| Control+Page Down, Control+Page Up | Next or previous room (the first press: the selected Sim's room). |
| Enter | The thing's interactions (in buy mode: pick it up to move it). |
| Control+Enter | Walk the selected Sim to the thing. |
| Control+Shift+Enter | Run there. |
| Control+F | Find a thing: type part of its name ("tel" finds the telephone, accents do not matter). Up and Down go through what matches, Enter acts on it, Control+Enter walks there, Delete removes a door or window, Escape goes back to where you were. |
| Delete | On a door or window, in buy mode or with build mode's hand: picks it up and sells it, refunded. |

Sims with a job on the lot say it: "Ravi Pérez, mail carrier", the waiter, the chef, the gardener, the maid. A pet's food bowl says whether it has food. The taxi and other cars at the curb are among the other things while they wait.

After the Sims comes **problems**: what wants attention, such as broken things, things that need cleaning, an empty pet bowl, puddles, rubbish, ashes and fire. Last comes **doors, windows and stairs**. A thing no Sim can get to from the lot's entrance says "no Sim can reach it".

### Interaction menus

Enter on a thing (or on a Sim) opens the game's interaction menu. Up and Down choose, Enter does
it. Entries that open more choices open a deeper menu. When the menu closes, the focus is back on
the thing.

### Rooms

| Key | What it does |
|---|---|
| Control+L | The room's layout (at the build cursor or the map cursor, else where the selected Sim stands): name, floor, tiles, bounds, doors and windows; each door and window, with its wall and the room behind it, is in the details buffer. |

### The lot map

**Control+M** turns on the map: a cursor that walks the lot tile by tile, starting at the
selected Sim. Each step plays a sound for the floor and for each wall, fence, railing, door or
window around it.

| Key | What it does |
|---|---|
| Arrows | One tile; with Shift, five. Says what stands there and which way it faces, the room and the floor when they change, the walls, fences, railings, doors and windows by side, stairs and slopes. Walls, fences and windows stop the cursor; doors let it through. |
| Alt+Arrows | The same, through walls. |
| Comma, period | What each step says: everything, objects only, floors only (by name, "Light granite tile"), walls only, rooms only. |
| Home | Back to the selected Sim. |
| Control+Shift+Page Up, Page Down | The floor above or below. |
| Enter | The interaction menu of what stands there. A thing on a table or desk comes before the table: the computer, not its desk. |
| Control+Enter | Walk the selected Sim there (Control+Shift+Enter: run). |
| Page Down, Page Up (and Shift, Control) | The lot's things as always; the map goes to each one. |
| R | Everything about the tile. Control+L the room's layout. |
| Escape or Control+M | Leave the map. |

### The narrator

The game shows many things only as pictures. The mod says them as they happen and keeps them in
the events buffer:

- **Balloons** over Sims: "Carlos thinks: Energy", "Carlos says: Politics", a crossed-out icon
  ("no ..."), screams, and the relationship headlines ("the relationship improves", "new
  friend", "first kiss").
- **What a meal lacks**: "Carlos thinks: a counter is needed" (an oven, a stove, a grill, a microwave).
- **Routes**: "Carlos cannot get there".
- **Visitors at the door**: "Elvira is at the door waiting to be greeted"; greet her (her interaction menu) to let her in.
- **Wants and fears**: "Carlos fulfilled a want: Go swimming, 500 aspiration points", "Carlos: a fear came true: ...".
- **Actions** of the family, **needs** falling low, **visitors**, and money paid or received. When an action is interrupted and waits in the queue, that is said: "Carlos stopped Sit for Panic; it waits in the queue".
- **Furniture leaving the lot** (a burglar, the repo man), with its value.

Your family's thoughts and screams are spoken, and anyone's screams. Conversations (what Sims say and the relationship headlines) are spoken as Control+Shift+N chooses; the rest only goes to the events buffer.

**Control+N** changes how much is spoken: everything, warnings only, or off. Lines not spoken
still go to the events buffer.

**Control+Shift+N** chooses whose conversations are spoken: anyone talking with your family (the start), everyone on the lot, only your family, or nobody. Lines not spoken still go to the events buffer.

Actions with another Sim name that Sim, whoever started them: "Carlos: Greet, with Elvira".

## Buy mode

F2 opens buy mode. The catalog's products are buttons: each product says its name, price and
ratings, and the details buffer holds its description. Moving past a page's last product (or
before its first) turns the page.

### Placing an object

Enter on a product picks it up. The mod finds every spot of the lot where it fits, in every
direction, nearest to the selected Sim first.

| Key | What it does |
|---|---|
| Page Down, Page Up (or Tab) | The spots, nearest first; each says its room, the directions it fits in and how far it is. |
| Control+Page Down, Control+Page Up | The rooms that have spots. |
| Shift+Page Down, Shift+Page Up (or comma, period) | Turn it; the facing is said. |
| Control+Shift+Page Down, Page Up | Beside the lot's things, nearest first, facing them when it fits. |
| Arrows | Move it one tile instead (Shift: five): whether it fits, what it runs into, the room when it changes, the walls around it, its facing and what it faces (further off too: "toward Sofa, 3 tiles away"), the tile; then the tile's sounds. Walls, fences and windows stop it; doors let it through. |
| Alt+Arrows | The same, through walls. |
| Home | Back to the selected Sim. |
| Enter | Buy it there; the game hands you another copy until Escape. |
| Escape | Stop placing (a moved object goes back). |
| Delete | Sell the held object. |
| R | Say the spot again, in full. |

A multi-tile object says how much floor it takes ("takes 2 tiles: 2 west to east, 1 north to
south").

A seat's spots come first where something is used sitting down, such as a computer on a desk, and the seat is turned toward it: "facing west, to use Computer sitting down". After an object is placed the mod says how it will be used: "to use Computer sitting down", "used from Office chair", "Sims can reach it" or "no Sim can reach it".

### Moving and selling

In buy mode, Page Down to a thing of the lot and press Enter: it is picked up, and the same keys
place it somewhere else. Delete sells it. Its spots start nearest where it stood.

## Build mode

F3 opens build mode. Its tools and styles are buttons.

Pressing a tool moves you to its styles, in a group named after the tool; Tab goes back to the tools.

### The build cursor

Enter on a wall, fence, foundation, floor or pool style starts the build cursor at the selected
Sim. Walls, fences and foundations go from corner to corner; floors and pools cover tiles.

| Key | What it does |
|---|---|
| Arrows | One corner or tile; with Shift, five. Says what stands there, the room when it changes, the floor (floors and pools) or the walls, fences, doors and windows meeting at the corner, a slope, then where it is. |
| Page Up, Page Down | The level above or below (0 the terrain, 1 a foundation's floor). |
| Space | Mark the start. |
| Enter | Build from the start to the cursor; the cost is said. |
| Control+Enter | Remove from the start to the cursor. |
| Home | Back to the selected Sim. |
| R | Everything about the cursor. |
| Escape | Leave the cursor. |

After a wall is built, a Sim of the family shut in a room without a door is announced; the
game's Control+Z undoes the wall.

### Doors, windows, stairs and roofs

Doors, windows and stairs are placed like furniture, from the spots where they fit: a door's
spot says which wall it is on and the room it leads to. Roofs: the roof page's automatic roof
buttons.

After a door is placed the mod says whether Sims can walk through it, or what is in the way. To change one later, find it in **doors, windows and stairs** or with Control+F: Enter picks it up to move it, in buy mode or with build mode's hand, and Delete removes it.

## Sound cues

The build cursor, placing's arrows and the lot map play a short sound on each step: the floor
(grass, wood, tile, carpet, stone, the pool's edge, water, no floor upstairs: what the floor is
made of, from its catalog definition), each side's wall, fence, railing, door or window (west on
the left, east on the right, north higher, south lower), stairs, a Sim or an object on the tile, a
slope, a room change, the lot's edge, and while placing whether the object fits.
**Control+Shift+S** turns them off and on.

## Other keys

| Key | What it does |
|---|---|
| Control+Shift+C | The game's cheat console: its input is a text field; Enter on it gives it the keyboard, type the cheat, Enter runs it, and what it writes is spoken. |
| Control+Shift+V | Light the focused control on screen, for a sighted helper. In live mode, with the game's window in front, the camera also looks at the lot's thing in focus and the mouse pointer goes onto it. |

## The game's own keys

These are the game's, and still work:

| Key | What it does |
|---|---|
| F1, F2, F3, F4, F5 | Live, buy, build, camera and options mode. |
| P | Pause. |
| 1, 2, 3 | Normal, fast, fastest speed. |
| Space, N | Select the next Sim. |
| Z, X | Zoom in and out. |
| Control+Z | Undo in buy and build mode. |

## Settings

The mod's settings are in `Sims2Access\Sims2Access.ini` in the game's folder, section `[mod]`.
The keys that change them save them at once.

| Setting | Values |
|---|---|
| `narrator` | 2 everything, 1 warnings only, 0 off (Control+N). |
| `cues` | 1 sound cues on, 0 off (Control+Shift+S). |
| `cue_volume` | Volume of the sound cues, 0 to 100 (default 80). |
| `show_focus` | 1 lights the focused control on screen (Control+Shift+V). |
| `social` | Whose conversations are spoken: 3 everyone, 2 anyone with your family, 1 your family, 0 nobody (Control+Shift+N). |
| `hours` | 1 says each new hour (Control+T). |

## When something goes wrong

- **No speech at all**: start your screen reader before the game. The mod's log is
  `Sims2Access\Sims2Access.log`; send it with your report.
- **"Unsupported game version"**: the game has updated; install the mod's next version.
- **A key does nothing**: Control+F1 says which keys work where you are.

## Not done yet

Wallpaper, drawn roofs and terrain in build mode; the names of the aspiration rewards; putting an object from a Sim's inventory on the lot; names for Create a Sim's heads, hair and
clothes; the expansions' own screens (university, nightlife, business, seasons, vacations,
apartments, magic) are only read by the generic navigation so far.
