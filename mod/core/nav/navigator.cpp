#include "navigator.h"

#include <set>

#include <algorithm>

#include "../game/addresses.h"
#include "../game/cheats.h"
#include "../game/gz.h"
#include "../game/mouse.h"
#include "../game/rtti.h"
#include "../speech/speech.h"
#include "../util/log.h"
#include "../util/strings.h"
#include "../util/text.h"
#include "buffers.h"
#include "collect.h"
#include "describe.h"
#include "nhood_view.h"
#include "build.h"
#include "console.h"
#include "placement.h"
#include "live.h"
#include "world.h"

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
    Ptr dragFrom = 0;              // the family tree: the Sim picked up to drop on another
    ULONGLONG pressedAt = 0;
    bool pressedWasOn = false;
    std::vector<std::pair<Ptr, std::string>> textsBefore;  // the screen's texts before the press
    // A catalog page was turned: once the thumbnails show it, focus its first (last) product.
    Ptr pageGroup = 0;
    int pageDelta = 0;
    ULONGLONG pageAt = 0;
};

using util::AppendSentence;

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
            LogVerbose("    %s %p [%ld,%ld,%ld,%ld]: %s", game::ClassName(it.widget).c_str(), (void*)it.widget, it.rect.left,
                       it.rect.top, it.rect.right, it.rect.bottom, Describe(it).c_str());
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
        if (group.key < 0x10000) continue;  // the mod's own groups (needs, queue, lots) are no news
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
    // The mod's own groups (needs, action queue, lots) come and go with the game's state: they
    // are no new panel, and never take the focus; the interaction menu is one (it is news).
    int appeared = 0;
    for (Ptr k : keys)
        if (!g.knownGroups.count(k) && (k >= 0x10000 || k == kPieGroup)) appeared++;
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
                if (g.knownGroups.count(group.key) || group.key < 0x10000) continue;
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

// The catalog's page buttons (window ids inside cTSWinProcCatalog).
constexpr uint32_t kCatalogNext = 0x1005;
constexpr uint32_t kCatalogPrevious = 0x1004;

void FinishPageTurn(ULONGLONG now);

void Reset() { g = State(); }

// Create a Family's family tree ("drag a Sim onto another"): its instruction text has this id,
// its Sims are buttons with id 3.
constexpr uint32_t kFamilyTreeText = 0x8cdd9478;
bool InFamilyTree(Ptr w) {
    if (game::WinId(w) != 3 || !game::IsButton(w)) return false;
    for (Ptr p = game::Parent(w); p; p = game::Parent(p))
        for (Ptr c : game::Children(p))
            if (game::WinId(c) == kFamilyTreeText) return true;
    return false;
}

void Tick() {
    game::MouseTick();
    ULONGLONG now = GetTickCount64();
    if (game::g_screenDumpRequested) {
        game::g_screenDumpRequested = false;
        Refresh(now);
        for (const Group& group : g.groups) {
            Log("  group %p %s, %d items", (void*)group.key, GroupName(group).c_str(), (int)group.items.size());
            for (const Item& it : group.items)
                Log("    %s  [%ld,%ld,%ld,%ld]", Describe(it).c_str(), it.rect.left, it.rect.top, it.rect.right, it.rect.bottom);
        }
        const Item* f = Focused();
        Log("  focus: %s", f ? Describe(*f).c_str() : "none");
    }
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
                    if (t.role != Role::Text || group.key < 0x10000) continue;  // not the mod's own groups
                    bool same = false;
                    for (const auto& before : g.textsBefore)
                        if (before.first == t.widget && before.second == t.label) same = true;
                    if (!same) AppendSentence(changed, t.label);
                }
            if (!changed.empty()) buffers::Event(changed);
            Relocate();
        }
    }
    FinishPageTurn(now);
    world::Tick();
    live::Tick();
    placement::Tick();
    build::Tick();
    console::Tick();
    if (now - g.lastCollect < kRefreshMs) return;
    g.lastCollect = now;
    Refresh(now);
}

// Typing goes to a text field: the focus window is one the player can edit.
bool TextEntryActive() {
    Ptr focus = game::FocusWindow();
    bool typing = focus && game::TextEditEditable(focus) && game::ShownOnScreen(focus);
    LogVerbose("focus %s %p: %s", game::ClassName(focus).c_str(), (void*)focus, typing ? "typing" : "not typing");
    return typing;
}

// A visible, enabled button with this window id under w (the catalog's page buttons).
Ptr FindButtonById(Ptr w, uint32_t id, int depth) {
    if (!w || depth > 8) return 0;
    for (Ptr c : game::Children(w)) {
        if (!game::Visible(c)) continue;
        if (game::IsButton(c) && game::WinId(c) == id && game::Enabled(c)) return c;
        if (Ptr f = FindButtonById(c, id, depth + 1)) return f;
    }
    return 0;
}

