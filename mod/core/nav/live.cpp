#include "live.h"

#include <cmath>

#include <windows.h>

#include <algorithm>

#include "../game/game_strings.h"
#include "../game/gz.h"
#include "../game/objects.h"
#include "../game/rtti.h"
#include "../speech/speech.h"
#include "../util/log.h"
#include "../util/strings.h"
#include "../util/text.h"
#include "../input/keyboard.h"
#include "../game/buy.h"
#include "buffers.h"
#include "navigator.h"
#include "world.h"

namespace nav::live {
namespace {

// The Sim's motives (game/objects.h SimMotive) with the index of their name in Live.package
// STR# 130, the needs panel's order.
struct Need {
    int motive;
    int nameIndex;
};
const Need kNeeds[] = {{7, 1}, {5, 3}, {6, 5}, {15, 7}, {8, 9}, {14, 11}, {9, 13}, {13, 15}};
constexpr unsigned kNeedNames = 130;
constexpr int kLowPercent = 35;  // a need at or below this is "low" in the status line

std::string NeedName(const Need& n) { return game::GameString("Live.package", kNeedNames, n.nameIndex); }

struct NeedValue {
    std::string name;
    int percent;
};

std::vector<NeedValue> Needs(int simId) {
    std::vector<NeedValue> out;
    for (const Need& n : kNeeds) {
        float v = 0;
        if (!game::SimMotive(simId, n.motive, &v)) continue;
        int percent = (int)((v + 100.0f) / 2.0f + 0.5f);
        out.push_back({NeedName(n), std::clamp(percent, 0, 100)});
    }
    return out;
}

// Windows of one class among the main window's children (the queue icons live there).
std::vector<Ptr> TopLevel(const char* cls) {
    std::vector<Ptr> out;
    for (Ptr w : game::Children(game::MainWindow()))
        if (game::Visible(w) && game::IsKindOf(w, cls)) out.push_back(w);
    return out;
}

// The speed buttons of the live panel, by window id (their tooltips name them).
struct SpeedButton {
    uint32_t id;
    int speed;  // 0 paused, 1..3
};
const SpeedButton kSpeeds[] = {{0x27d7258f, 0}, {0x57e01cc1, 1}, {0x37e01cc3, 2}, {0x47e01cc5, 3}};
constexpr uint32_t kClockId = 0x37d82a93;
constexpr uint32_t kFundsId = 0x3000;

Ptr FindById(Ptr w, uint32_t id, int depth) {
    if (!w || depth > 25 || !game::Visible(w)) return 0;
    if (game::WinId(w) == id) return w;
    for (Ptr c : game::Children(w))
        if (Ptr f = FindById(c, id, depth + 1)) return f;
    return 0;
}

int g_lastSpeed = -1;
ULONGLONG g_lastCheck = 0;

// The family watch: the selected Sim's current action and needs as they change.
int g_watchedSim = 0;
std::string g_lastAction;          // the queue's first entry at the last check
std::vector<int> g_needLevels;     // 0 fine, 1 low, 2 urgent, per kNeeds entry
constexpr int kUrgentPercent = 15;

// A Sim's first name (what the narrator says), else its internal name's end.
std::string SimName(int simId) {
    std::string first, last;
    if (game::SimName(simId, &first, &last) && !first.empty()) return first;
    game::LotObject o;
    if (!game::ReadLotObject(simId, &o)) return "";
    size_t dash = o.name.rfind(" - ");
    return dash == std::string::npos ? o.name : o.name.substr(dash + 3);
}

// Visitors: Sims on the lot from outside the family, announced once they have stayed a second
// (some are created and removed at once).
struct Visitor {
    int id;
    std::string name;
    ULONGLONG since;
    bool announced;
};
std::vector<Visitor> g_visitors;
bool g_visitorsPrimed = false;  // the Sims already there when the lot loaded are not "arriving"

void WatchVisitors() {
    std::vector<int> family = game::FamilySimIds();
    std::vector<int> present;
    for (const game::LotObject& o : game::LotObjects())
        if (o.person && o.x >= 0 && std::find(family.begin(), family.end(), o.id) == family.end())
            present.push_back(o.id);
    ULONGLONG now = GetTickCount64();
    for (size_t i = g_visitors.size(); i-- > 0;) {
        if (std::find(present.begin(), present.end(), g_visitors[i].id) != present.end()) continue;
        if (g_visitors[i].announced) buffers::Event(util::Format(S(Str::VisitorLeft), g_visitors[i].name.c_str()));
        g_visitors.erase(g_visitors.begin() + i);
    }
    for (int id : present) {
        auto it = std::find_if(g_visitors.begin(), g_visitors.end(), [&](const Visitor& v) { return v.id == id; });
        if (it == g_visitors.end()) {
            std::string first, last;
            std::string name = game::SimName(id, &first, &last) ? (last.empty() ? first : first + " " + last) : "";
            if (name.empty()) continue;
            g_visitors.push_back({id, name, now, !g_visitorsPrimed});
        } else if (!it->announced && now - it->since > 1000) {
            it->announced = true;
            buffers::Event(util::Format(S(Str::VisitorArrived), it->name.c_str()));
        }
    }
    g_visitorsPrimed = true;
}

void WatchFamily() {
    int sim = game::SelectedSimId();
    if (sim != g_watchedSim) {  // another Sim selected: say who, start over quietly
        if (g_watchedSim && sim) {
            std::string first, last;
            std::string who = game::SimName(sim, &first, &last) ? first : SimName(sim);
            buffers::Event(util::Format(S(Str::SimSelectedNow), who.c_str()));
        }
        g_watchedSim = sim;
        g_lastAction.clear();
        g_needLevels.clear();
    }
    if (!sim) return;
    std::string name = SimName(sim);
    Group q = QueueGroup();
    std::string action = q.items.empty() ? "" : q.items[0].label;
    if (action != g_lastAction) {
        if (!g_lastAction.empty()) buffers::Record(util::Format(S(Str::ActionFinished), name.c_str(), g_lastAction.c_str()));
        if (!action.empty()) buffers::Event(util::Format(S(Str::ActionStarted), name.c_str(), action.c_str()));
        g_lastAction = action;
    }
    std::vector<NeedValue> needs = Needs(sim);
    bool first = g_needLevels.size() != needs.size();
    if (first) g_needLevels.assign(needs.size(), 0);
    std::string low, urgent;
    for (size_t i = 0; i < needs.size(); i++) {
        int level = needs[i].percent <= kUrgentPercent ? 2 : needs[i].percent <= kLowPercent ? 1 : 0;
        // Hysteresis: a need leaves "low" only once it is clearly back up.
        if (level == 0 && g_needLevels[i] > 0 && needs[i].percent < kLowPercent + 10) level = g_needLevels[i];
        if (level > g_needLevels[i] && !first) (level == 2 ? urgent : low) += (level == 2 ? (urgent.empty() ? "" : ", ") : (low.empty() ? "" : ", ")) + needs[i].name;
        g_needLevels[i] = level;
    }
    if (!urgent.empty()) buffers::Event(util::Format(S(Str::NeedUrgent), name.c_str(), urgent.c_str()));
    if (!low.empty()) buffers::Event(util::Format(S(Str::NeedLow), name.c_str(), low.c_str()));
}

}  // namespace

Group NeedsGroup() {
    Group g;
    g.key = kNeedsGroup;
    g.name = S(Str::GroupNeeds);
    int sim = game::SelectedSimId();
    if (!sim) return g;
    std::vector<NeedValue> needs = Needs(sim);
    int order = 0;
    for (const NeedValue& n : needs) {
        Item it;
        it.role = Role::Text;
        it.number = ++order;  // identifies the row across refreshes
        it.label = util::Format(S(Str::MotiveValue), n.name.c_str(), n.percent);
        g.items.push_back(it);
    }
    return g;
}

Group QueueGroup() {
    Group g;
    g.key = kQueueGroup;
    g.name = S(Str::GroupActionQueue);
    std::vector<Ptr> icons = TopLevel("cTSWinActionQueueIcon");
    std::sort(icons.begin(), icons.end(), [](Ptr a, Ptr b) { return game::Area(a).left < game::Area(b).left; });
    for (Ptr w : icons) {
        Item it;
        it.widget = w;
        it.role = Role::QueuedAction;
        it.label = util::CleanForSpeech(game::Caption(w));
        it.rect = game::ScreenRect(w);
        if (!it.label.empty()) g.items.push_back(it);
    }
    return g;
}

void CancelQueued(const Item& it) {
    Log("cancel queued action \"%s\"", it.label.c_str());
    game::ClickWindow(it.widget);
    speech::Say(it.label + ", " + S(Str::Cancelled));
}

void SayStatus() {
    int sim = game::SelectedSimId();
    game::LotObject o;
    if (!sim || !game::ReadLotObject(sim, &o)) {
        speech::Say(S(Str::NoSimSelected));
        return;
    }
    std::string line = SimName(sim);
    Group q = QueueGroup();
    line += ", " + (q.items.empty() ? std::string(S(Str::StatusIdle)) : q.items[0].label);
    std::vector<NeedValue> needs = Needs(sim);
    std::sort(needs.begin(), needs.end(), [](const NeedValue& a, const NeedValue& b) { return a.percent < b.percent; });
    std::string low;
    for (const NeedValue& n : needs)
        if (n.percent <= kLowPercent) low += (low.empty() ? "" : ", ") + util::Format(S(Str::MotiveValue), n.name.c_str(), n.percent);
    line += "; " + (low.empty() ? std::string(S(Str::StatusNeedsFine)) : util::Format(S(Str::StatusNeedsLow), low.c_str()));
    std::vector<std::string> details = {line};
    for (const NeedValue& n : needs) details.push_back(util::Format(S(Str::MotiveValue), n.name.c_str(), n.percent));
    buffers::SetDetails(details);
    speech::Say(line);
}

std::string RatingName(int rating) {
    // hunger, comfort, hygiene, bladder, energy, fun, room -> Live.package STR# 130
    static const int kNameIndex[] = {1, 5, 9, 13, 3, 7, 15};
    if (rating < 0 || rating >= 7) return "";
    return game::GameString("Live.package", kNeedNames, kNameIndex[rating]);
}

std::string FundsText() {
    Ptr funds = FindById(game::MainWindow(), kFundsId, 0);
    return funds ? util::CleanForSpeech(game::Caption(funds)) : "";
}

// ---- Camera following the selected Sim --------------------------------------------------------
bool g_follow = true;
int g_followSim = 0;
float g_followX = -1000, g_followY = -1000;
ULONGLONG g_followAt = 0;

void FollowTick() {
    if (!g_follow || game::CurrentModeClass() != "cTSUIModeLive") return;
    if (game::ModalWindow() || nav::TextEntryActive()) return;  // Return would press a dialog's button
    int sim = game::SelectedSimId();
    game::LotObject o;
    if (!sim || !game::ReadLotObject(sim, &o) || o.x < 0) return;
    ULONGLONG now = GetTickCount64();
    bool moved = std::hypot(o.x - g_followX, o.y - g_followY) >= 4.0;
    if (sim == g_followSim && (!moved || now - g_followAt < 3000)) return;
    g_followSim = sim;
    g_followX = o.x;
    g_followY = o.y;
    g_followAt = now;
    input::PostGameKey(VK_RETURN);  // "Center on active Sim"
}

void ToggleFollow() {
    g_follow = !g_follow;
    g_followSim = 0;
    if (!g_follow) {
        speech::Say(S(Str::CameraFree));
        return;
    }
    std::string name = SimName(game::SelectedSimId());
    speech::Say(util::Format(S(Str::CameraFollowing), name.c_str()));
}

void SelectMember(int index) {
    // The family's portraits: buttons of the Sim chooser, top to bottom.
    std::vector<Ptr> buttons;
    std::vector<Ptr> stack = {game::MainWindow()};
    int visited = 0;
    while (!stack.empty() && visited < 600) {
        Ptr w = stack.back();
        stack.pop_back();
        visited++;
        if (!game::Visible(w)) continue;
        if (game::ProcClass(w) == "cTSWinProcSimChooser") {
            std::vector<Ptr> inner = {w};
            while (!inner.empty()) {
                Ptr x = inner.back();
                inner.pop_back();
                for (Ptr c : game::Children(x)) {
                    if (!game::Visible(c)) continue;
                    if (game::IsButton(c) && !game::Tooltip(c).empty()) buttons.push_back(c);
                    else inner.push_back(c);
                }
            }
            break;
        }
        for (Ptr c : game::Children(w)) stack.push_back(c);
    }
    std::sort(buttons.begin(), buttons.end(), [](Ptr a, Ptr b) {
        RECT ra = game::ScreenRect(a), rb = game::ScreenRect(b);
        return ra.top != rb.top ? ra.top < rb.top : ra.left < rb.left;
    });
    if (index < 0 || index >= (int)buttons.size()) {
        speech::Say(util::Format(S(Str::NoSuchSim), index + 1));
        return;
    }
    Log("select family member %d: %s", index + 1, game::Tooltip(buttons[index]).c_str());
    game::Press(buttons[index]);
}

void Tick() {
    ULONGLONG now = GetTickCount64();
    if (now - g_lastCheck < 300) return;
    g_lastCheck = now;
    if (!world::LotViewActive()) {
        g_lastSpeed = -1;
        g_watchedSim = 0;
        g_visitors.clear();
        g_visitorsPrimed = false;
        return;
    }
    WatchFamily();
    WatchVisitors();
    FollowTick();
    // 0 paused, else the chosen speed (the speed buttons show the choice, not the pause).
    Ptr main = game::MainWindow();
    int speed = -1;
    Ptr chosen = 0;
    for (const SpeedButton& b : kSpeeds) {
        Ptr w = FindById(main, b.id, 0);
        if (w && b.speed > 0 && game::ButtonOn(w)) {
            speed = b.speed;
            chosen = w;
        }
    }
    bool paused = game::GamePaused();
    if (paused) speed = 0;
    if (speed < 0 || speed == g_lastSpeed) return;
    bool first = g_lastSpeed < 0;
    g_lastSpeed = speed;
    if (first) return;  // entering the lot: nothing changed
    if (paused) {
        Ptr pause = FindById(main, kSpeeds[0].id, 0);
        std::string line = pause ? util::WithoutHotkey(util::CleanForSpeech(game::Tooltip(pause))) : "";
        Ptr clock = FindById(main, kClockId, 0), funds = FindById(main, kFundsId, 0);
        if (clock) util::AppendSentence(line, util::CleanForSpeech(game::Caption(clock)));
        std::string money = funds ? util::CleanForSpeech(game::Caption(funds)) : "";
        if (!money.empty()) line += ", " + money;
        buffers::Event(line);
    } else if (chosen) {
        buffers::Event(util::WithoutHotkey(util::CleanForSpeech(game::Tooltip(chosen))));
    }
}

}  // namespace nav::live
