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
#include "nhood_view.h"

namespace nav {
namespace {

constexpr ULONGLONG kRefreshMs = 250;   // re-collect the tree this often
constexpr ULONGLONG kSettleMs = 400;    // a new root or new panels must stay this long to be announced
constexpr ULONGLONG kPressFeedbackMs = 150;

struct State {
    Ptr root = 0;
    std::vector<Group> groups;
    Ptr focusWidget = 0;
    int focusNumber = 0;           // virtual items (lots) have no widget: their number
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
    std::vector<std::pair<Ptr, std::string>> textsBefore;  // the screen's texts before the press
};

// Joins texts into one line, with a stop between them unless one is already there.
void AppendSentence(std::string& out, const std::string& s) {
    if (s.empty()) return;
    if (!out.empty()) {
        char last = out.back();
        out += (last == '.' || last == '!' || last == '?' || last == ':') ? " " : ". ";
    }
    out += s;
}

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
    g.focusNumber = it ? it->number : 0;
    g.focusLabel = it ? it->label : "";
}

bool Relocate() {
    for (int gi = 0; gi < (int)g.groups.size(); gi++)
        for (int ii = 0; ii < (int)g.groups[gi].items.size(); ii++) {
            const Item& it = g.groups[gi].items[ii];
            if (it.widget == g.focusWidget && it.number == g.focusNumber &&
                (it.label == g.focusLabel || it.role == Role::TextField || !it.widget)) {
                g.group = gi;
                g.index = ii;
                return true;
            }
        }
    // Same widget with a new label (a value that changed): keep it.
    for (int gi = 0; gi < (int)g.groups.size(); gi++)
        for (int ii = 0; ii < (int)g.groups[gi].items.size(); ii++)
            if (g.groups[gi].items[ii].widget == g.focusWidget && g.groups[gi].items[ii].number == g.focusNumber) {
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
    // A dialog: its texts (title, message) first, in reading order.
    if (g.root != game::MainWindow() && !g.announcedGroups.count(g.root)) {
        std::string texts;
        for (const Group& group : g.groups)
            for (const Item& it : group.items)
                if (it.role == Role::Text) AppendSentence(texts, it.label);
        if (!texts.empty()) buffers::Event(texts);
        for (const Group& group : g.groups) g.announcedGroups.insert(group.key);
        g.announcedGroups.insert(g.root);
        AnnounceFocus(false);
        return;
    }
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
    bool focusKept = (g.focusWidget || g.focusNumber) && Relocate();
    if (!focusKept) FocusDefault();
    if (g.announcePending && now - g.changedAt >= kSettleMs) {
        g.announcePending = false;
        if (!focusKept || appeared * 2 >= (int)keys.size()) {
            if (appeared * 2 >= (int)keys.size()) FocusDefault();
            Announce();
        } else {
            // A panel appeared beside the focus (the lot panel, a subpanel): its texts are read,
            // and focus moves to it when it has controls.
            int panel = -1;
            std::string texts;
            for (int gi = 0; gi < (int)g.groups.size(); gi++) {
                const Group& group = g.groups[gi];
                if (g.knownGroups.count(group.key)) continue;
                for (const Item& it : group.items)
                    if (it.role == Role::Text) AppendSentence(texts, it.label);
                if (panel < 0 && !IsTextOnly(group)) panel = gi;
            }
            if (!texts.empty()) buffers::Event(texts);
            if (panel >= 0) {
                SetFocus(panel, FirstControl(g.groups[panel]));
                AnnounceFocus(false);
            }
        }
        g.knownGroups = keys;
    } else if (!g.announcePending) {
        g.knownGroups = keys;
        const Item* it = Focused();
        if (!focusKept && it && !IsReadOnly(*it)) AnnounceFocus(true);
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
        // Texts the press changed on the same screen (the next page of a story, a new value).
        if (InputRoot() == g.root) {
            g.groups = Collect(g.root);
            std::string changed;
            for (const Group& group : g.groups)
                for (const Item& t : group.items) {
                    if (t.role != Role::Text) continue;
                    bool same = false;
                    for (const auto& before : g.textsBefore)
                        if (before.first == t.widget && before.second == t.label) same = true;
                    if (!same) AppendSentence(changed, t.label);
                }
            if (!changed.empty()) buffers::Event(changed);
            Relocate();
        }
    }
    if (now - g.lastCollect < kRefreshMs) return;
    g.lastCollect = now;
    Refresh(now);
}

// Typing goes to a text field: the focus window is one, or holds one (the cheat console keeps
// the focus on its container, which forwards keys to its input line).
bool TextEntryActive() {
    Ptr focus = game::FocusWindow();
    if (!focus || !game::ShownOnScreen(focus)) return false;
    if (game::IsTextEdit(focus)) return true;
    for (Ptr child : game::Children(focus))
        if (game::IsTextEdit(child) && game::Visible(child) && game::Enabled(child)) return true;
    return false;
}

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
    if (it->role == Role::Lot) {
        Log("open lot %d (family %d)", it->number, it->family);
        neighborhood::OpenLot(*it);
        return;
    }
    if (!game::Enabled(it->widget)) {
        speech::Say(Describe(*it));
        return;
    }
    Log("press %s %p (%s)", game::ClassName(it->widget).c_str(), (void*)it->widget, it->label.c_str());
    g.textsBefore.clear();
    for (const Group& group : g.groups)
        for (const Item& t : group.items)
            if (t.role == Role::Text) g.textsBefore.push_back({t.widget, t.label});
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