// Past the catalog's last product (or before its first): its next (previous) page, if any.
bool TurnCatalogPage(int delta) {
    const Group& group = g.groups[g.group];
    if (group.proc != "cTSWinProcCatalog") return false;
    Ptr button = FindButtonById(group.key, delta > 0 ? kCatalogNext : kCatalogPrevious, 0);
    if (!button) return false;
    Log("catalog: %s page", delta > 0 ? "next" : "previous");
    game::Press(button);
    g.pageGroup = group.key;
    g.pageDelta = delta;
    g.pageAt = GetTickCount64();
    return true;
}

// The turned page's first (or last) product, once the catalog has refilled its thumbnails.
void FinishPageTurn(ULONGLONG now) {
    if (!g.pageGroup || now - g.pageAt < 200) return;
    Ptr key = g.pageGroup;
    int delta = g.pageDelta;
    g.pageGroup = 0;
    g.groups = Collect(g.root);
    for (int gi = 0; gi < (int)g.groups.size(); gi++) {
        if (g.groups[gi].key != key) continue;
        const std::vector<Item>& items = g.groups[gi].items;
        int pick = -1;
        for (int i = 0; i < (int)items.size(); i++)
            if (items[i].role == Role::Product && (pick < 0 || delta < 0)) pick = i;
        if (pick < 0) return;
        SetFocus(gi, pick);
        const Item& it = items[pick];
        buffers::SetDetails(DetailLines(it, g.groups[gi], pick));
        speech::Say(std::string(S(delta > 0 ? Str::NextPage : Str::PreviousPage)) + ". " + Describe(it));
        return;
    }
}

void Move(int delta) {
    Refresh(GetTickCount64());
    if (g.group < 0) {
        speech::Say(S(Str::NothingHere));
        return;
    }
    int n = (int)g.groups[g.group].items.size();
    if ((g.index + delta >= n || g.index + delta < 0) && TurnCatalogPage(delta)) return;
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
    if (it->role == Role::QueuedAction) {
        live::CancelQueued(*it);
        return;
    }
    if (it->role == Role::TextField) {
        // A click gives a text field the keyboard (a press through its slots does not).
        Log("click text field %p", (void*)it->widget);
        game::ClickWindow(it->widget);
        return;
    }
    if (it->role == Role::PieItem) {
        Log("choose \"%s\" in the interaction menu", it->label.c_str());
        game::ClickWindow(it->widget);
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
    // The family tree: Enter picks a Sim up, Enter on another drops it there (the game then asks
    // how they are related); Enter on the same one again puts it down.
    if (InFamilyTree(it->widget)) {
        if (!g.dragFrom || !game::Visible(g.dragFrom)) {
            g.dragFrom = it->widget;
            speech::Say(util::Format(S(Str::FamilyTreePicked), it->label.c_str()));
        } else if (g.dragFrom == it->widget) {
            g.dragFrom = 0;
            speech::Say(S(Str::Cancelled));
        } else {
            Log("family tree: drag %s onto %s", game::Caption(g.dragFrom).c_str(), it->label.c_str());
            game::DragWindow(g.dragFrom, it->widget);
            g.dragFrom = 0;
        }
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
    if (it->role == Role::Product && it->buyObject) placement::Begin(it->label);
    else if (it->role == Role::Product) build::Begin(it->label);
}

void RepeatFocus() {
    Refresh(GetTickCount64());
    AnnounceFocus(true);
}

bool FocusedIsSlider() {
    const Item* it = Focused();
    return it && it->role == Role::Slider;
}

// The points still to give on Create a Sim's personality page (its "total" bar), -1 if none.
int PointsLeft(Ptr bar) {
    for (Ptr s : game::Children(game::Parent(bar))) {
        int value, steps;
        if (game::Caption(s) == "total" && game::StepBar(s, &value, &steps)) {
            Ptr proc = game::Proc(s);
            return game::Rd<int>(proc + game::addr::stepbar::kValue) / 100;
        }
    }
    return -1;
}

void AdjustFocused(int delta) {
    Refresh(GetTickCount64());
    const Item* it = Focused();
    if (!it || it->role != Role::Slider) return;
    Ptr w = it->widget;
    int step, steps;
    if (!game::StepBar(w, &step, &steps)) return;
    int before = step;
    int left = PointsLeft(w);  // the total bar follows a change a little later: counted here
    int target = std::clamp(step + delta, 0, steps);
    if (left == 0 && target > step) target = step;  // no point left to give
    if (target != step) game::SetStepBar(w, target);
    game::StepBar(w, &step, &steps);
    std::string line = util::Format(S(Str::ValueOfMax), step, steps);
    if (left >= 0) line += ", " + util::Format(S(Str::PointsLeft), left + before - step);
    Log("slider %p: %d -> %d of %d (asked %d)", (void*)w, before, step, steps, target);
    speech::Say(line);
}

bool FocusedIsTextField() {
    const Item* it = Focused();
    return !it || it->role == Role::TextField;
}

Ptr FocusedWidget() {
    const Item* it = Focused();
    return it ? it->widget : 0;
}

}  // namespace nav
