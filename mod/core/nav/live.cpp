#include "live.h"

#include <cmath>

#include <windows.h>

#include <algorithm>
#include <map>

#include "../game/game_strings.h"
#include "../game/addresses.h"
#include "../game/gz.h"
#include "../game/objects.h"
#include "../game/rtti.h"
#include "../speech/speech.h"
#include "../util/log.h"
#include "../util/settings.h"
#include "../util/strings.h"
#include "../util/text.h"
#include "../input/keyboard.h"
#include "../game/buy.h"
#include "balloons.h"
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

// A funds caption is an amount and nothing else: digits, their separators, the simoleon sign (the
// game draws U+00A7 as it), spaces (also no-break ones).
bool IsAmount(const std::string& s) {
    bool digit = false;
    for (size_t i = 0; i < s.size(); ++i) {
        unsigned char c = (unsigned char)s[i];
        if (c >= '0' && c <= '9') digit = true;
        else if (c == ' ' || c == '.' || c == ',' || c == '\'' || c == '-' || c == '$') {}
        else if (c == 0xc2 && i + 1 < s.size() && ((unsigned char)s[i + 1] == 0xa7 || (unsigned char)s[i + 1] == 0xa0)) ++i;
        else if (c == 0xe2 && i + 2 < s.size() && (unsigned char)s[i + 1] == 0x80 && (unsigned char)s[i + 2] == 0xaf) i += 2;
        else return false;
    }
    return digit;
}

// The control panel's funds: id 0x3000 is not unique (the phone book's selected thumbnail has it
// too, and so do the vending machine's products: 'Teléfono móvil "NOYIN 2680" 149' was read as
// funds of 2.680.149), the funds are the one whose caption is only an amount.
Ptr FindFunds(Ptr w, int depth) {
    if (!w || depth > 25 || !game::Visible(w)) return 0;
    if (game::WinId(w) == kFundsId && IsAmount(game::Caption(w))) return w;
    for (Ptr c : game::Children(w))
        if (Ptr f = FindFunds(c, depth + 1)) return f;
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
    bool atDoorSaid = false;
};
ULONGLONG g_doorCheckAt = 0;

// A visitor not greeted yet (person data 34 = 0) standing by a door: knocking or ringing, waiting
// for someone of the family to greet them before coming in (a tester did not know who rang).
bool WaitingAtDoor(int id) {
    int greet = -1, npc = 0;
    if (!game::PersonDatum(id, game::addr::person::kDataGreetStatus, &greet) || greet != 0) return false;
    if (game::PersonDatum(id, game::addr::person::kDataNpcType, &npc) && npc != 0) return false;  // a service Sim
    game::LotObject o;
    if (!game::ReadLotObject(id, &o) || o.x < 0) return false;
    for (const game::WallObject& w : game::WallObjects())
        if (w.type == 8 && std::max(o.level, 1) == std::max(w.level, 1) && std::hypot(w.x - o.x, w.y - o.y) <= 2.0f)
            return true;
    return false;
}
std::vector<Visitor> g_visitors;
bool g_visitorsPrimed = false;  // the Sims already there when the lot loaded are not "arriving"

// The narrator's verbosity (Ctrl+N, kept as [mod] narrator=2/1/0): everything, warnings only
// (urgent needs, visitors), or nothing spoken; every line still goes to the events buffer.
int Verbosity() { return util::GetSetting("narrator", 2); }

// Whose conversations are spoken (Ctrl+Shift+N): testers of the Sims 1 mod wanted to hear what
// visitors answer their Sims and how other Sims get on.
int g_social = -1;  // read from the settings on first use
int CurrentSocial() {
    if (g_social < 0) {
        g_social = util::GetSetting("social", kSocialWithFamily);
        if (g_social < kSocialNobody || g_social > kSocialEveryone) g_social = kSocialWithFamily;
    }
    return g_social;
}

// Every new hour of the game's clock, spoken when the player asked for it (Ctrl+T; a tester of the
// Sims 1 mod went past the hour he wanted at the fastest speed): "son las 14:00".
int g_hours = -1;        // -1: not read from the settings yet
std::string g_hourSeen;  // the clock's hour part ("14", "2 PM") last read

