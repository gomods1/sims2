#include "navigator.h"

#include <set>

#include <algorithm>

#include "../game/addresses.h"
#include "../game/buy.h"
#include "../game/cheats.h"
#include "../game/gz.h"
#include "../game/mouse.h"
#include "../game/rtti.h"
#include "../input/keyboard.h"
#include "../speech/speech.h"
#include "../util/log.h"
#include "../util/settings.h"
#include "../util/strings.h"
#include "../util/text.h"
#include "buffers.h"
#include "cues.h"
#include "manual.h"
#include "map.h"
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
constexpr ULONGLONG kToolPressMs = 700;  // a new catalog panel has settled and been announced by then

struct State {
    Ptr root = 0;
    std::vector<Group> groups;
    Ptr focusWidget = 0;
    int focusNumber = 0;           // virtual items (lots) have no widget: their number
    Ptr focusGroupKey = 0;         // the focused item's group (virtual items are found in it)
    std::string focusLabel;
    int group = -1, index = -1;
    std::set<Ptr> knownGroups;     // group keys at the last announcement
    ULONGLONG lastCollect = 0;
    bool announcePending = false;  // root or panels changed; announce once settled
    ULONGLONG changedAt = 0;
    std::set<Ptr> announcedGroups; // groups whose texts were read since the last screen change
    Ptr pressed = 0;               // the last pressed widget, for its new state
    Ptr dragFrom = 0;              // the family tree: the Sim picked up to drop on another
    bool hadPie = false;           // the interaction menu was up at the last refresh
    struct {
        Ptr widget = 0;
        int number = 0;
        std::string label;
        Ptr groupKey = 0;
        int index = -1;
    } beforePie;                   // the focus when the interaction menu opened
    ULONGLONG pressedAt = 0;
    bool pressedWasOn = false;
    std::vector<std::pair<Ptr, std::string>> textsBefore;  // the screen's texts before the press
    // A catalog page was turned: once the thumbnails show it, focus its first (last) product.
    Ptr pageGroup = 0;
    int pageDelta = 0;
    ULONGLONG pageAt = 0;
    // A build tool was pressed: once the catalog shows what it offers, focus goes to its styles.
    ULONGLONG toolAt = 0;
    std::string toolName, toolBefore;
    bool catalogNamed = false;
};

using util::AppendSentence;

State g;

const Item* Focused() {
    if (g.group < 0 || g.group >= (int)g.groups.size()) return nullptr;
    const auto& items = g.groups[g.group].items;
    return g.index >= 0 && g.index < (int)items.size() ? &items[g.index] : nullptr;
}

// ---- The focus on screen, for a sighted helper (Ctrl+Shift+V) ---------------------------------
// The focused control gets the mouse-enter a real pointer would give it, so the game draws it lit
// as under the mouse (in the Sims 1 mod a tester's partner could not tell what was selected). Off
// unless asked for: hovering can show tooltips.

int g_showFocus = -1;  // -1 not read yet
Ptr g_hovered = 0;

bool ShowFocus() {
    if (g_showFocus < 0) g_showFocus = util::GetSetting("show_focus", 0) ? 1 : 0;
    return g_showFocus == 1;
}

bool StillListed(Ptr w) {
    for (const Group& gr : g.groups)
        for (const Item& it : gr.items)
            if (it.widget == w) return true;
    return false;
}

// A thing of the lot focused while the focus is shown: the camera looks at it and the mouse pointer
// goes to the middle of the game's window, over it (in the Sims 1 mod a tester's partner saw the
// pointer resting on a vegetable cart while the mod had the cactus beside it). Live mode only (in
// buy and build mode the pointer carries the tools), and only while the game's window is in front.
int g_pointedAt = 0;
void PointAtThing(const Item* it) {
    if (!ShowFocus() || !it || it->role != Role::LotObject || it->number == g_pointedAt) return;
    if (game::CurrentModeClass() != "cTSUIModeLive" || placement::Active() || build::Active()) return;
    HWND hwnd = game::GameWindow();
    if (!hwnd || GetForegroundWindow() != hwnd) return;
    game::LotObject o;
    if (!game::ReadLotObject(it->number, &o) || o.x < 0) return;
    if (!game::CenterCamera(o.x, o.y, true)) return;
    g_pointedAt = it->number;
    RECT r;
    if (!GetClientRect(hwnd, &r)) return;
    POINT p{(r.right - r.left) / 2, (r.bottom - r.top) / 2};
    ClientToScreen(hwnd, &p);
    game::MoveRealPointer(p.x, p.y);
}

