# Roadmap

What the mod must eventually cover so a blind player can play The Sims 2 and all its expansions
by keyboard, as the Sims 1 mod does for the Sims 1. Keys follow the Sims 1 mod wherever the same
job exists (PageUp/PageDown + Shift/Ctrl for the lot's objects and rooms, Enter/Ctrl+Enter,
Ctrl+E status, Ctrl+N narrator, Ctrl+L room layout, Ctrl+1..9 Sims, Ctrl+Shift+C cheats,
review buffers on Ctrl+arrows). Screen status lives in `SCREENS.md`.

## Done

- Loader, speech, crash logging, languages (19), game strings from the packages.
- Every GZ window screen and dialog by generic navigation: groups by panel, labels from tooltips,
  values drawn on buttons, text fields the player can edit, modal dialogs, texts a press changes.
- Neighborhood chooser, neighborhood dialogs and story, the neighborhood panel; the lots as a
  list with their families (apartments with their tenants); Enter opens the game's lot panel.
- Live mode HUD: control panel, Sim panel (clock, speed, age, panels), wants and fears, needs
  with their levels (when the needs panel is open), family portraits.
- Interaction menus as lists, chosen by Enter (sub-levels too).
- Object browser: PageUp/PageDown, Shift for the buy categories, Enter for the interactions;
  Ctrl for rooms (named from their contents).
- Live state: pause and speed, clock and funds, Ctrl+E, needs and action queue groups, family
  members (Ctrl+1..9), a narrator for actions, needs, selection and visitors.
- Buy mode: catalog products (name, price, ratings, description), placing a bought object by
  keyboard (spots where it fits, nearest first, by room; arrows; facing; Enter buys), moving and
  selling objects of the lot (Enter on the object browser, Delete sells); moving past the last
  product (or before the first) turns the catalog's page.
- Live mode by ear: pause, speed, clock, funds changes (and money paid or received), Ctrl+E
  state, the action queue (Enter cancels), family members (Ctrl+1..9), the narrator (actions,
  needs, visitors, balloons and relationship headlines, Ctrl+N verbosity), rooms
  (Ctrl+PageUp/PageDown, Ctrl+L layout), walking (Ctrl+Enter, Ctrl+Shift+Enter runs), the camera
  following the selected Sim (Ctrl+Shift+F).
- The Sim panel: needs, relationships (daily and lifetime), job (column by column, workdays),
  skills, personality, interests, hobby enthusiasm, memories, chemistry, rewards and inventory.
- Create a Family / Sim / Pet: names, age, sex, skin, body, grids ("3 of 6"; pet breeds by name),
  personality with the points left, pet trait pairs, aspiration, star sign, turn-ons; accepting;
  the family tree (pick up, drop on another); the family bin; moving in.
- Build mode: walls, fences, rooms, foundations (corners); floors and pools (tiles); doors,
  windows and stairs through placement's spots; the cost of each change; auto roofs (the roof
  page's buttons).
