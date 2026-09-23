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
- Object browser: PageUp/PageDown, Shift for the buy categories, Enter for the interactions.

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
- Buy mode: catalog by category, product details, placing through a list of legal spots
  (the Sims 1 mod's placement model, rebuilt on this engine's placement checks), selling.
- Build mode: walls, floors, doors and windows, stairs, pools, terrain (the Sims 1 mod's cursor).
- Create a Sim / family (CAS): names, age, gender, personality, aspiration, looks (with the
  game's own names where they exist), and the family bin, moving families in and out.
- Expansion content: university (dorms, classes), nightlife (dates, cars), business, pets,
  seasons, travel, apartments, hobbies, magic; their panels and dialogs through the generic
  navigation first, then dedicated readers where the generic one falls short.
- Translation of every new mod string (19 languages); the key help per screen (Ctrl+F1).