bool HoursOn() {
    if (g_hours < 0) g_hours = util::GetSetting("hours", 0) ? 1 : 0;
    return g_hours == 1;
}

// The clock's text without its minutes: "11:46" -> "11", "2:05 PM" -> "2 PM".
std::string HourPart(const std::string& clock) {
    size_t colon = clock.find(':');
    if (colon == std::string::npos) return clock;
    size_t end = colon + 1;
    while (end < clock.size() && clock[end] >= '0' && clock[end] <= '9') end++;
    return clock.substr(0, colon) + clock.substr(end);
}

}  // namespace

void Narrate(const std::string& line, bool warning) {
    int v = Verbosity();
    if (v >= 2 || (v == 1 && warning)) buffers::Event(line);
    else buffers::Record(line);
}

namespace {

void WatchVisitors() {
    std::vector<int> family = game::FamilySimIds();
    std::vector<int> present;
    for (const game::LotObject& o : game::LotObjects())
        if (o.person && o.x >= 0 && std::find(family.begin(), family.end(), o.id) == family.end())
            present.push_back(o.id);
    ULONGLONG now = GetTickCount64();
    for (size_t i = g_visitors.size(); i-- > 0;) {
        if (std::find(present.begin(), present.end(), g_visitors[i].id) != present.end()) continue;
        if (g_visitors[i].announced) Narrate(util::Format(S(Str::VisitorLeft), g_visitors[i].name.c_str()), true);
        g_visitors.erase(g_visitors.begin() + i);
    }
    for (int id : present) {
        auto it = std::find_if(g_visitors.begin(), g_visitors.end(), [&](const Visitor& v) { return v.id == id; });
        if (it == g_visitors.end()) {
            std::string first, last;
            std::string name = game::SimName(id, &first, &last) ? (last.empty() ? first : first + " " + last) : "";
            if (name.empty()) continue;
            std::string role = world::SimRole(id);  // "Ravi Pérez, cartero"
            if (!role.empty()) name += ", " + role;
            g_visitors.push_back({id, name, now, !g_visitorsPrimed});
        } else if (!it->announced && now - it->since > 1000) {
            it->announced = true;
            Narrate(util::Format(S(Str::VisitorArrived), it->name.c_str()), true);
        }
    }
    if (now - g_doorCheckAt > 1000) {
        g_doorCheckAt = now;
        for (Visitor& v : g_visitors) {
            if (v.atDoorSaid || !WaitingAtDoor(v.id)) continue;
            v.atDoorSaid = true;
            Narrate(util::Format(S(Str::VisitorAtDoor), v.name.c_str()), true);
        }
    }
    g_visitorsPrimed = true;
}

// Wants and fears coming true (a tester heard nothing of either, then only "+1000 aspiration
// points" without the want): a family Sim's aspiration reward points (person data 161, x10) going
// up is a want fulfilled, named by the want that left that Sim's list in the game's want simulator
// (game::SimWishes: any family Sim, the wants panel open or not; fulfilling one replaces the whole
// list, so it is the one gone whose points are the points gained); a sharp drop of the aspiration
// score (160, -100..100) with a fear gone from the list is a fear come true.
struct AspirationState {
    int points = -1, score = -1000;
    std::vector<game::Wish> wants, fears;  // as last seen
};
std::map<int, AspirationState> g_aspiration;
ULONGLONG g_aspirationAt = 0;

// What was in before and is not now (a want may be there twice).
std::vector<game::Wish> Gone(std::vector<game::Wish> before, const std::vector<game::Wish>& now) {
    for (const game::Wish& n : now) {
        auto it = std::find_if(before.begin(), before.end(), [&](const game::Wish& b) { return b.name == n.name; });
        if (it != before.end()) before.erase(it);
    }
    return before;
}

// The one that came true among those gone: the only one, else the only one worth the points
// (any, for points 0: fears).
std::string CameTrue(const std::vector<game::Wish>& gone, int points) {
    if (gone.size() == 1) return util::CleanForSpeech(gone[0].name);
    std::string found;
    int matches = 0;
    for (const game::Wish& w : gone)
        if (points == 0 || w.points == points) {
            found = w.name;
            matches++;
        }
    return matches == 1 ? util::CleanForSpeech(found) : "";
}

void WatchAspiration() {
    ULONGLONG now = GetTickCount64();
    if (now - g_aspirationAt < 1000) return;
    g_aspirationAt = now;
    for (int id : game::FamilySimIds()) {
        int points = 0, score = 0;
        if (!game::PersonDatum(id, game::addr::person::kDataRewardPoints, &points) ||
            !game::PersonDatum(id, game::addr::person::kDataAspirationScore, &score))
            continue;
        std::vector<game::Wish> wants, fears;
        bool lists = game::SimWishes(id, false, &wants) && game::SimWishes(id, true, &fears);
        AspirationState& st = g_aspiration[id];
        bool first = st.points < 0;
        int gained = (points - st.points) * 10, dropped = st.score - score;
        std::vector<game::Wish> wantsGone = Gone(st.wants, wants), fearsGone = Gone(st.fears, fears);
        st.points = points;
        st.score = score;
        if (lists) {
            st.wants = wants;
            st.fears = fears;
        }
        if (first || !lists) {
            if (!first && gained > 0)
                Narrate(util::Format(S(Str::AspirationPointsGained), SimName(id).c_str(), gained), true);
            continue;
        }
        std::string who = SimName(id);
        if (gained > 0) {
            std::string want = CameTrue(wantsGone, gained);
            Narrate(want.empty() ? util::Format(S(Str::AspirationPointsGained), who.c_str(), gained)
                                 : util::Format(S(Str::WantFulfilled), who.c_str(), want.c_str(), gained),
                    true);
        } else if ((dropped >= 10 || gained < 0) && !fearsGone.empty()) {
            std::string fear = CameTrue(fearsGone, 0);
            if (!fear.empty()) Narrate(util::Format(S(Str::FearRealized), who.c_str(), fear.c_str()), true);
        }
    }
}

// Furniture that leaves the lot in live mode (a burglar, the repo man, a fire): the lot's catalog
// objects worth something, compared every two seconds. Buy and build mode move and sell objects
// on purpose: the list starts over there.
struct Furniture {
    std::string name;
    int price;
};
std::map<int, Furniture> g_furniture;
bool g_furniturePrimed = false;
ULONGLONG g_furnitureAt = 0;
void WatchFurniture() {
    ULONGLONG now = GetTickCount64();
    if (game::CurrentModeClass() != "cTSUIModeLive") {
        g_furniturePrimed = false;
        return;
    }
    if (now - g_furnitureAt < 2000) return;
    g_furnitureAt = now;
    std::map<int, Furniture> seen;
    for (const game::LotObject& o : game::LotObjects())
        if (!o.person && o.price >= 50 && !o.catalogName.empty()) seen[o.id] = {util::CleanForSpeech(o.catalogName), o.price};
    if (g_furniturePrimed)
        for (const auto& [id, f] : g_furniture)
            if (!seen.count(id))
                Narrate(util::Format(S(Str::FurnitureGone), f.name.c_str(), util::Money(f.price).c_str()), true);
    g_furniture = std::move(seen);
    g_furniturePrimed = true;
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
        // The action that ran is still in the queue behind the new one: something pushed in (in
        // the Sims 1 mod a tester's Sim left what it was doing for garden gnomes, unsaid).
        bool requeued = false;
        for (size_t i = 1; i < q.items.size() && !g_lastAction.empty() && !action.empty(); i++)
            requeued |= q.items[i].label == g_lastAction;
        if (requeued) {
            Log("action change: '%s' -> '%s', the first waits in the queue", g_lastAction.c_str(), action.c_str());
            Narrate(util::Format(S(Str::ActionInterruptedWaits), name.c_str(), g_lastAction.c_str(), action.c_str()), true);
            g_lastAction = action;
            return;
        }
        if (!g_lastAction.empty()) buffers::Record(util::Format(S(Str::ActionFinished), name.c_str(), g_lastAction.c_str()));
        if (!action.empty()) {
            // An action the Sim keeps coming back to (stomping, examining on its own) is said once
            // a minute at most; the events buffer has every start.
            static std::map<std::string, ULONGLONG> s_said;
            ULONGLONG now = GetTickCount64();
            std::string line = util::Format(S(Str::ActionStarted), name.c_str(), action.c_str());
            auto it = s_said.find(line);
            if (it != s_said.end() && now - it->second < 60000) {
                buffers::Record(line);
            } else {
                s_said[line] = now;
                Narrate(line, false);
            }
        }
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
    if (!urgent.empty()) Narrate(util::Format(S(Str::NeedUrgent), name.c_str(), urgent.c_str()), true);
    if (!low.empty()) Narrate(util::Format(S(Str::NeedLow), name.c_str(), low.c_str()), false);
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
    int self = game::SelectedSimId();
    for (Ptr w : icons) {
        Item it;
        it.widget = w;
        it.role = Role::QueuedAction;
        it.label = util::CleanForSpeech(game::Caption(w));
        it.rect = game::ScreenRect(w);
        if (it.label.empty()) continue;
        // The other Sim of an action done with one, whoever chose it ("Saludar, con Elvira").
        int with = game::QueueIconObject(w);
        game::LotObject other;
        if (with && with != self && game::ReadLotObject(with, &other) && other.person)
            it.label = util::Format(S(Str::ActionWithSim), it.label.c_str(), SimName(with).c_str());
        g.items.push_back(it);
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
    Ptr funds = FindFunds(game::MainWindow(), 0);
    return funds ? util::CleanForSpeech(game::Caption(funds)) : "";
}

// ---- The camera's zoom and facing ----------------------------------------------------------------
// The game's keys and buttons change them: a player can't tell otherwise. Zoom 0 is the farthest.
namespace {

const Str kCompass[8] = {Str::DirNorth, Str::DirNorthEast, Str::DirEast, Str::DirSouthEast,
                         Str::DirSouth, Str::DirSouthWest, Str::DirWest, Str::DirNorthWest};

std::string ZoomText(int zoom, int steps) {
    Str word = zoom * 3 < steps ? Str::ZoomFar : zoom * 3 < steps * 2 ? Str::ZoomMedium : Str::ZoomNear;
    return std::string(S(word)) + ", " + util::Format(S(Str::ValueOfMax), zoom + 1, steps);
}

int g_viewZoom = -1, g_viewCompass = -1;

// Said once the camera has settled (half a second unchanged): a turn glides through the compass
// points between (turning a held object turns the camera with it; a tester heard four lines a turn).
int g_viewSaidZoom = -1, g_viewSaidCompass = -1;
ULONGLONG g_viewChangedAt = 0;
void WatchView() {
    int zoom, steps, compass;
    if (!game::CameraView(&zoom, &steps, &compass)) return;
    ULONGLONG now = GetTickCount64();
    bool first = g_viewZoom < 0;
    if (zoom != g_viewZoom || compass != g_viewCompass) g_viewChangedAt = now;
    g_viewZoom = zoom;
    g_viewCompass = compass;
    if (first) {
        g_viewSaidZoom = zoom;
        g_viewSaidCompass = compass;
        return;
    }
    if (now - g_viewChangedAt < 500) return;
    if (zoom != g_viewSaidZoom) buffers::Event(ZoomText(zoom, steps));
    if (compass != g_viewSaidCompass) buffers::Event(util::Format(S(Str::CameraFacing), S(kCompass[compass])));
    g_viewSaidZoom = zoom;
    g_viewSaidCompass = compass;
}

}  // namespace

std::string CameraLine() {
    int zoom, steps, compass;
    if (!game::CameraView(&zoom, &steps, &compass)) return "";
    return util::Format(S(Str::CameraStatus), ZoomText(zoom, steps).c_str(), S(kCompass[compass]));
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

void WatchHour() {
    Ptr clock = FindById(game::MainWindow(), kClockId, 0);
    std::string text = clock ? util::CleanForSpeech(game::Caption(clock)) : "";
    if (text.find(':') == std::string::npos) return;
    // The caption starts with the weekday ("Sab 7:01"): the time is from its first digit.
    size_t digit = text.find_first_of("0123456789");
    if (digit != std::string::npos) text = text.substr(digit);
    std::string hour = HourPart(text);
    bool changed = !g_hourSeen.empty() && hour != g_hourSeen;
    g_hourSeen = hour;
    if (changed && HoursOn()) buffers::Event(util::Format(S(Str::HourChanged), text.c_str()));
}

void CycleSocial() {
    int v = CurrentSocial();
    g_social = v == kSocialWithFamily ? kSocialEveryone
             : v == kSocialEveryone   ? kSocialFamily
             : v == kSocialFamily     ? kSocialNobody
                                      : kSocialWithFamily;
    util::SetSetting("social", g_social);
    speech::Say(S(g_social == kSocialEveryone     ? Str::SocialEveryone
                  : g_social == kSocialWithFamily ? Str::SocialWithFamily
                  : g_social == kSocialFamily     ? Str::SocialFamilyOnly
                                                  : Str::SocialNobody));
}

int SocialScope() { return CurrentSocial(); }

int RunningActionWith() {
    std::vector<Ptr> icons = TopLevel("cTSWinActionQueueIcon");
    if (icons.empty()) return 0;
    Ptr first = *std::min_element(icons.begin(), icons.end(),
                                  [](Ptr a, Ptr b) { return game::Area(a).left < game::Area(b).left; });
    return game::QueueIconObject(first);
}

void ToggleHours() {
    g_hours = HoursOn() ? 0 : 1;
    util::SetSetting("hours", g_hours);
    speech::Say(S(g_hours ? Str::HoursOn : Str::HoursOff));
}

void CycleNarrator() {
    int v = (Verbosity() + 2) % 3;  // 2 -> 1 -> 0 -> 2
    util::SetSetting("narrator", v);
    static const Str kNames[] = {Str::NarratorOff, Str::NarratorWarnings, Str::NarratorAll};
    speech::Say(S(kNames[v]));
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

// Money paid or received in live mode (bills, wages, purchases by phone): "-$40, 8.415 left".
// Buy and build mode say their own costs.
long long g_lastFunds = -1;
void WatchFunds() {
    if (game::CurrentModeClass() != "cTSUIModeLive") {
        g_lastFunds = -1;
        return;
    }
    std::string text = FundsText();
    long long v = 0;
    bool any = false;
    for (char c : text)
        if (c >= '0' && c <= '9') {
            v = v * 10 + (c - '0');
            any = true;
        }
    if (!any) return;
    if (g_lastFunds >= 0 && v != g_lastFunds) {
        long long diff = v - g_lastFunds;
        std::string amount = (diff > 0 ? "+" : "-") + util::Money((int)(diff > 0 ? diff : -diff));
        Narrate(util::Format(S(Str::FundsChanged), amount.c_str(), text.c_str()), true);
    }
    g_lastFunds = v;
}

void Tick() {
    if (world::LotViewActive()) balloons::Tick();
    ULONGLONG now = GetTickCount64();
    if (now - g_lastCheck < 300) return;
    g_lastCheck = now;
    if (!world::LotViewActive()) {
        g_lastSpeed = -1;
        g_watchedSim = 0;
        g_visitors.clear();
        g_visitorsPrimed = false;
        g_aspiration.clear();
        g_lastFunds = -1;  // another lot, another family: its funds are not a payment
        g_furniturePrimed = false;
        g_viewZoom = -1;
        g_hourSeen.clear();
        return;
    }
    WatchView();
    WatchHour();
    WatchFamily();
    WatchVisitors();
    WatchFurniture();
    WatchFunds();
    WatchAspiration();
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
        Ptr clock = FindById(main, kClockId, 0), funds = FindFunds(main, 0);
        if (clock) util::AppendSentence(line, util::CleanForSpeech(game::Caption(clock)));
        std::string money = funds ? util::CleanForSpeech(game::Caption(funds)) : "";
        if (!money.empty()) line += ", " + money;
        buffers::Event(line);
    } else if (chosen) {
        buffers::Event(util::WithoutHotkey(util::CleanForSpeech(game::Tooltip(chosen))));
    }
}

}  // namespace nav::live
