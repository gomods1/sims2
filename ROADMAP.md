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
  following the selected Sim (Ctrl+F).
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

## Next

- Check in the game what the port built but could not try on the Contreras lot: the build
  cursor's line and cues while building, text fields (focus taking the keyboard, caret keys) in
  Create a Sim, Enter on a skill or interest (its description), slopes (the lot is flat: the
  terrain height call returns 0 everywhere) and ponds (the game's water test said no on a pool;
  pools are found by their floor), walls whose layers read `00000100` (one edge upstairs, left out).
- Buy mode's catalog "Back" (the Sims 1 mod presses it on Backspace): find whether the Sims 2
  catalog has one (its subsort buttons) and give Backspace the same job.
- The manuals' game names (Create a Pet, turn-ons, mode names, trait pairs) were written by the
  translators without the game's own texts in some languages: check them against the packages'
  STR# tables.

- Build mode: wallpaper (the tool picks walls from the screen: `STRUCTS.md`), drawn roofs, terrain;
  checks before building walls (done after: a Sim shut in is announced; still: objects cut off);
  ready-made rooms.
- Buy mode: the game's reason when a spot is refused (STR# 0x89; the Sims 1 mod says it, and its
  "needs a surface" in its own words). Which directions fit a spot and the footprint are done.
- Create a Sim: names for heads, hair, clothes (no
  names in the game: perhaps colors and categories), group names without design captions.
- Balloons still unnamed: job, University's professor sheet, wants and fears (flag 0x2000).
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

