// Hooks into the game (MinHook). Every hook is optional: a failed one is logged and its feature
// stays off.
#pragma once

#include <windows.h>

#include <vector>

#include "memory.h"

namespace game {

void InitHooks();
void ShutdownHooks();

// Modal windows the game is running (cGZWinMgrW32::DoModalWin), innermost last. Their loops run
// inside the game's message pump, so the mod keeps ticking while one is open.
const std::vector<Ptr>& ModalStack();

// Posted test keys (tools/keys.py): the game sees their modifiers as held.
void SetSyntheticModifier(int vk, bool down);

// The real GetAsyncKeyState (without the test keys' modifiers).
short RealGetAsyncKeyState(int vk);

}  // namespace game
