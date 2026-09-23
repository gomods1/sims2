# Screens (Sims 2 Legacy Collection, Spanish game)

What each screen does with the mod now. "Generic" means the navigator reads the GZ window tree
(groups by panel, labels from tooltips, values, dialogs); "dedicated" means code for that screen.
Tested live unless marked **not yet seen**.

## Start and neighborhoods

| Screen | Status |
|---|---|
| Loading screens, EA/Maxis intro | generic: texts read as they appear |
| Neighborhood chooser | generic: thumbnails read by name, their tooltip as detail |
| Neighborhood view | dedicated: the lots as the first group (homes first, with their families; community lots; for sale; apartment buildings with tenants); Enter opens the game's lot panel, or with a family chosen in the family bin asks to move it in; the neighborhood panel (families, lots and houses, decorations, story, options) generic |
| Lot panel (Información del solar) | generic: name, value, size, zone, family; Entrar en solar, Empaquetar, Cambiar nombre |
| Family bin | generic: families as buttons; choosing one starts the game's move-in |
| Neighborhood story, options, game tips | generic |
| Move-in question ("¿Mudar a la familia ...?") | generic dialog |

## Create a Family / Create a Sim

| Screen | Status |
|---|---|
| Family screen | generic: Crear un Sim (Sim, child, dog, cat), surname field, Aceptar / Cancelar familia, Editar <Sim> |
| Step 1: name, age, sex, skin, body | generic plus: typing echoed, field named by its prompt, choices say "seleccionado", repeated labels numbered (Tono de piel 1-4) |
| Steps 2-5: heads, hair, face, clothes | grids read "3 de 6" with their selection; no names for the items yet |
| Step 6: personality, aspiration, star sign | dedicated: personality bars as sliders ("de Gruñón a Cordial, 6 de 10", Left/Right with the points left); aspirations, signs (the chosen one said); group names from design captions |
| Turn-ons / turn-offs (Nightlife) | generic plus: the slots named "Me pone 1/2", "Me corta"; the trait list is a generic modal |
| Pets | **not yet seen** |

## Lot: live mode

| Screen | Status |
|---|---|
| Control panel, Sim panel, wants and fears, family portraits | generic plus: funds and clock values, speed and pause said as they change |
| Needs | dedicated group from the Sim's data (panel open or not) |
| Action queue | dedicated group; Enter cancels |
| Interaction (pie) menus | dedicated: a list of the game's entries; Enter chooses (sub-levels too) |
| Objects of the lot | dedicated browser: PageUp/PageDown, Shift categories, Ctrl rooms; Enter menus; Ctrl+Enter walks there |
| Relationships panel | generic: filters, family tree; the list (people) **not yet checked with relationships** |
| Job panel | generic plus: skills as values ("Habilidad 'Cocina': 0 de 10"); job details **not yet seen** (the test Sim is unemployed) |
| Simology: personality, interests, hobbies, memories, chemistry | generic plus: personality and interests as values; hobbies, memories, chemistry **not yet checked** |
| Game tips, notices (visitors, clubs), dialogs | generic; notices' buttons (Trasladar, Centrar, Cerrar) |
| Seasons indicator | generic buttons (the season's days left) |
| Narrator | dedicated: actions started / finished, needs low / urgent, selection, visitors; balloons **not yet** |

## Lot: buy mode

| Screen | Status |
|---|---|
| Buy panel, categories and subcategories | generic |
| Catalog | dedicated: products (name, price, ratings; description in the details), pages turned by moving past either end |
| Placing a product, moving and selling objects | dedicated (`nav/placement.cpp`) |

## Lot: build mode

| Screen | Status |
|---|---|
| Build panel and its tool pages | generic ("Atrás" goes back to the categories) |
| Walls, fences, rooms, foundations | dedicated corner cursor (`nav/build.cpp`) |
| Floors, pools | dedicated tile cursor |
| Doors, windows, stairs, roofs, terrain, wallpaper | **not yet** (doors and windows are objects: buy placement may place them; untested) |

## Other

| Screen | Status |
|---|---|
| Options (graphics, audio, game, camera) | generic plus: option buttons say which is chosen, check boxes |
| Main menu, save / quit dialogs | generic |
| Cheat console | typing echoed; the game's own console |
| Travel: phone > Llamar... > Pedir taxi, the taxi, "¿Adónde te gustaría ir?" | generic plus: the destination lists (list boxes) read and selectable, Aceptar travels; arrival works |
| List boxes anywhere (cTSWinListBox) | dedicated: every row (scrolled out too), selected ones said, Enter selects |
| University, business, vacations, apartments, hobbies, magic | **not yet seen** |
