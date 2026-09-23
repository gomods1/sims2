// The game's UI toolkit: Maxis' GZ framework windows (cGZWin and subclasses, the same toolkit as
// SimCity 4). Every window is reached through its cIGZWin interface pointer (what child lists
// hold). Screens are loaded from UI scripts in ui.package; the TS classes (cTSWinProc*) attach
// behaviour to them.
#pragma once

#include <windows.h>

#include <string>
#include <vector>

#include "memory.h"

namespace game {

// Window manager (cGZWinMgrW32), found through the framework's system services.
Ptr WinMgr();
Ptr MainWindow();
Ptr FocusWindow();    // where keys go (a text field while typing)
Ptr CaptureWindow();  // the window holding the mouse capture
int ModalCount();     // nesting of DoModalWin loops (dialogs that run their own loop)
Ptr ModalWindow();    // the innermost modal dialog, 0 when none

// Windows (cIGZWin*)
std::vector<Ptr> Children(Ptr w);   // front to back
Ptr Parent(Ptr w);
uint32_t WinId(Ptr w);
RECT Area(Ptr w);                    // in the parent's coordinates
RECT ScreenRect(Ptr w);              // in the main window's coordinates
bool Visible(Ptr w);                 // the window's own flag
bool Enabled(Ptr w);
bool ShownOnScreen(Ptr w);           // visible, and every ancestor too
std::string Caption(Ptr w);          // UTF-8 (often an English design-time name: see Label)
Ptr CompleteObject(Ptr w);           // the start of the C++ object (for class-specific fields)

// Buttons (cGZWinBtn and subclasses)
bool IsButton(Ptr w);
std::string Tooltip(Ptr w);          // UTF-8, "Title|Description" as the game stores it
bool ButtonOn(Ptr w);                // a toggle or radio button that is on
bool ButtonToggles(Ptr w);

// Text (cGZWinText, cGZWinTextEdit)
bool IsText(Ptr w);
bool IsTextEdit(Ptr w);
std::string TextEditText(Ptr w);    // UTF-8

// Presses a window the way a left click at its centre does: mouse enter, left down, left up,
// through its own handlers, so sounds, notifications and state changes all run.
bool Press(Ptr w);
bool PressAt(Ptr w, int x, int y);   // window coordinates

}  // namespace game