- Travel: taxi from home to a community lot, Options > Barrio back; saving (Options > Guardar).
- Ported from the Sims 1 mod (its commits up to v0.2.0, 2026-11-01), verified in the game on the
  Contreras lot unless noted:
  - The lot's things on PageDown/PageUp in every mode, Shift+ kinds, Ctrl+ rooms.
  - Alt+letter jumps to the group whose name starts with it; Ctrl+Shift+V lights the focused
    control for a sighted helper; the main screen's "mod help" group (keys, manual).
  - The player's manual (`docs/manual`, 20 languages, pandoc; Ctrl+Shift+F1), `tools/build_docs.py
    --check`, `tools/package.py` (zip + the shared installer's new Sims 2 entry, `..\installer`
    `build_offline_sims2.bat`, game 9).
  - The lot map (Ctrl+M) and the sound cues of every cursor, from the lot's real build data
    (`game/lotgrid.cpp`): floors by their catalog material, walls, fences, railings, doors and
    windows by side, stairs, pools; SpeechHost plays them through XAudio2 (Ctrl+Shift+S).
  - Placing: the arrows' line (fits, what it runs into, room changes, walls, facing, what it
    faces, tile), a multi-tile object's size, Ctrl+Shift+PageDown/PageUp beside the lot's things.
  - The build cursor's line (what stands there, room changes, the floor or the walls at the
    corner, a slope, the coordinates last) and its cues (built, not yet tried in the game).
  - Multi-tile furniture (beds, sofas, tables, wide windows) is listed with its catalog name (it
    was missing from the lists).
  - Text fields take the keyboard when focused and speak the character at the caret; Enter on a
    read-only item speaks its tooltip description (built, not yet tried in the game).

- Ported from the Sims 1 mod's commit of 2026-11-02, verified on the Contreras lot unless noted:
  - Ctrl+F finds a thing by typing part of its name; the camera's following moved to Ctrl+Shift+F.
  - The kinds "problems" and "doors, windows and stairs"; broken and dirty said on a thing's line.
  - Delete on a door or window (picked up and sold); after placing a door, whether Sims can use it.
  - The camera's zoom said when it changes, zoom and facing in the control panel.
  - A build tool's press moves to its styles, in a group named after the tool.
  - Left out, because this game already does it or differs: the arrow cursor naming a door's wall
    (`WallPart` already did), the job's hours and carpool (the game's own label and hover text),
    "no menu quits a job" (here the phone has "Phone Call - Work - Quit Job"), the roof panel's colours
    and its "the game builds the roof" hint (this game's roofs have named tools, and can be
    drawn), holding off the object a tool brings back (not seen here). Grass painting has no
    counterpart yet: terrain is not done.

- 2026-11-10, the Sims 1 mod's commit of 2026-11-09 ported and the 2026-11-10 tester feedback
  (Sims 2), with the roadmap's RE (STRUCTS.md "Buy mode", "Light", "Rewards catalogs",
  "SimAntics"); verified live on the Contreras lot unless noted:
  - Catalog products say their size ("3 por 1 casillas", from the object folder's multi-tile
    groups); Left/Right turn the catalog's pages (NAVIGATION.md's key rule changed with it).
  - Placing: the game's own reason when it does not fit ("No puede cruzarse con la pared", the
    tool's error set), the facing or nearest tile that fits, the cursor's part of a multi-tile
    object and where it extends (a held object's tiles from its definition's part offsets).
  - The rooms' light (the game's light map, scored with its own Room thresholds) on the map, the
    build cursor, placing and Ctrl+L; indoors only.
  - Conversation balloons say "habla de" (topics), a crossed-out one "se queja de" (heard:
    "Carlos habla de Crimen", "Carlos se queja de Deportes").
  - The aspiration rewards name each reward with its price ("Árbol del dinero, 3.000 puntos de
    aspiración"), both pages; Backspace presses buy mode's "Atrás" (window 0xb110).
  - Placing speaks the object's real facing: the tool's rotation is not the object's direction
    (the object faces 2 - rotation; every "mirando al ..." while placing, the spots' directions,
    the seats' and "beside a thing" turns were a quarter or half turn off before); after placing,
    the facing is read from the placed object. Room names leave the held object out (a bedroom
    was "sala de estar" while a sofa was tried in it); the first line asks the tool for a verdict.
  - Not yet seen live: "no te alcanza" (Carlos could afford every reward), the career rewards, the
    professor sheet's majors, the job sheet's "unemployed", aspirations02; a Sim's own car
    ("Car Ownable", the outing pickup) in the lists.
  - Not ported: the Sims 1 clock's AM fix (the Spanish game's clock reads "13:01"; check the
    English one), the FPS counter (not seen in this game), simoleons for "$" (the game's money is
    drawn as § and was already spoken as simoleons).

## Next

- Check in the game what the 2026-11-10 changes could not try (above). Placing: once, after six
  Ctrl+Shift+PageDown turns and the game's period key, a chair said "mirando al norte" and went
  down facing south (not reproduced in single sequences; the placed line now says the real
  facing). The held object's own direction variable follows the game's turning keys but not the
  mod's rotation writes: find the game's call that applies a rotation (the , and . handler) and
  use it instead of writing +0xa0. A tester was stood up downtown because the car that picks a
  Sim up was missing from the lists (probably an older build: the taxi was listed since
  2026-11-08); ask which car it is if it happens again.
- Wants and fears that Sims talk about (balloon flag 0x2000, STRUCTS.md "SimAntics"): read Temp 4
  and 5 in the effects hook and name the want through the want simulator.
- The rooms' light outdoors (daylight: lighting manager slot 35, room 0); light rooms are another
  partition than the mod's rooms (type 0).
- The 2026-11-08 tester feedback, still open: putting an object from a Sim's inventory on the lot (the
  inventory panel was empty on the Contreras lot: try dragging an item through the mod's virtual
  mouse or the hand tool); eating out (a restaurant lot: the host, waiter and chef are now named;
  the destination dialog does not say a lot's kind); build mode "needs a lot of love". Not seen
  live: a "counter is needed" thought (the sheet lookup is the game's, the cells from the
  texture), "$Subject" in a thought, a fear come true.

- Check in the game what the 2026-11-07 port (the Sims 1 mod's tester feedback of 2026-11-06)
  could not try on the Contreras lot: a thing no Sim can reach (only "reachable" was heard, 0 of
  238), a promotion mark (Carlos is at his
  career's top: try a Sim below it), an interrupted action ("X stopped A for B; it waits in the
  queue"), a visitor's conversation with the family under Ctrl+Shift+N (the balloon's stack
  object), the pointer moving with Ctrl+Shift+V (the game must be in front). Pets: Ctrl+Shift+E
  says only the gender (no outfit data).
- Not ported from the Sims 1 commit: "Enter on N alike" (the Sims 2 lists never merge identical
  things: each is its own item already); the travel map's filters (no equivalent: the Sims 2
  taxi's "Where would you like to go?" dialog, UI script `a99d8a11_cafe0090`, is a list box, a text
  field and Accept / Cancel; its neighborhood-view form `a99d8a11_49001020` has the usual
  neighborhood buttons and no lot filters); the relationship numbers of pairs without the selected Sim (the
  Sims 2 mod has no relationship watch yet: it speaks the game's headlines instead).

- Check in the game what the 2026-11-02 port could not try: an action with another Sim (no visitor
  was on the lot: "Carlos: Saludar, con Elvira"), the facing said when the camera turns (the
  panel's Rotar buttons do nothing to a press: find out how to turn it by keyboard), the door
  check's two warnings (it only ever said usable), Delete with build mode's hand.

- Check in the game what the port built but could not try on the Contreras lot: the build
  cursor's line and cues while building, text fields (focus taking the keyboard, caret keys) in
  Create a Sim, Enter on a skill or interest (its description), slopes (the lot is flat: the
  terrain height call returns 0 everywhere) and ponds (the game's water test said no on a pool;
  pools are found by their floor), walls whose layers read `00000100` (one edge upstairs, left out).
- The manuals' game names (Create a Pet, turn-ons, mode names, trait pairs) were written by the
  translators without the game's own texts in some languages: check them against the packages'
  STR# tables.

- Build mode: wallpaper (the tool picks walls from the screen: `STRUCTS.md`), drawn roofs, terrain
  (with it the Sims 1 mod's grass under the cursor);
  checks before building walls (done after: a Sim shut in is announced; still: objects cut off);
  ready-made rooms.
- Buy mode: the reasons of kind 2 ("must be placed on one of these:" and its list) were not heard
  yet; check the list reads well.
- Create a Sim: names for heads, hair, clothes (no
  names in the game: perhaps colors and categories), group names without design captions.
- Balloons still unnamed: wants and fears (flag 0x2000), and the sheets listed in STRUCTS.md
  "SimAntics" (thoughtPets01, birdbee01, bookgenre-*, novel-plots, the vacation sheets,
  conversation-Arts/Computers/Economy/Mechanics/Sports, TeachMassage).
- Expansion content, checked one by one through the generic navigation, dedicated readers where
  it falls short: university (dorms, classes), nightlife (dates, cars), business, seasons,
  vacations, apartments, magic.
- Every new mod string, Keys* help and manual section in all 19 translations in the same change
  (CLAUDE.md "Keys and docs never drift").

## Findings to follow up

- Community lots: the phone booth's "Pedir un taxi" asks "¿Adónde? Casa / Otro solar"; after
  "Casa" (2026-09-23, at ultra speed) no taxi appeared in two game days (not on the object list
  either). Options > Barrio leaves (the game warns the lot's progress is lost). Verify the taxi at
  normal speed.

