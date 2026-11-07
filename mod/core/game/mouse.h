// A virtual mouse for the game: clicks in the 3D view without moving the player's real cursor.
// While it is in use, GetCursorPos tells the game the virtual position and the game's own
// SetCursorPos calls (the pie menu centers the cursor on itself) move the virtual cursor only.
#pragma once

#include <windows.h>

namespace game {

void InitMouse(HWND gameWindow);  // after InitHooks (MinHook)

// Client coordinates of the game window (its full size, e.g. 2560x1600).
void MouseMove(int x, int y);
void MouseClick(int x, int y, bool right = false);
// The virtual cursor stays in place until released (the game polls it between messages).
void MouseRelease();
bool MouseActive();
// A drag with the left button from one client point to another, over the next ticks (MouseTick,
// once per tick): press, a few moves on the way, release.
void MouseDrag(int x0, int y0, int x1, int y1);
bool MouseDragging();
void MouseTick();
HWND GameWindow();
// The player's real pointer (screen coordinates), past the hook; refused while the virtual mouse
// is in use. For a sighted helper (Ctrl+Shift+V), never in the mod's own tests.
bool MoveRealPointer(int screenX, int screenY);

}  // namespace game
