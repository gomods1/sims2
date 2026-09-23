// Placing a bought object without the mouse. Enter on a catalog product picks it up (the game's
// object tool); the mod then holds a tile cursor that the tool follows (game/buy.h):
//   PageDown / PageUp            next / previous spot where the object fits, nearest to the Sim first
//   Ctrl+PageDown / Ctrl+PageUp  next / previous room with such spots (the Sim's own room first)
//   arrows                       move one tile (Shift: five); -y is north
//   Home                         back to the selected Sim's tile
//   , and .                      the game's rotation keys; the facing and verdict are said again
//   Enter                        put the object down (buys it) where it fits; the game hands over
//                                another copy, so placing goes on
//   R                            say the spot again
//   Escape                       stop placing (the game's Escape drops the object)
#pragma once

#include <windows.h>

#include <string>

namespace nav::placement {

bool Active();
void Begin(const std::string& productName);
// A key while placing: true when the mod used it.
bool OnKey(WPARAM vk, bool shift, bool ctrl);
void Tick();

}  // namespace nav::placement
