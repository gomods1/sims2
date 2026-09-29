// The lot's things (every Sim and object a player can interact with) as two groups of the lot
// view's navigation (NAVIGATION.md): "things by kind" (Sims, then the buy mode categories) and
// "things by room" (the selected Sim's room first). Their items are kinds or rooms; Enter opens
// one (its things, nearest to the selected Sim first), Backspace goes back to the kinds or
// rooms. On a thing:
//   Enter        its interaction menu (buy mode: pick it up to move it)
//   Ctrl+Enter   walk the selected Sim there (Ctrl+Shift+Enter: run), through the game's own
//                "go here" menu on a click on that tile
#pragma once

#include <string>

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
void Focus(int objectId);   // the thing focus is on (Ctrl+Enter and Enter act on it)
void Interact();
void WalkTo(bool run);
bool HasFocus();            // a thing has been focused
std::string RoomNameAt(float x, float y, int level);  // the room there, by its contents
std::string WhereFromSim(float x, float y, int level);
// Ctrl+L: the room at a tile: its name, size, bounds, doors and windows (where each leads).
void SayLayout(int x, int y, int level);  // "3 tiles north" from the selected Sim
void Tick();  // says when an object's interaction menu did not open (it has none now)

}  // namespace nav::world
