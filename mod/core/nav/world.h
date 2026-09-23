// The object browser of the lot view: every Sim and object a player can interact with, by
// category (Sims, then the buy mode categories), nearest to the selected Sim first.
//   PageUp / PageDown              previous / next object in the category
//   Shift+PageUp / Shift+PageDown  previous / next category
//   Ctrl+PageUp / Ctrl+PageDown    previous / next room (the first press: the Sim's own room)
//   Enter (after PageUp/PageDown)  open the object's interaction menu (buy mode: pick it up)
//   Ctrl+Enter                     walk the selected Sim there (Ctrl+Shift+Enter: run), through
//                                  the game's own "go here" menu on a click on that tile
//   R                              say the focused object again
#pragma once

#include <string>

namespace nav::world {

bool LotViewActive();  // live, buy or build mode of a lot is on screen
void Reset();
void NextObject(int delta);
void NextCategory(int delta);
void NextRoom(int delta);
void Interact();
void WalkTo(bool run);
void Repeat();
std::string RoomNameAt(float x, float y, int level);  // the room there, by its contents
std::string WhereFromSim(float x, float y, int level);  // "3 tiles north" from the selected Sim
void Tick();  // says when an object's interaction menu did not open (it has none now)

}  // namespace nav::world