void HoverFocused(const Item* it) {
    PointAtThing(it);
    bool lit = ShowFocus() && it && it->widget && it->role != Role::PieItem && it->role != Role::ListRow;
    Ptr want = lit ? it->widget : 0;
    if (want == g_hovered) return;
    if (g_hovered && StillListed(g_hovered)) game::Unhover(g_hovered);  // a window gone may be freed
    g_hovered = want;
    if (want && game::Visible(want)) game::Hover(want);
}

// ---- Caret keys on a text field --------------------------------------------------------------
// Left/Right (Ctrl: by word), Home and End go to the field; once the game has moved its caret,
// the character (or word) there is spoken.
struct PendingCaret {
    Ptr field = 0;
    bool word = false;
    int ticks = 0;
} g_caret;

// The UTF-8 character starting at (or containing) byte `at`, empty at the end.
std::string CharacterAt(const std::string& text, int at) {
    if (at < 0 || at >= (int)text.size()) return "";
    while (at > 0 && ((unsigned char)text[at] & 0xc0) == 0x80) at--;
    int n = 1;
    while (at + n < (int)text.size() && ((unsigned char)text[at + n] & 0xc0) == 0x80) n++;
    return text.substr(at, n);
}

std::string SpokenCharacter(const std::string& c) {
    if (c.empty() || c == "\r" || c == "\n") return S(Str::Blank);
    return c == " " ? S(Str::Space) : c;
}

void SayCaret(Ptr field, bool word) {
    std::string text = game::TextEditText(field);
    int caret = game::TextEditCaret(field);
    std::string c = CharacterAt(text, caret);
    if (!word || c.empty() || c == " ") {
        speech::Say(SpokenCharacter(c));
        return;
    }
    size_t end = text.find_first_of(" \r\n", caret);
    speech::Say(text.substr(caret, end == std::string::npos ? std::string::npos : end - caret));
}

void SetFocus(int group, int index) {
    g.group = group;
    g.index = index;
    const Item* it = Focused();
    g.focusWidget = it ? it->widget : 0;
    g.focusNumber = it ? it->number : 0;
    g.focusLabel = it ? it->label : "";
    g.focusGroupKey = it ? g.groups[group].key : 0;
    if (it && it->role == Role::LotObject) world::Focus(it->number);  // Ctrl+Enter walks there
    // Typing follows the focus (in the Sims 1 mod a biography's letters went on into the name).
    if (it && it->role == Role::TextField && game::TextEditEditable(it->widget)) game::SetKeyboardFocus(it->widget);
    HoverFocused(it);
}

bool Relocate() {
    for (int gi = 0; gi < (int)g.groups.size(); gi++)
        for (int ii = 0; ii < (int)g.groups[gi].items.size(); ii++) {
            const Item& it = g.groups[gi].items[ii];
            // a virtual item (a lot, a thing, a kind) only in its own group: numbers repeat
            // across groups (kind 3, object 3)
            if (it.widget == g.focusWidget && it.number == g.focusNumber &&
                (it.widget || g.groups[gi].key == g.focusGroupKey) &&
                (it.label == g.focusLabel || it.role == Role::TextField || !it.widget)) {
                g.group = gi;
                g.index = ii;
                return true;
            }
        }
    // Same widget with a new label (a value that changed): keep it.
    for (int gi = 0; gi < (int)g.groups.size(); gi++)
        for (int ii = 0; ii < (int)g.groups[gi].items.size(); ii++)
            if (g.groups[gi].items[ii].widget == g.focusWidget && g.groups[gi].items[ii].number == g.focusNumber &&
                (g.focusWidget || g.groups[gi].key == g.focusGroupKey)) {
                SetFocus(gi, ii);
                return true;
            }
    return false;
}

// The focused item went away (a thing the Sim picked up): the item now at its place in the same
// group, so the focus stays where the player was (NAVIGATION.md), not the screen's default.
bool StayInGroup() {
    for (int gi = 0; gi < (int)g.groups.size(); gi++)
        if (g.groups[gi].key == g.focusGroupKey && !g.groups[gi].items.empty()) {
            SetFocus(gi, std::min(std::max(g.index, 0), (int)g.groups[gi].items.size() - 1));
            return true;
        }
    return false;
}

