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

## Next (live mode first: it is where the game is played)

- Game state by ear: pause and speed changes, the clock and day, funds changes; Ctrl+E the
  selected Sim's state (action, needs from the Sim's data even with the panel closed, mood);
  the action queue as a group (Enter cancels); family members (select with Ctrl+1..9, Space/N).
- Narrator: thought and speech balloons, need warnings, what each Sim starts and finishes,
  visitors arriving and leaving, phone calls and letters (dialogs already read), money.
- Rooms: the lot's rooms (Ctrl+PageUp/PageDown), room names from contents, Ctrl+L layout.
- Walk to an object or spot (Ctrl+Enter), camera following the selected Sim (sounds come from
  what is on screen), edge scrolling off.
- Sim names in full (neighbor data), relationships, skills, job, aspiration, personality,
  interests, inventory.
- Create a Family / Create a Sim: names (typing echoed), age, sex, skin tone, body shape, heads
  and other grids ("3 of 6"), personality sliders with the points left, aspiration, star sign,
  turn-ons; accept the Sim and the family; the family bin; moving a family in (Enter on a lot
  with the family chosen: the game's own question and cost).
- Build mode: walls, fences, rooms, foundations by corners; floors and pools by tiles (a cursor,
  Space marks, Enter builds, Ctrl+Enter removes, PageUp/PageDown levels), with the cost.
- Walking to an object (Ctrl+Enter; Ctrl+Shift+Enter runs).
- Buy mode, still: the game's reason
  when a spot is refused (STR# 0x89), which directions fit a spot, doors and windows, multi-tile
  objects' footprint in the spot line.
- Build mode, still: walls' checks before building (shutting a Sim in, cutting objects off),
  ready-made rooms, the room's layout (Ctrl+L), doors and windows (objects: buy placement's spots
  on walls), stairs, roofs, terrain, wallpaper, the build catalog's names for groups.
- Create a Sim, still: names for heads, hair, clothes (their property sets), the family's
  relationships, pets, the CAS group names without design captions (skin, body).
- Create a Sim / family (CAS): names, age, gender, personality, aspiration, looks (with the
  game's own names where they exist), and the family bin, moving families in and out.
- Expansion content: university (dorms, classes), nightlife (dates, cars), business, pets,
  seasons, travel, apartments, hobbies, magic; their panels and dialogs through the generic
  navigation first, then dedicated readers where the generic one falls short.
- Translation of every new mod string (19 languages); the key help per screen (Ctrl+F1).
