#include "keyboard.h"

#include <cstring>

#include "../game/gz.h"
#include "../game/mouse.h"
#include "../game/hooks.h"
#include "../nav/buffers.h"
#include "../nav/navigator.h"
#include "../nav/nhood_view.h"
#include "../nav/build.h"
#include "../nav/placement.h"
#include "../nav/live.h"
#include "../nav/world.h"
#include "../speech/speech.h"
#include "../util/log.h"
#include "../util/strings.h"

namespace input {
namespace {

// Keys the mod owns (see CLAUDE.md "Keys" for the game's own bindings they replace):
//   Up/Left, Down/Right    previous/next item in the group
//   Home, End              first/last item in the group
//   Tab, Shift+Tab         next/previous group
//   Enter                  press the focused item
//   R                      repeat the focused item (typing while a text field has the keyboard)
//   Ctrl+Up / Ctrl+Down    review buffers: next / previous line of the current buffer
//   Ctrl+Left / Ctrl+Right previous / next review buffer
//   Ctrl+F1                the mod's keys for this screen

bool g_consumed[256] = {};  // key-downs we took; their key-ups are taken too
bool g_worldFocus = false;  // Enter and R act on the object browser (after PageUp/PageDown)
WPARAM g_swallowChar = 0;   // character TranslateMessage will produce from the last claimed key

// Keys posted by tools/keys.py carry this reserved lParam bit; their modifiers are tracked from
// their own messages. Real keys read the physical keyboard.
constexpr LPARAM kSyntheticKey = 1 << 25;
constexpr LPARAM kGameKey = 1 << 26;  // posted by the mod for the game's own binding (PostGameKey)
bool g_syntheticDown[256] = {};
bool g_synthetic = false;
bool Held(int vk) {
    return g_synthetic ? g_syntheticDown[vk] : (game::RealGetAsyncKeyState(vk) & 0x8000) != 0;
}
bool IsModifier(WPARAM vk) { return vk == VK_SHIFT || vk == VK_CONTROL || vk == VK_MENU; }

// The last character of UTF-8 text (one code point), empty for empty text.
std::string LastUtf8Char(const std::string& s) {
    if (s.empty()) return "";
    size_t i = s.size() - 1;
    while (i > 0 && (static_cast<unsigned char>(s[i]) & 0xC0) == 0x80) i--;
    return s.substr(i);
}

// Echo while typing into a game field: the character typed, or the one Backspace deletes.
void SayCharacter(const std::string& utf8) {
    if (utf8.empty()) return;
    speech::Say(utf8 == " " ? S(Str::Space) : utf8);
}

bool Dispatch(WPARAM vk, bool shift, bool ctrl) {
    bool typing = nav::TextEntryActive();
    // The caret keys belong to the field only while the mod's focus is on it too (a screen can
    // keep a field focused all along, as Create a Family does with the surname).
    bool fieldFocus = typing && nav::FocusedIsTextField();
    bool lot = !typing && nav::world::LotViewActive();
    if (lot && nav::placement::Active()) {
        if (nav::placement::OnKey(vk, shift, ctrl)) {
            if (vk == 'R') g_swallowChar = 'r';
            if (vk == VK_RETURN) g_swallowChar = '\r';
            return true;
        }
        if (vk == VK_ESCAPE || vk == VK_OEM_COMMA || vk == VK_OEM_PERIOD) return false;
    }
    if (lot && nav::build::Active()) {
        if (nav::build::OnKey(vk, shift, ctrl)) {
            if (vk == 'R') g_swallowChar = 'r';
            if (vk == VK_RETURN) g_swallowChar = ctrl ? '\n' : '\r';
            if (vk == VK_SPACE) g_swallowChar = ' ';
            return true;
        }
        if (vk == VK_ESCAPE) return false;
    }
    if (vk != VK_PRIOR && vk != VK_NEXT && vk != VK_RETURN && vk != 'R') g_worldFocus = false;
    switch (vk) {
    case VK_PRIOR: case VK_NEXT:
        // The lot's objects (the game's own keys change floor; the control panel has buttons for that).
        if (!lot) return false;
        g_worldFocus = true;
        if (ctrl) nav::world::NextRoom(vk == VK_NEXT ? +1 : -1);
        else if (shift) nav::world::NextCategory(vk == VK_NEXT ? +1 : -1);
        else nav::world::NextObject(vk == VK_NEXT ? +1 : -1);
        return true;
    case VK_UP: if (ctrl) nav::buffers::Deeper(); else nav::Move(-1); return true;
    case VK_DOWN: if (ctrl) nav::buffers::TowardTop(); else nav::Move(+1); return true;
    case VK_LEFT:
        if (fieldFocus && !ctrl) return false;  // the caret
        if (!ctrl && nav::FocusedIsSlider()) {
            nav::AdjustFocused(-1);
            return true;
        }
        if (ctrl) nav::buffers::Switch(-1); else nav::Move(-1);
        return true;
    case VK_RIGHT:
        if (fieldFocus && !ctrl) return false;
        if (!ctrl && nav::FocusedIsSlider()) {
            nav::AdjustFocused(+1);
            return true;
        }
        if (ctrl) nav::buffers::Switch(+1); else nav::Move(+1);
        return true;
    case VK_HOME: case VK_END:
        if (fieldFocus) return false;
        nav::MoveToEdge(vk == VK_END);
        return true;
    case VK_TAB:
        g_swallowChar = '\t';
        nav::NextGroup(shift ? -1 : +1);
        return true;
    case VK_RETURN:
        if (ctrl && lot && g_worldFocus && !game::ModalWindow()) {
            g_swallowChar = '\n';  // Ctrl+Enter's character is a line feed
            nav::world::WalkTo(shift);
            return true;
        }
        // A text field's Enter is the game's (the cheat console runs its line), unless the mod's
        // focus is on another item (Create a Family keeps its surname field focused throughout).
        if (ctrl || (typing && nav::FocusedIsTextField())) return false;
        g_swallowChar = '\r';
        if (g_worldFocus && lot && !game::ModalWindow()) nav::world::Interact();
        else nav::Activate();
        return true;
    case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
        if (!ctrl || shift || !lot) return false;  // plain digits are the game's speeds
        g_swallowChar = vk;
        nav::live::SelectMember((int)vk - '1');
        return true;
    case 'N':
        if (!ctrl || shift) return false;
        g_swallowChar = 'n';
        nav::live::CycleNarrator();
        return true;
    case 'F':
        if (!ctrl || shift || !lot) return false;
        g_swallowChar = 'f';
        nav::live::ToggleFollow();
        return true;
    case 'E':
        if (!ctrl || shift || !lot) return false;
        g_swallowChar = 'e';
        nav::live::SayStatus();
        return true;
    case VK_F1:
        if (!ctrl) return false;  // F1 alone is the game's
        // The keys of what is in front of the player.
        if (lot && nav::placement::Active()) speech::Say(S(Str::KeysPlacement));
        else if (lot && nav::build::Active()) speech::Say(S(Str::KeysBuild));
        else if (lot) speech::Say(S(Str::KeysLot));
        else if (nav::neighborhood::ViewActive()) speech::Say(S(Str::KeysNeighborhood));
        else speech::Say(S(Str::KeysScreen));
        return true;
    case 'R':
        if (typing || shift || ctrl) return false;
        g_swallowChar = 'r';
        if (g_worldFocus && lot && !game::ModalWindow()) nav::world::Repeat();
        else nav::RepeatFocus();
        return true;
    default:
        return false;
    }
}

}  // namespace

bool OnKeyDown(WPARAM vk, LPARAM lp) {
    if (vk >= 256) return false;
    if (lp & kGameKey) return false;
    if (vk == VK_BACK && nav::TextEntryActive()) {
        // Said before the game deletes it (the caret is at the end while typing).
        SayCharacter(LastUtf8Char(game::TextEditText(game::FocusWindow())));
        return false;
    }
    LogVerbose("key down vk=0x%02x lparam=0x%08lx", (unsigned)vk, (unsigned long)lp);
    g_synthetic = (lp & kSyntheticKey) != 0;
    if (IsModifier(vk)) {
        if (g_synthetic) {
            g_syntheticDown[vk] = true;
            game::SetSyntheticModifier((int)vk, true);
        }
        return false;
    }
    bool ctrl = Held(VK_CONTROL);
    bool ctrlKey = vk == VK_F1 || vk == VK_UP || vk == VK_DOWN || vk == VK_LEFT || vk == VK_RIGHT || vk == 'E' || vk == 'F' || vk == 'N' || (vk >= '1' && vk <= '9') ||
                   vk == VK_PRIOR || vk == VK_NEXT || vk == VK_RETURN;
    if (Held(VK_MENU) || (ctrl && !ctrlKey)) return false;
    g_consumed[vk] = true;
    if (Dispatch(vk, Held(VK_SHIFT), ctrl)) return true;
    g_consumed[vk] = false;
    return false;
}

bool OnKeyUp(WPARAM vk, LPARAM lp) {
    if (vk >= 256) return false;
    if (lp & kGameKey) return false;
    if (IsModifier(vk)) {
        if (lp & kSyntheticKey) {
            g_syntheticDown[vk] = false;
            game::SetSyntheticModifier((int)vk, false);
        }
        return false;
    }
    if (!g_consumed[vk]) return false;
    g_consumed[vk] = false;
    return true;
}

bool OnChar(WPARAM ch) {
    bool letter = g_swallowChar >= 'a' && g_swallowChar <= 'z';
    bool swallow = g_swallowChar && (ch == g_swallowChar || (letter && (ch == g_swallowChar - 32 || ch == (g_swallowChar & 0x1f))));
    g_swallowChar = 0;
    if (!swallow && ch >= 32 && ch != 127 && nav::TextEntryActive()) {
        // The window is ANSI: the character comes in the system code page.
        char in[2] = {static_cast<char>(ch), 0};
        wchar_t wide[4] = {};
        char utf8[16] = {};
        if (MultiByteToWideChar(CP_ACP, 0, in, 1, wide, 4) > 0 &&
            WideCharToMultiByte(CP_UTF8, 0, wide, 1, utf8, sizeof utf8 - 1, nullptr, nullptr) > 0)
            SayCharacter(utf8);
    }
    return swallow;
}

void PostGameKey(WPARAM vk) {
    HWND hwnd = game::GameWindow();
    if (!hwnd) return;
    LPARAM lp = 1 | ((LPARAM)MapVirtualKeyA((UINT)vk, 0) << 16) | kGameKey;
    PostMessageA(hwnd, WM_KEYDOWN, vk, lp);
    PostMessageA(hwnd, WM_KEYUP, vk, lp | (1 << 30) | (1u << 31));
}

void OnFocusLost() { memset(g_syntheticDown, 0, sizeof g_syntheticDown); }

}  // namespace input
