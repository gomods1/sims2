#include "live.h"

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
#include "buffers.h"
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

std::string SimName(int simId) {
    game::LotObject o;
    if (!game::ReadLotObject(simId, &o)) return "";
    size_t dash = o.name.rfind(" - ");
    return dash == std::string::npos ? o.name : o.name.substr(dash + 3);
}

void WatchFamily() {
    int sim = game::SelectedSimId();
    if (sim != g_watchedSim) {  // another Sim selected: start over quietly
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
    size_t dash = o.name.rfind(" - ");
    std::string line = dash == std::string::npos ? o.name : o.name.substr(dash + 3);
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

void Tick() {
    ULONGLONG now = GetTickCount64();
    if (now - g_lastCheck < 300) return;
    g_lastCheck = now;
    if (!world::LotViewActive()) {
        g_lastSpeed = -1;
        g_watchedSim = 0;
        return;
    }
    WatchFamily();
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
