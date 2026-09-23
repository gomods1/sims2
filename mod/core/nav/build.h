// Build mode by keyboard: a corner cursor for the tools that are dragged over the lot (walls,
// fences, rooms). Enter on a wall style in the build catalog starts it; the mod drags the game's
// own tool from corner to corner (the ground pick answers with the mod's corners, game/buy.h).
//   arrows            move the corner (Shift: five); -y is north
//   Space             mark where the wall starts
//   Enter             build from the mark to the cursor (without a mark: nothing to build)
//   Ctrl+Enter        remove the walls from the mark to the cursor (the game's Ctrl-drag)
//   PageUp / PageDown the level up / down (0 = the terrain; 1 = a foundation's floor or the next)
//   Home              back to the selected Sim
//   R                 say the corner and the mark again
//   Escape            leave the cursor (the game's Escape drops the tool)
#pragma once

#include <windows.h>

#include <string>

namespace nav::build {

bool Active();
void Begin(const std::string& toolName);
bool OnKey(WPARAM vk, bool shift, bool ctrl);
// The cursor's tile and level (for Ctrl+L); false when the cursor is not active.
bool CursorTile(int* x, int* y, int* level);
void CheckShutInNow();  // the check made after a wall is built (dev command !shutin)
void Tick();

}  // namespace nav::build
