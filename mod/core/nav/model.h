// What the navigator moves over: items (navigable windows) in groups (their container).
#pragma once

#include <windows.h>

#include <string>
#include <vector>

#include "../game/memory.h"

namespace nav {

using game::Ptr;

enum class Role {
    Button,     // a pressable window with a label
    Toggle,     // a button that stays on or off (a tab, a check box, an option)
    Text,       // read-only text: titles, messages, names
    TextField,  // a text field the player types into
    Lot,        // a lot on the neighborhood map (number = lot id): Enter opens its info panel
};

struct Item {
    Ptr widget = 0;
    Role role = Role::Button;
    std::string label;   // UTF-8, what is spoken first
    std::string detail;  // the rest of a tooltip ("Title|Description": the description)
    RECT rect = {};      // main window coordinates
    int number = 0;      // Role::Lot: the lot id
    int family = 0;      // Role::Lot: the family living there (its id), 0 when none
};

struct Group {
    Ptr key = 0;         // the container window, or a kind of virtual group (below)
    std::string name;    // virtual groups: their name
    std::vector<Item> items;
    RECT bounds = {};
};

// Groups the mod builds from game state rather than from windows.
constexpr Ptr kLotsGroup = 1;

inline bool IsReadOnly(const Item& it) { return it.role == Role::Text; }

// Index of the first item that is not text; -1 when the group only holds text. Out of line: an
// inlined copy inside a loop over groups was miscompiled by MSVC 14.44 /O2 (it read the items
// pointer from an unwritten stack slot; crash in FocusDefault, 2026-09-23).
int FirstControlOrNone(const Group& g);
inline bool IsTextOnly(const Group& g) { return FirstControlOrNone(g) < 0; }
inline int FirstControl(const Group& g) {
    int i = FirstControlOrNone(g);
    return i < 0 ? 0 : i;
}

}  // namespace nav
