#include "navigator.h"

#include <set>

#include "../game/gz.h"
#include "../game/rtti.h"
#include "../speech/speech.h"
#include "../util/log.h"
#include "../util/strings.h"
#include "../util/text.h"
#include "buffers.h"
#include "collect.h"
#include "describe.h"

namespace nav {
namespace {

constexpr ULONGLONG kRefreshMs = 250;   // re-collect the tree this often
constexpr ULONGLONG kSettleMs = 400;    // a new root or new panels must stay this long to be announced
constexpr ULONGLONG kPressFeedbackMs = 150;

struct State {
    Ptr root = 0;
    std::vector<Group> groups;
    Ptr focusWidget = 0;
    std::string focusLabel;
    int group = -1, index = -1;
    std::set<Ptr> knownGroups;     // group keys at the last announcement
    ULONGLONG lastCollect = 0;
    bool announcePending = false;  // root or panels changed; announce once settled
    ULONGLONG changedAt = 0;
    std::set<Ptr> announcedGroups; // groups whose texts were read since the last screen change
    Ptr pressed = 0;               // the last pressed widget, for its new state
    ULONGLONG pressedAt = 0;
    bool pressedWasOn = false;
};

State g;

const Item* Focused() {
    if (g.group < 0 || g.group >= (int)g.groups.size()) return nullptr;
    const auto& items = g.groups[g.group].items;
    return g.index >= 0 && g.index < (int)items.size() ? &items[g.index] : nullptr;
}

void SetFocus(int group, int index) {
    g.group = group;
    g.index = index;
    const Item* it = Focused();
    g.focusWidget = it ? it->widget : 0;
    g.focusLabel = it ? it->label : "";
}

bool Relocate() {
    for (int gi = 0; gi < (int)g.groups.size(); gi++)
        for (int ii = 0; ii < (int)g.groups[gi].items.size(); ii++) {
            const Item& it = g.groups[gi].items[ii];
            if (it.widget == g.focusWidget && (it.label == g.focusLabel || it.role == Role::TextField)) {
                g.group = gi;
                g.index = ii;
                return true;
            }
        }
    // Same widget with a new label (a value that changed): keep it.
    for (int gi = 0; gi < (int)g.groups.size(); gi++)
        for (int ii = 0; ii < (int)g.groups[gi].items.size(); ii++)
            if (g.groups[gi].items[ii].widget == g.focusWidget) {
                SetFocus(gi, ii);
                return true;
            }
    return false;
}

void FocusDefault() {
    for (int gi = 0; gi < (int)g.groups.size(); gi++)
        if (!IsTextOnly(g.groups[gi])) {
            SetFocus(gi, FirstControl(g.groups[gi]));
            return;
        }
    SetFocus(g.groups.empty() ? -1 : 0, g.groups.empty() ? -1 : 0);
}

void AnnounceFocus(bool withGroup) {
    const Item* it = Focused();
    if (!it) {
        speech::Say(S(Str::NothingHere));
        return;
    }
    const Group& group = g.groups[g.group];
    std::string prefix = withGroup && group.items.size() > 1 ? GroupSummary(group) + ". " : "";
    buffers::SetDetails(DetailLines(*it, group, g.index));
    speech::Say(prefix + Describe(*it));
}

void LogModel() {
    Log("screen: root %s %p, %d groups", game::ClassName(g.root).c_str(), (void*)g.root, (int)g.groups.size());
    for (const Group& group : g.groups) {
        Log("  group %p %s, %d items", (void*)group.key, GroupName(group).c_str(), (int)group.items.size());
        for (const Item& it : group.items)
            LogVerbose("    %s %p: %s", game::ClassName(it.widget).c_str(), (void*)it.widget, Describe(it).c_str());
    }
}

// Reads the texts of groups that appeared (a dialog's message, a panel's title) as events, then
// the focus. Called once the change has settled.
void Announce() {
    LogModel();
    for (const Group& group : g.groups) {
        if (g.announcedGroups.count(group.key)) continue;
        g.announcedGroups.insert(group.key);
        if (g.knownGroups.count(group.key)) continue;
        for (const Item& it : group.items)
            if (it.role == Role::Text && IsTextOnly(group)) buffers::Event(it.label);
    }
    // Only texts (a loading screen, a message without buttons): they were just read.
    bool anyControl = false;
    for (const Group& group : g.groups) anyControl = anyControl || !IsTextOnly(group);
    if (anyControl) AnnounceFocus(true);
}

void Refresh(ULONGLONG now) {
    Ptr root = InputRoot();
    if (root != g.root) {
        g.root = root;
        g.announcePending = true;
        g.changedAt = now;
        g.announcedGroups.clear();
        g.knownGroups.clear();
        buffers::ClearDetails();
    }
    g.groups = Collect(root);
    std::set<Ptr> keys;
    for (const Group& group : g.groups) keys.insert(group.key);
    int appeared = 0;
    for (Ptr k : keys)
        if (!g.knownGroups.count(k)) appeared++;
    if (appeared > 0 && !g.announcePending) {
        g.announcePending = true;
        g.changedAt = now;
    }
    bool focusKept = g.focusWidget && Relocate();
    if (!focusKept) FocusDefault();
    if (g.announcePending && now - g.changedAt >= kSettleMs) {
        g.announcePending = false;
        if (!focusKept || appeared * 2 >= (int)keys.size()) {
            if (appeared * 2 >= (int)keys.size()) FocusDefault();
            Announce();
        } else {
            // A panel appeared beside the focus: read what it says without moving focus.
            for (const Group& group : g.groups)
                if (!g.knownGroups.count(group.key) && IsTextOnly(group))
                    for (const Item& it : group.items) buffers::Event(it.label);
        }
        g.knownGroups = keys;
    } else if (!g.announcePending) {
        g.knownGroups = keys;
        if (!focusKept && g.focusWidget) AnnounceFocus(true);
    }
}

}  // namespace

void Reset() { g = State(); }

void Tick() {
    ULONGLONG now = GetTickCount64();
    if (g.pressed && now - g.pressedAt >= kPressFeedbackMs) {
        Ptr w = g.pressed;
        g.pressed = 0;
        const Item* it = Focused();
        if (it && it->widget == w && it->role == Role::Toggle && game::ButtonOn(w) != g.pressedWasOn)
            speech::Say(game::ButtonOn(w) ? S(Str::Selected) : S(Str::NotChecked));
    }
    if (now - g.lastCollect < kRefreshMs) return;
    g.lastCollect = now;
    Refresh(now);
}

bool TextEntryActive() { return game::IsTextEdit(game::FocusWindow()) && game::ShownOnScreen(game::FocusWindow()); }

void Move(int delta) {
    Refresh(GetTickCount64());
    if (g.group < 0) {
        speech::Say(S(Str::NothingHere));
        return;
    }
    int n = (int)g.groups[g.group].items.size();
    int next = g.index + delta;
    if (next < 0) next = 0;
    if (next >= n) next = n - 1;
    SetFocus(g.group, next);
    AnnounceFocus(false);
}

void MoveToEdge(bool last) {
    Refresh(GetTickCount64());
    if (g.group < 0) {
        speech::Say(S(Str::NothingHere));
        return;
    }
    SetFocus(g.group, last ? (int)g.groups[g.group].items.size() - 1 : 0);
    AnnounceFocus(false);
}

void NextGroup(int delta) {
    Refresh(GetTickCount64());
    int n = (int)g.groups.size();
    if (n == 0) {
        speech::Say(S(Str::NothingHere));
        return;
    }
    int next = ((g.group < 0 ? 0 : g.group) + delta + n) % n;
    SetFocus(next, FirstControl(g.groups[next]));
    AnnounceFocus(true);
}

void Activate() {
    Refresh(GetTickCount64());
    const Item* it = Focused();
    if (!it) {
        speech::Say(S(Str::NothingHere));
        return;
    }
    if (IsReadOnly(*it)) {
        AnnounceFocus(false);
        return;
    }
    if (!game::Enabled(it->widget)) {
        speech::Say(Describe(*it));
        return;
    }
    Log("press %s %p (%s)", game::ClassName(it->widget).c_str(), (void*)it->widget, it->label.c_str());
    g.pressed = it->widget;
    g.pressedAt = GetTickCount64();
    g.pressedWasOn = game::ButtonOn(it->widget);
    game::Press(it->widget);
}

void RepeatFocus() {
    Refresh(GetTickCount64());
    AnnounceFocus(true);
}

Ptr FocusedWidget() {
    const Item* it = Focused();
    return it ? it->widget : 0;
}

}  // namespace nav
