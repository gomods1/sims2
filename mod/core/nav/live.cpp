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
        return;
    }
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
        if (funds) line += ", " + util::CleanForSpeech(game::Caption(funds));
        buffers::Event(line);
    } else if (chosen) {
        buffers::Event(util::WithoutHotkey(util::CleanForSpeech(game::Tooltip(chosen))));
    }
}

}  // namespace nav::live
