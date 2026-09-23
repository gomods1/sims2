#include "keyboard.h"

#include <cstring>

#include "../game/hooks.h"
#include "../nav/buffers.h"
#include "../nav/navigator.h"
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
WPARAM g_swallowChar = 0;   // character TranslateMessage will produce from the last claimed key

// Keys posted by tools/keys.py carry this reserved lParam bit; their modifiers are tracked from
// their own messages. Real keys read the physical keyboard.
constexpr LPARAM kSyntheticKey = 1 << 25;
bool g_syntheticDown[256] = {};
bool g_synthetic = false;
bool Held(int vk) {
    return g_synthetic ? g_syntheticDown[vk] : (game::RealGetAsyncKeyState(vk) & 0x8000) != 0;
}
bool IsModifier(WPARAM vk) { return vk == VK_SHIFT || vk == VK_CONTROL || vk == VK_MENU; }

bool Dispatch(WPARAM vk, bool shift, bool ctrl) {
    bool typing = nav::TextEntryActive();
    switch (vk) {
    case VK_UP: if (ctrl) nav::buffers::Deeper(); else nav::Move(-1); return true;
    case VK_DOWN: if (ctrl) nav::buffers::TowardTop(); else nav::Move(+1); return true;
    case VK_LEFT:
        if (typing && !ctrl) return false;  // the caret
        if (ctrl) nav::buffers::Switch(-1); else nav::Move(-1);
        return true;
    case VK_RIGHT:
        if (typing && !ctrl) return false;
        if (ctrl) nav::buffers::Switch(+1); else nav::Move(+1);
        return true;
    case VK_HOME: case VK_END:
        if (typing) return false;
        nav::MoveToEdge(vk == VK_END);
        return true;
    case VK_TAB:
        g_swallowChar = '\t';
        nav::NextGroup(shift ? -1 : +1);
        return true;
    case VK_RETURN:
        if (ctrl || typing) return false;  // a text field's Enter is the game's (the cheat console runs its line)
        g_swallowChar = '\r';
        nav::Activate();
        return true;
    case VK_F1:
        if (!ctrl) return false;  // F1 alone is the game's
        speech::Say(S(Str::KeysScreen));
        return true;
    case 'R':
        if (typing || shift || ctrl) return false;
        g_swallowChar = 'r';
        nav::RepeatFocus();
        return true;
    default:
        return false;
    }
}

}  // namespace

bool OnKeyDown(WPARAM vk, LPARAM lp) {
    if (vk >= 256) return false;
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
    bool ctrlKey = vk == VK_F1 || vk == VK_UP || vk == VK_DOWN || vk == VK_LEFT || vk == VK_RIGHT;
    if (Held(VK_MENU) || (ctrl && !ctrlKey)) return false;
    g_consumed[vk] = true;
    if (Dispatch(vk, Held(VK_SHIFT), ctrl)) return true;
    g_consumed[vk] = false;
    return false;
}

bool OnKeyUp(WPARAM vk, LPARAM lp) {
    if (vk >= 256) return false;
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
    return swallow;
}

void OnFocusLost() { memset(g_syntheticDown, 0, sizeof g_syntheticDown); }

}  // namespace input
