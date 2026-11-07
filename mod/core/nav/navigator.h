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
void JumpToGroup(char letter);  // Alt+letter: the next group whose spoken name starts with it
void ToggleShowFocus();         // Ctrl+Shift+V: the focused control lit on screen, for a sighted helper
// A caret key went to the text field that has the keyboard: the character (or word) the caret
// lands on is spoken once the game has moved it. Delete: the character it is about to remove.
void NoteCaretKey(bool word);
void SayCharacterAtCaret();
void Activate();             // press the focused item
void RepeatFocus();          // say the focused item again, with its group
bool Back();                 // Backspace: up one level (a kind's or room's things back to the
                             // kinds or rooms); false when the focus is at a top level
// PageUp/PageDown in the lot view: the next thing of the kind or room chosen last (Thing), the
// next kind (Shift) or room (Ctrl), opened in its group with focus on its first thing.
enum class Browse { Thing, Kind, Room };
void BrowseThings(Browse how, int delta);

// Ctrl+F (lot view): find a thing by typing part of its name. While active, typing goes to the
// search, Up/Down (Page keys, Home/End) go through what matches, Enter acts on it as on any thing
// of the lot, Ctrl+Enter walks there, Escape goes back to where the focus was. The matches are a
// list of the "things by kind" group, so everything else goes on from there once it is over.
bool FindActive();
void FindBegin();
void FindChar(WPARAM ch);    // a typed character (the window's ANSI, a double-byte one in two calls)
void FindErase();            // Backspace
void FindMove(int delta);
void FindEdge(bool last);
void FindConfirm(bool walk);
void FindCancel();
void FindEnd();              // over, with the matches left as they are (another key took over)
bool FocusOnThing();         // the focus is on a thing of the lot

game::Ptr FocusedWidget();
bool FocusedIsTextField();
bool FocusedIsSlider();
void AdjustFocused(int delta);  // Left/Right on a slider

}  // namespace nav