void FocusDefault() {
    // The mod's own groups (the needs, the action queue: Enter there cancels an action) only when
    // nothing else has a control; the interaction menu is the exception.
    for (int pass = 0; pass < 2; pass++)
        for (int gi = 0; gi < (int)g.groups.size(); gi++) {
            Ptr key = g.groups[gi].key;
            bool own = key < 0x10000 && key != kPieGroup;
            if (IsTextOnly(g.groups[gi]) || (pass == 0 && own)) continue;
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
    // entering a group says its name and size, even of one item (NAVIGATION.md)
    std::string prefix = withGroup ? GroupSummary(group) + ". " : "";
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
    // The interaction menu closed: the panels it hid are back, no news (the object browser, where
    // it was opened from, keeps the keys).
    bool hasPie = keys.count(kPieGroup) != 0;
    if (hasPie && !g.hadPie) {
        // the menu opened: where the focus was (a thing of the lot), to come back to
        g.beforePie = {g.focusWidget, g.focusNumber, g.focusLabel, g.focusGroupKey, g.index};
    }
    if (g.hadPie && !hasPie) {
        for (Ptr k : keys) {
            g.knownGroups.insert(k);
            g.announcedGroups.insert(k);
        }
        // back where the menu was opened from (the thing in "things by kind"), not the screen's default
        g.focusWidget = g.beforePie.widget;
        g.focusNumber = g.beforePie.number;
        g.focusLabel = g.beforePie.label;
        g.focusGroupKey = g.beforePie.groupKey;
        g.index = g.beforePie.index;
    }
    g.hadPie = hasPie;
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
    if (!focusKept && !(g.focusGroupKey && StayInGroup())) FocusDefault();
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

// What the catalog shows: its products' names (a tool that changes them has styles to offer).
std::string CatalogContents(const std::vector<Group>& groups) {
    std::string names;
    for (const Group& gr : groups)
        if (gr.proc == "cTSWinProcCatalog")
            for (const Item& it : gr.items)
                if (it.role == Role::Product) names += it.label + "\n";
    return names;
}

// Build mode's tools and what they offer are two groups (NAVIGATION.md): pressing a tool moves to
// its styles, in the group named after it, on the first; tools that change no style stay where they
// are (the day and night switch, undo).
void FinishToolPress(ULONGLONG now) {
    if (g.catalogNamed && game::CurrentModeClass() != "cTSUIModeBuild") {
        g.catalogNamed = false;
        SetCatalogName("");
    }
    if (!g.toolAt || now - g.toolAt < kToolPressMs || g.announcePending) return;
    g.toolAt = 0;
    g.groups = Collect(g.root);
    std::string contents = CatalogContents(g.groups);
    if (contents.empty() || contents == g.toolBefore) return;
    SetCatalogName(g.toolName);
    g.catalogNamed = true;
    for (int gi = 0; gi < (int)g.groups.size(); gi++) {
        const Group& gr = g.groups[gi];
        if (gr.proc != "cTSWinProcCatalog") continue;
        int at = FirstControl(gr);
        for (int i = (int)gr.items.size() - 1; i >= 0; i--)
            if (gr.items[i].role == Role::Product) at = i;
        SetFocus(gi, at);
        AnnounceFocus(true);
        return;
    }
}

// Ctrl+F's state (the code further down); where things were, for Escape.
struct FindState {
    bool active = false;
    std::string query;       // UTF-8
    bool listed = false;     // the matches are the open list of the kinds group
    char lead = 0;           // the first byte of a double-byte character
    Ptr beforeWidget = 0, beforeGroup = 0, beforeLast = 0;
    int beforeNumber = 0, beforeIndex = -1, beforeKind = 0;
    std::string beforeLabel;
};
FindState g_find;

void Reset() {
    g_find = FindState{};
    g = State();
    g_hovered = 0;
    g_caret = PendingCaret{};
    map::Reset();
    cues::Reset();
}

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
            Log("  group %p %s, %d items %s", (void*)group.key, GroupName(group).c_str(), (int)group.items.size(),
                group.proc.c_str());
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
    FinishToolPress(now);
    // A dialog or menu came up, or the lot went away: the typing is the game's again.
    if (g_find.active && (InputRoot() != game::MainWindow() || !world::LotViewActive())) g_find.active = false;
    world::Tick();
    live::Tick();
    placement::Tick();
    build::Tick();
    map::Tick();
    console::Tick();
    if (g_caret.field && ++g_caret.ticks >= 2) {  // the game handles the key after the mod's hook
        Ptr field = g_caret.field;
        bool word = g_caret.word;
        g_caret = PendingCaret{};
        if (game::FocusWindow() == field) SayCaret(field, word);
    }
    if (ShowFocus() && !g_hovered) HoverFocused(Focused());
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
        // What the game's hover popup says about it (a skill, an interest, a job field): Enter
        // speaks it (in the Sims 1 mod a tester found such items "do nothing").
        if (!it->detail.empty()) speech::Say(it->detail);
        else AnnounceFocus(false);
        return;
    }
    if (it->role == Role::ModManual) {
        if (it->number == 1) input::SayKeyHelp();
        else manual::Open();
        return;
    }
    if (it->role == Role::QueuedAction) {
        live::CancelQueued(*it);
        return;
    }
    if (it->role == Role::TextField) {
        // The window manager's focus gives a text field the keyboard without moving its caret (a
        // click would put it where it lands); a click when that is refused.
        if (game::SetKeyboardFocus(it->widget)) {
            AnnounceFocus(false);
            return;
        }
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
    if (it->role == Role::Level) {
        // a kind or a room of the lot's things: its things, from the first
        Ptr key = g.groups[g.group].key;
        world::Open(key, it->number);
        g.groups = Collect(g.root);
        for (int gi = 0; gi < (int)g.groups.size(); gi++)
            if (g.groups[gi].key == key) SetFocus(gi, 0);
        AnnounceFocus(true);
        return;
    }
    if (it->role == Role::LotObject) {
        world::Focus(it->number);
        world::Interact();
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
    if (g.groups[g.group].proc == "cTSWinProcBuildPanel" && it->role != Role::Product) {
        g.toolAt = GetTickCount64();
        g.toolName = it->label;
        g.toolBefore = CatalogContents(g.groups);
    }
    game::Press(it->widget);
    if (it->role == Role::Product && it->buyObject) placement::Begin(it->label);
    else if (it->role == Role::Product) build::Begin(it->label);
}

bool Back() {
    Refresh(GetTickCount64());
    if (g.group < 0) return false;
    Ptr key = g.groups[g.group].key;
    int was = world::Close(key);
    if (!was) return false;
    // back to the kinds or rooms, on the one that was open
    g.groups = Collect(g.root);
    for (int gi = 0; gi < (int)g.groups.size(); gi++) {
        if (g.groups[gi].key != key) continue;
        int at = 0;
        for (int ii = 0; ii < (int)g.groups[gi].items.size(); ii++)
            if (g.groups[gi].items[ii].number == was) at = ii;
        SetFocus(gi, at);
    }
    AnnounceFocus(true);
    return true;
}

// ---- Find (Ctrl+F) ---------------------------------------------------------------------------

namespace {

// Back to the list and the focus from before the search.
void FindRestore() {
    world::Open(kKindsGroup, g_find.beforeKind);
    world::Open(g_find.beforeLast, world::OpenNumber(g_find.beforeLast));  // PageDown's list as it was
    g_find.listed = false;
    g.focusWidget = g_find.beforeWidget;
    g.focusNumber = g_find.beforeNumber;
    g.focusLabel = g_find.beforeLabel;
    g.focusGroupKey = g_find.beforeGroup;
    g.index = g_find.beforeIndex;
    g.groups = Collect(g.root);
    if ((g.focusWidget || g.focusNumber) && Relocate()) return;
    if (!(g.focusGroupKey && StayInGroup())) FocusDefault();
}

// The matches for the query, focus on the best: "tel: 2 encontrados. Teléfono ..."
void FindShow() {
    int count = world::SetFindQuery(g_find.query);
    if (!count) {
        speech::Say(util::Format(S(Str::FindNoMatch), g_find.query.c_str()));
        return;
    }
    world::Open(kKindsGroup, world::kFindKind);
    g_find.listed = true;
    g.groups = Collect(g.root);
    for (int gi = 0; gi < (int)g.groups.size(); gi++) {
        if (g.groups[gi].key != kKindsGroup || g.groups[gi].items.empty()) continue;
        SetFocus(gi, 0);
        buffers::SetDetails(DetailLines(*Focused(), g.groups[gi], 0));
        std::string found = count == 1 ? util::Format(S(Str::FindCountOne), g_find.query.c_str())
                                       : util::Format(S(Str::FindCount), g_find.query.c_str(), count);
        speech::Say(found + ". " + Describe(*Focused()));
        map::FollowFocus();  // the map, when it is on, goes to the thing
        return;
    }
}

}  // namespace

bool FindActive() { return g_find.active; }

void FindBegin() {
    if (!world::LotViewActive()) {
        speech::Say(S(Str::NotOnLot));
        return;
    }
    if (placement::Active() || build::Active()) {
        speech::Say(S(Str::FindNotNow));
        return;
    }
    if (g_find.active) {  // the keys again
        speech::Say(S(Str::FindPrompt));
        return;
    }
    Refresh(GetTickCount64());
    g_find = FindState{};
    g_find.active = true;
    g_find.beforeWidget = g.focusWidget;
    g_find.beforeNumber = g.focusNumber;
    g_find.beforeLabel = g.focusLabel;
    g_find.beforeGroup = g.focusGroupKey;
    g_find.beforeIndex = g.index;
    g_find.beforeKind = world::OpenNumber(kKindsGroup);
    g_find.beforeLast = world::LastGroup();
    speech::Say(S(Str::FindPrompt));
}

void FindChar(WPARAM ch) {
    if (ch < 0x20 || ch >= 0x100 || ch == 0x7f) {
        g_find.lead = 0;
        return;
    }
    // The window is ANSI: the character comes in the system code page.
    char bytes[2] = {(char)ch, 0};
    int count = 1;
    if (g_find.lead) {
        bytes[0] = g_find.lead;
        bytes[1] = (char)ch;
        count = 2;
        g_find.lead = 0;
    } else if (IsDBCSLeadByte((BYTE)ch)) {
        g_find.lead = (char)ch;
        return;
    }
    wchar_t wide[4] = {};
    char utf8[16] = {};
    int n = MultiByteToWideChar(CP_ACP, 0, bytes, count, wide, 4);
    if (n <= 0 || WideCharToMultiByte(CP_UTF8, 0, wide, n, utf8, sizeof utf8 - 1, nullptr, nullptr) <= 0) return;
    if (utf8[0] == ' ' && g_find.query.empty()) return;
    g_find.query += utf8;
    FindShow();
}

void FindErase() {
    std::string& q = g_find.query;
    if (q.empty()) {
        speech::Say(S(Str::FindEmpty));
        return;
    }
    size_t at = q.size() - 1;
    while (at > 0 && ((unsigned char)q[at] & 0xc0) == 0x80) at--;  // a whole UTF-8 character
    q.resize(at);
    if (!q.empty()) {
        FindShow();
        return;
    }
    if (g_find.listed) FindRestore();
    speech::Say(S(Str::FindEmpty));
}

void FindMove(int delta) {
    if (!g_find.listed) {
        speech::Say(S(Str::FindTypeFirst));
        return;
    }
    Move(delta);
    map::FollowFocus();
}

void FindEdge(bool last) {
    if (!g_find.listed) {
        speech::Say(S(Str::FindTypeFirst));
        return;
    }
    MoveToEdge(last);
    map::FollowFocus();
}

void FindConfirm(bool walk) {
    if (!g_find.listed) {
        speech::Say(S(Str::FindTypeFirst));
        return;
    }
    g_find.active = false;
    if (walk) world::WalkTo(false);
    else Activate();
}

void FindCancel() {
    g_find.active = false;
    if (g_find.listed) FindRestore();
    speech::Say(S(Str::FindCancelled));
}

void FindEnd() { g_find.active = false; }

bool FocusOnThing() {
    const Item* it = Focused();
    return it && it->role == Role::LotObject;
}

// PageUp/PageDown (nav/world.h): the lot's things from anywhere in the lot view. A kind or room
// is opened in its group and focus goes there, so Tab, the arrows, Enter and Backspace go on from
// it as usual.
void BrowseThings(Browse how, int delta) {
    Refresh(GetTickCount64());
    Ptr key = how == Browse::Kind ? kKindsGroup : how == Browse::Room ? kRoomsGroup : world::LastGroup();
    const Item* it = Focused();
    bool inGroup = it && g.groups[g.group].key == key;
    if (how == Browse::Thing && inGroup && it->role == Role::LotObject) {
        Move(delta);
        return;
    }
    // A thing: from the kind or room focused in its list, else the one open, else the first.
    int number = how != Browse::Thing ? world::Step(key, delta)
                 : inGroup && it->role == Role::Level ? it->number
                 : world::OpenNumber(key) ? world::OpenNumber(key)
                                          : world::Step(key, +1);
    // Twice: the kind or room left open may have emptied since.
    for (int attempt = 0; attempt < 2 && number; attempt++) {
        bool same = number == world::OpenNumber(key);
        world::Open(key, number);
        g.groups = Collect(g.root);
        for (int gi = 0; gi < (int)g.groups.size(); gi++) {
            const Group& gr = g.groups[gi];
            if (gr.key != key || gr.items.empty() || gr.items.front().role != Role::LotObject) continue;
            int at = 0;  // back in the same kind or room: on the thing focused last
            for (int ii = 0; same && ii < (int)gr.items.size(); ii++)
                if (gr.items[ii].number == world::FocusedId()) at = ii;
            SetFocus(gi, at);
            AnnounceFocus(true);
            return;
        }
        world::Close(key);
        number = world::Step(key, delta);
    }
    speech::Say(S(Str::NoObjects));
}

// The first letter of a group's name, lowercase, without its accent ("Catálogo" c, "Čas" c); a
// Cyrillic letter gives the key it sits on in the Russian layout ("Объекты" j); 0 for a name that
// starts with anything else (a CJK name).
char FirstLetter(const std::string& utf8) {
    if (utf8.empty()) return 0;
    unsigned char c = (unsigned char)utf8[0];
    if (c < 0x80) return isalpha(c) ? (char)tolower(c) : 0;
    if ((c & 0xe0) != 0xc0 || utf8.size() < 2) return 0;
    int cp = ((c & 0x1f) << 6) | ((unsigned char)utf8[1] & 0x3f);
    static const char kLatin1[] = "aaaaaaaceeeeiiiidnooooo0ouuuuyts";  // U+00C0..U+00DF; +0x20 the lowercase
    static const char kLatinA[] =                                       // U+0100..U+017F
        "aaaaaaccccccccdd" "ddeeeeeeeeeegggg" "gggghhhhiiiiiiii" "iiiijjkkklllllll"
        "lllnnnnnnnnnoooo" "oooorrrrrrssssss" "sstttttuuuuuuuuu" "uuuuwwyyyzzzzzzs";
    static const char kCyrillic[] = "f0dult0pbqrkvyjghcnea0wxio0sm00z";  // А..Я: the ЙЦУКЕН keys
    char f = 0;
    if (cp >= 0xc0 && cp <= 0xff) f = kLatin1[(cp & 0xdf) - 0xc0];
    else if (cp >= 0x100 && cp <= 0x17f) f = kLatinA[cp - 0x100];
    else if (cp >= 0x410 && cp <= 0x44f) f = kCyrillic[(cp - 0x410) & 0x1f];
    return f == '0' ? 0 : f;
}

void JumpToGroup(char letter) {
    Refresh(GetTickCount64());
    int n = (int)g.groups.size();
    for (int step = 1; step <= n; step++) {
        int gi = ((g.group < 0 ? -1 : g.group) + step + n) % n;
        const Group& gr = g.groups[gi];
        // the lot's things go by their own name too while a kind or room is open
        bool match = FirstLetter(GroupName(gr)) == letter ||
                     (gr.key == kKindsGroup && FirstLetter(S(Str::ThingsByKind)) == letter) ||
                     (gr.key == kRoomsGroup && FirstLetter(S(Str::ThingsByRoom)) == letter);
        if (!match) continue;
        SetFocus(gi, FirstControl(gr));
        AnnounceFocus(true);
        return;
    }
    for (const Group& gr : g.groups) LogVerbose("group letter %c: not '%s'", letter, GroupName(gr).c_str());
    speech::Say(util::Format(S(Str::NoGroupLetter), std::string(1, (char)toupper(letter)).c_str()));
}

void ToggleShowFocus() {
    g_showFocus = ShowFocus() ? 0 : 1;
    util::SetSetting("show_focus", g_showFocus);
    HoverFocused(Focused());
    speech::Say(S(g_showFocus ? Str::ShowFocusOn : Str::ShowFocusOff));
}

void NoteCaretKey(bool word) {
    Ptr field = game::FocusWindow();
    if (field && game::TextEditEditable(field)) g_caret = PendingCaret{field, word, 0};
}

void SayCharacterAtCaret() {
    Ptr field = game::FocusWindow();
    if (!field || !game::TextEditEditable(field)) return;
    speech::Say(SpokenCharacter(CharacterAt(game::TextEditText(field), game::TextEditCaret(field))));
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
