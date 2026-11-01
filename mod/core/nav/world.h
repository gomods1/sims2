// The lot's things (every Sim and object a player can interact with) as two groups of the lot
// view's navigation (NAVIGATION.md): "things by kind" (Sims, then the buy mode categories) and
// "things by room" (the selected Sim's room first). Their items are kinds or rooms; Enter opens
// one (its things, nearest to the selected Sim first), Backspace goes back to the kinds or
// rooms. From anywhere in the lot view (live, buy and build mode):
//   PageDown / PageUp              next / previous thing of the kind or room chosen last
//   Shift+PageDown / Shift+PageUp  next / previous kind (its things, from the nearest)
//   Ctrl+PageDown / Ctrl+PageUp    next / previous room (the first press: the selected Sim's)
// (the game's own PageUp/PageDown change floor; the control panel's floor buttons still do).
// On a thing:
//   Enter        its interaction menu (buy mode: pick it up to move it)
//   Ctrl+Enter   walk the selected Sim there (Ctrl+Shift+Enter: run), through the game's own
//                "go here" menu on a click on that tile
#pragma once

#include <string>
#include <vector>

#include "../game/objects.h"
#include "model.h"

namespace nav::world {

bool LotViewActive();  // live, buy or build mode of a lot is on screen
void Reset();
Group KindsGroup();    // kKindsGroup: the kinds, or the things of the open kind
Group RoomsGroup();    // kRoomsGroup: the rooms, or the things of the open room
// Enter on a kind or room (its item's number): its things. Backspace: back to the kinds or
// rooms; returns the number of the kind or room that was open (0: none was open).
void Open(Ptr group, int number);
int Close(Ptr group);
// The kind or room after the open one (delta +1) or before it, skipping empty ones, wrapping; with
// none open the first kind (the last going back) or the selected Sim's room. 0: the lot has none.
int Step(Ptr group, int delta);
int OpenNumber(Ptr group);  // the kind or room open in the group, 0: none
Ptr LastGroup();            // the group a kind or room was opened in last (kKindsGroup at first)
int FocusedId();            // object id of the thing focus was last on, -1: none
void Focus(int objectId);   // the thing focus is on (Ctrl+Enter and Enter act on it)
void Interact();
void WalkTo(bool run);
bool HasFocus();            // a thing has been focused
std::string RoomNameAt(float x, float y, int level);  // the room there, by its contents
std::string WhereFromSim(float x, float y, int level);
// Ctrl+L: the room at a tile: its name, size, bounds, doors and windows (where each leads).
void SayLayout(int x, int y, int level);  // "3 tiles north" from the selected Sim
void Tick();  // says when an object's interaction menu did not open (it has none now)

// A thing's name as the lists say it (a Sim's full name, an object's catalog name).
std::string DisplayName(const game::LotObject& o);
bool IsListed(int objectId);  // a thing the lists show (a Sim, an object a player knows)
// What stands on a tile (whole objects and the parts of multi-tile ones), each thing once, Sims
// first; doors and windows left out (they are the walls' openings).
std::vector<game::LotObject> ThingsAt(int x, int y, int level);
// The selected Sim walks (or runs) to a tile, through the game's own "go here" menu: the live map.
void WalkToSpot(float x, float y, int level, bool run);

}  // namespace nav::world
