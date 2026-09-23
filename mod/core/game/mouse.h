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
HWND GameWindow();

}  // namespace game
