// Placing a bought object without the mouse. Enter on a catalog product picks it up (the game's
// object tool); the mod then holds a tile cursor that the tool follows (game/buy.h):
//   PageDown / PageUp (Tab)      next / previous spot where the object fits, nearest to the Sim first
//   Ctrl+PageDown / Ctrl+PageUp  next / previous room with such spots (the Sim's own room first)
//   Shift+PageDown / PageUp      turn it (the game's period / comma)
//   Ctrl+Shift+PageDown / PageUp beside the next / previous thing of the lot, nearest first (on a
//                                free tile beside it, in its room, facing it when it fits)
//   arrows                       move one tile (Shift: five); -y is north. The line: whether it
//                                fits, what it runs into, the room when it changes, the facing,
//                                what it faces, the tile last; then the tile's sounds (nav/cues.h).
//                                Walls, fences and windows stop it (doors do not)
//   Alt+arrows                   the same through walls
//   Home                         back to the selected Sim's tile
//   , and .                      the game's rotation keys; the facing and verdict are said again
//   Enter                        put the object down (buys it) where it fits; the game hands over
//                                another copy, so placing goes on
//   R                            say the spot again
//   Escape                       stop placing (the game's Escape drops a new object, or puts a
//                                moved one back)
//   Delete                       sell the object (the game's key)
// Enter on an object of the lot browser in buy mode picks it up to move it (BeginMove).
#pragma once

#include <windows.h>

#include <cstdint>
#include <string>

#include "../game/memory.h"

namespace nav::placement {

bool Active();
void Move(int dx, int dy, bool throughWalls);  // the arrows (Alt: through walls)
void Begin(const std::string& productName);
// Picks up an object of the lot (buy mode's hand, clicked on it through the object pick).
void BeginMove(game::Ptr object, uint32_t guid, const std::string& name, float x, float y, int level);
// A key while placing: true when the mod used it.
// Delete on a door or window of the lot's things: picks it up and sells it, as the game's hand and
// Delete do; says what came of it.
void BeginRemove(game::Ptr object, uint32_t guid, const std::string& name, float x, float y, int level);
bool OnKey(WPARAM vk, bool shift, bool ctrl);
void Tick();

}  // namespace nav::placement
