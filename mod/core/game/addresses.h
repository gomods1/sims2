// Every address and offset the mod uses in Sims2EP9.exe 1.18.0.187 (SHA-256 24041f83..., The Sims 2
// Legacy Collection, Steam). Preferred-base VAs (image base 0xA0000000); game::Va() relocates them.
// Evidence for each is in PATCH_TARGETS.md and STRUCTS.md.
#pragma once

#include <cstdint>

namespace game::addr {

// The GZ framework singleton (cCTGFramework, static in .data). Slot 5 is
// GetSystemService(serviceId, iid, void** out), thiscall, ret 0xc (0xa01428e2).
constexpr uintptr_t kFramework = 0xa10fdb50;
constexpr int kFrameworkGetSystemService = 5;

// The window manager service (cGZWinMgrW32): service id at wm+0x10, QueryInterface accepts 0x5a4.
constexpr uint32_t kWinMgrServiceId = 0xa417445e;
constexpr uint32_t kWinMgrIid = 0x5a4;

namespace wm {
constexpr int kCapture = 0x34;     // GZGetCapture (slot 29)
constexpr int kFocus = 0x38;       // GZGetFocus (slot 37): the window keys go to
constexpr int kMouseWin = 0x40;    // the window under the mouse (slot 25)
constexpr int kMainWindow = 0x44;  // GetMainWindow (slot 3)
constexpr int kModalCount = 0x190; // GetModalNestCount (slot 44)
constexpr int kSlotDoModal = 42;   // DoModalWin(cIGZWin*), thiscall (0xa0972f48)
}  // namespace wm

// cGZWin, through the cIGZWin interface pointer (the one child lists hold). Fields:
namespace win {
constexpr int kManager = 0x04;
constexpr int kId = 0x10;
constexpr int kChildren = 0x54;    // -> container; [container] = std::list head; node: next, prev, cIGZWin*
constexpr int kParent = 0x58;
constexpr int kCaption = 0x60;     // cRZString: vtable, then std::string (UTF-8) at +0x64
constexpr int kArea = 0xc8;        // l, t, r, b in the parent's coordinates
constexpr int kFlags = 0xe8;       // 1 visible, 2 enabled
constexpr uint32_t kFlagVisible = 1;
constexpr uint32_t kFlagEnabled = 2;
// Virtual slots (cIGZWin, matching SimCity 4's gzcom headers from GetWindowManager on; verified
// by disassembly of cGZWin's vtable 0xa0e52f54).
constexpr int kSlotMouseDownL = 155;  // (x, y, flags): window coordinates
constexpr int kSlotMouseUpL = 157;    // (x, y, flags); a button notifies its click here
constexpr int kSlotMouseEnter = 161;
constexpr int kSlotMouseExit = 162;
constexpr int kSlotKeyDown = 151;     // (vk, modifiers)
}  // namespace win

// cGZWinBtn, from the complete object (the cIGZWinBtn interface is at +0, cGZWin at +4).
namespace btn {
constexpr int kFlags = 0xfc;    // GetBtnFlag (0xa0936a5c): 1 on, 2 toggles on click, 0x800 stays on
                                // (radio), 0x1000 held down by the mouse
constexpr uint32_t kFlagOn = 1;
constexpr uint32_t kFlagToggle = 2;
constexpr int kStyle = 0x160;
constexpr int kTip = 0x188;     // cRZString tooltip ("Title|Description"), std::string at +0x18c
}  // namespace btn

// cGZWinTextEdit, from the cIGZWin pointer: the text as a cRZString (std::string at +0x100).
namespace edit {
constexpr int kText = 0xfc;
}  // namespace edit

}  // namespace game::addr
