// The live-mode map (ported from the Sims 1 mod, where a tester asked for it): a read-only cursor
// over the lot during play, with what stands on each tile and the sound cues of nav/cues.h,
// without leaving live mode ("I didn't notice the bed was turned that way" is something a player
// should find out while playing). A spatial tool (NAVIGATION.md): while it is on, the arrows move it.
//   Ctrl+M                          the map on / off (it starts at the selected Sim)
//   Arrows                          one tile (Shift: five): what stands there and which way it
//                                   faces, the room when it changes, walls, doors and windows by side
//   Home                            back to the selected Sim
//   Ctrl+Shift+PageUp / PageDown    the floor above / below
//   PageDown / PageUp (Shift, Ctrl) the lot's things as everywhere; the cursor goes to the thing
//   Enter                           the interaction menu of what stands there (buy mode: pick it up)
//   Ctrl+Enter                      the selected Sim walks to the tile (Ctrl+Shift+Enter: runs)
//   R                               where the cursor is, in full (details buffer)
//   Escape                          leave the map
#pragma once

#include <windows.h>

namespace nav::map {

void Reset();
void Tick();  // ends the map when the lot view goes away or a building tool takes the keys
bool Active();
bool CursorTile(int* x, int* y, int* level);
bool OnKey(WPARAM vk, bool shift, bool ctrl);  // the map's keys while it is on

void Toggle();
void End();
void FollowFocus();  // after the Page keys: the cursor goes to the thing they focused

}  // namespace nav::map
