// The keyboard focus the game does not have. Follows the window that owns input (a modal dialog
// or the main window), keeps a focused item, announces what appears and focus changes, and
// presses the focused window. All functions run on the game's main thread.
#pragma once

#include <windows.h>

#include "../game/memory.h"

namespace nav {

void Reset();
void Tick();  // about every 30 ms

// A text field has the keyboard: letters and Space belong to it, not to the mod.
bool TextEntryActive();

void Move(int delta);        // previous/next item in the current group (no wrap)
void MoveToEdge(bool last);  // first/last item in the current group
void NextGroup(int delta);   // previous/next group (wraps)
void Activate();             // press the focused item
void RepeatFocus();          // say the focused item again, with its group
game::Ptr FocusedWidget();
bool FocusedIsTextField();
bool FocusedIsSlider();
void AdjustFocused(int delta);  // Left/Right on a slider   // the focused item is a text field (or nothing is focused)

}  // namespace nav
