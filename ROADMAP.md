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
  the family bin; moving in.
- Build mode: walls, fences, rooms, foundations (corners); floors and pools (tiles); doors,
  windows and stairs through placement's spots; the cost of each change.
- Travel: taxi from home to a community lot, Options > Barrio back; saving (Options > Guardar).

## Next

- Build mode: wallpaper (the tool picks walls from the screen: `STRUCTS.md`), roofs, terrain;
  checks before building walls (shutting a Sim in, cutting objects off); ready-made rooms.
- Buy mode: the game's reason when a spot is refused (STR# 0x89), which directions fit a spot,
  multi-tile objects' footprint in the spot line.
- Create a Sim: the family's relationships (the drag screen), names for heads, hair, clothes (no
  names in the game: perhaps colors and categories), group names without design captions.
- Balloons still unnamed: job, University's professor sheet, wants and fears (flag 0x2000).
- Expansion content, checked one by one through the generic navigation, dedicated readers where
  it falls short: university (dorms, classes), nightlife (dates, cars), business, seasons,
  vacations, apartments, magic.
- Translation of every new mod string (19 languages); the key help per screen (Ctrl+F1).

## Findings to follow up

- Community lots: the phone booth's "Pedir un taxi" asks "¿Adónde? Casa / Otro solar"; after
  "Casa" (2026-09-23, at ultra speed) no taxi appeared in two game days (not on the object list
  either). Options > Barrio leaves (the game warns the lot's progress is lost). Verify the taxi at
  normal speed.

