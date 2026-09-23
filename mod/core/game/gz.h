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
uint32_t ButtonFlags(Ptr w);  // GetBtnFlag bits (game/addresses.h btn::kFlags)
bool CaptionShown(Ptr w);            // the button draws its caption (a value such as the funds)
std::string ProcClass(Ptr w);        // the class of the window's proc (cTSWinProc*), empty if none
Ptr Proc(Ptr w);                     // the window's proc object, 0 if none
// A meter (a window with a cTSWinProcGradientFillBar): its fill as 0..100; false if none.
bool MeterPercent(Ptr w, int* percent);

// A list box (cTSWinListBox): its rows as text (cells joined), the row windows (a press selects
// one), and which are selected. False when w is no list box.
struct ListRow {
    Ptr window = 0;
    std::string text;  // UTF-8
    bool selected = false;
};
bool ListBoxRows(Ptr w, std::vector<ListRow>* rows);
// A step bar (cTSWinProcStepFillBar): its value in steps and its number of steps.
bool StepBar(Ptr w, int* value, int* steps);
// Sets a step bar to a step (0..steps) by a click on the bar, as the mouse does.
bool SetStepBar(Ptr w, int step);

// Text (cGZWinText, cGZWinTextEdit)
bool IsText(Ptr w);
bool IsTextEdit(Ptr w);
std::string TextEditText(Ptr w);    // UTF-8
// A long text field (a console's output): its length, and the text from an offset (max bytes).
uint32_t TextEditLength(Ptr w);
std::string TextEditTextFrom(Ptr w, uint32_t from, uint32_t max);
bool TextEditEditable(Ptr w);        // the player can type into it

// Presses a window the way a left click at its centre does: mouse enter, left down, left up,
// through its own handlers, so sounds, notifications and state changes all run.
bool Press(Ptr w);
// Clicks the centre of a window with the virtual mouse (game/mouse.h): for windows whose proc
// takes the click from the window manager's mouse messages (pie menu entries).
bool ClickWindow(Ptr w);
// Drags one window onto another with the virtual mouse (over the next ticks: game::MouseTick).
bool DragWindow(Ptr from, Ptr to);
// UI coordinates (the main window's, 1478x924 on a 2560x1600 client) to client pixels.
POINT UiToClient(int x, int y);
bool PressAt(Ptr w, int x, int y);   // window coordinates

}  // namespace game
