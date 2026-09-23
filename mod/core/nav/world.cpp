#include "world.h"

#include "../game/buy.h"
#include "../game/hooks.h"
#include "../game/mouse.h"
#include "collect.h"
#include "placement.h"

#include <windows.h>

#include <algorithm>
#include <cmath>
#include <vector>

#include "../game/game_strings.h"
#include "../game/gz.h"
#include "../game/objects.h"
#include "../game/rtti.h"
#include "../speech/speech.h"
#include "../util/log.h"
#include "../util/strings.h"
#include "../util/text.h"
#include "buffers.h"

namespace nav::world {
namespace {

using game::LotObject;
using game::Ptr;

// Buy mode's function sort flags (OBJD, game/addresses.h) with the index of their name in
// UIText.package STR# 150, in the order the buy catalog shows them.
struct Category {
    uint16_t flag;   // 0: the Sims; 0xFFFF: objects without a category
    int nameIndex;   // in STR# 150, or -1 for the mod's own name
};
const Category kCategories[] = {
    {0, -1},         // Sims
    {0x0001, 8},     // Comfort
    {0x0002, 9},     // Surfaces
    {0x0020, 10},    // Decorative
    {0x0008, 11},    // Electronics
    {0x0004, 12},    // Appliances
    {0x0010, 13},    // Plumbing
    {0x0080, 14},    // Lighting
    {0x0040, 15},    // Miscellaneous
    {0x0100, 16},    // Hobbies
    {0xFFFF, -1},    // other objects (doors, windows, plants, the mailbox...)
};
constexpr int kCategoryCount = sizeof(kCategories) / sizeof(kCategories[0]);
constexpr unsigned kCategoryNames = 150;

int g_category = 0;
int g_focusId = -1;
bool g_roomMode = false;          // PageUp/PageDown walk a room instead of a category
int g_roomLevel = 0, g_roomId = -1;
ULONGLONG g_menuAskedAt = 0;  // Interact() posted a menu request at this time
ULONGLONG g_walkAskedAt = 0;  // WalkTo() clicked the ground at this time
bool g_walkRun = false;
bool g_walkClickPending = false;  // waiting for Ctrl and Shift to come up (else a Ctrl+click)

std::string CategoryName(int c) {
    if (kCategories[c].flag == 0) return S(Str::CategorySims);
    if (kCategories[c].nameIndex < 0) return S(Str::CategoryOther);
    std::string n = game::GameString("UIText.package", kCategoryNames, kCategories[c].nameIndex);
    return n.empty() ? S(Str::CategoryOther) : n;
}

// A Sim's full name; else from its internal one ("E001_User00013 - Carlos" -> "Carlos").
std::string SimName(const LotObject& o) {
    std::string first, last;
    if (game::SimName(o.id, &first, &last)) return last.empty() ? first : first + " " + last;
    size_t dash = o.name.rfind(" - ");
    return dash == std::string::npos ? o.name : o.name.substr(dash + 3);
}

std::string DisplayName(const LotObject& o) {
    if (o.person) return SimName(o);
    std::string n = util::CleanForSpeech(o.catalogName.empty() ? o.name : o.catalogName);
    return n;
}

// What the browser lists: placed objects with a name a player would know, and the Sims.
bool Listed(const LotObject& o) {
    if (o.x < 0 || o.y < 0) return false;  // controllers and other objects kept off the lot
    if (o.person) return true;
    if (o.catalogName.empty() || o.catalogName[0] == '*') return false;  // internal objects
    if (o.name == "PlumbBob") return false;  // the selected Sim's marker
    return true;
}

bool InCategory(const LotObject& o, int c) {
    uint16_t flag = kCategories[c].flag;
    if (flag == 0) return o.person;
    if (o.person) return false;
    if (flag == 0xFFFF) return o.functionFlags == 0;
    return (o.functionFlags & flag) != 0;
}

// A room's name from what stands in it: each object votes for the buy mode rooms it is sold for
// (an object meant for many rooms votes less), the most votes wins. Room 0 is outside.
std::string RoomName(int level, int room, const std::vector<LotObject>& all) {
    static const struct {
        uint16_t flag;
        int nameIndex;  // UIText.package STR# 150
    } kRooms[] = {{0x0004, 5}, {0x0001, 4}, {0x0002, 2}, {0x0100, 17}, {0x0080, 3},
                  {0x0020, 1}, {0x0008, 0}, {0x0010, 6}, {0x0040, 7}};
    if (room == 0) return game::GameString("UIText.package", kCategoryNames, 6);
    double best = 0;
    int bestIndex = 0;  // living room when nothing tells
    for (const auto& r : kRooms) {
        double votes = 0;
        for (const LotObject& o : all) {
            if (o.person || o.level != level || o.room != room || !(o.roomFlags & r.flag)) continue;
            int bits = 0;
            for (uint16_t f = o.roomFlags; f; f &= f - 1) bits++;
            votes += 1.0 / bits;
        }
        if (votes > best + 1e-9) {
            best = votes;
            bestIndex = r.nameIndex;
        }
    }
    return game::GameString("UIText.package", kCategoryNames, bestIndex);
}

bool InRoom(const LotObject& o) { return o.level == g_roomLevel && o.room == g_roomId; }

bool SimPosition(LotObject* sim) {
    int id = game::SelectedSimId();
    return id && game::ReadLotObject(id, sim);
}

double Distance(const LotObject& a, const LotObject& b) { return std::hypot(a.x - b.x, a.y - b.y); }

// "3 casillas al noreste" from the selected Sim; -y is north, +x east (the lot's own axes).
std::string Where(const LotObject& o, const LotObject* sim) {
    if (!sim) return "";
    // Level 0 is the terrain, 1 the floor of a house on a foundation: both the ground floor.
    int floor = std::max(o.level, 1), simFloor = std::max(sim->level, 1);
    if (floor != simFloor) return util::Format(S(Str::FloorNumber), floor);
    double dx = o.x - sim->x, dy = o.y - sim->y;
    int tiles = (int)std::lround(std::hypot(dx, dy));
    if (tiles == 0) return S(Str::HereNow);
    static const Str kDirs[] = {Str::DirEast, Str::DirSouthEast, Str::DirSouth, Str::DirSouthWest,
                                Str::DirWest, Str::DirNorthWest, Str::DirNorth, Str::DirNorthEast};
    double angle = std::atan2(dy, dx);  // 0 east, pi/2 south (+y)
    int octant = ((int)std::lround(angle / (3.14159265358979 / 4)) + 8) % 8;
    const char* dir = S(kDirs[octant]);
    return tiles == 1 ? util::Format(S(Str::OneTileAway), dir) : util::Format(S(Str::TilesAway), tiles, dir);
}

std::vector<LotObject> ObjectsIn(int c, const LotObject* sim) {
    std::vector<LotObject> out;
    for (const LotObject& o : game::LotObjects())
        if (Listed(o) && (g_roomMode ? InRoom(o) : InCategory(o, c))) out.push_back(o);
    std::stable_sort(out.begin(), out.end(), [&](const LotObject& a, const LotObject& b) {
        if (!sim) return a.id < b.id;
        bool sa = a.level == sim->level, sb = b.level == sim->level;
        if (sa != sb) return sa;
        return Distance(a, *sim) < Distance(b, *sim);
    });
    return out;
}

void Speak(const LotObject& o, const LotObject* sim, const std::string& prefix) {
    std::string where = Where(o, sim);
    std::string line = DisplayName(o);
    if (!g_roomMode && o.room >= 0) line += ", " + RoomName(o.level, o.room, game::LotObjects());
    if (o.person && sim && o.id == sim->id) line += std::string(", ") + S(Str::SelectedSimSuffix);
    if (!where.empty()) line += ", " + where;
    std::vector<std::string> details = {line};
    if (!o.person && o.price > 0) details.push_back(util::Money(o.price));
    buffers::SetDetails(details);
    speech::Say(prefix.empty() ? line : prefix + ". " + line);
}

}  // namespace

std::string WhereFromSim(float x, float y, int level) {
    LotObject simObj;
    if (!SimPosition(&simObj)) return "";
    LotObject spot;
    spot.x = x;
    spot.y = y;
    spot.level = level;
    return Where(spot, &simObj);
}

void SayLayout(int x, int y, int level) {
    int room = game::RoomAt(x + 0.5f, y + 0.5f, level);
    if (room < 0) {
        speech::Say(S(Str::LayoutNoRoom));
        return;
    }
    std::vector<LotObject> all = game::LotObjects();
    std::string name = RoomName(level, room, all);
    // The room's tiles (off-lot tiles belong to no room of this level).
    int count = 0, x0 = 1 << 20, y0 = 1 << 20, x1 = -1, y1 = -1;
    for (int tx = 0; tx < 80; tx++)
        for (int ty = 0; ty < 80; ty++) {
            if (game::RoomAt(tx + 0.5f, ty + 0.5f, level) != room) continue;
            count++;
            x0 = std::min(x0, tx);
            y0 = std::min(y0, ty);
            x1 = std::max(x1, tx);
            y1 = std::max(y1, ty);
        }
    // Doors and windows on its walls, and what is on their other side.
    static const int kDx[] = {-1, 0, 1, 0}, kDy[] = {0, 1, 0, -1};           // 0 -x, 2 +y, 4 +x, 6 -y
    static const Str kFront[] = {Str::DirWest, Str::DirSouth, Str::DirEast, Str::DirNorth};
    static const Str kBehind[] = {Str::DirEast, Str::DirNorth, Str::DirWest, Str::DirSouth};
    int doors = 0, windows = 0;
    std::vector<std::string> lines;
    for (const game::WallObject& w : game::WallObjects()) {
        if (w.level != level) continue;
        int d = (w.direction / 2) & 3;
        float fx = w.x, fy = w.y, bx = w.x - kDx[d], by = w.y - kDy[d];
        int front = game::RoomAt(fx, fy, level), behind = game::RoomAt(bx, by, level);
        Str side;
        int other;
        float ox, oy;
        if (front == room && behind != room) {
            side = kBehind[d];
            other = behind;
            ox = bx;
            oy = by;
        } else if (behind == room && front != room) {
            side = kFront[d];
            other = front;
            ox = fx;
            oy = fy;
        } else {
            continue;
        }
        (w.type == 8 ? doors : windows)++;
        std::string otherName = other >= 0 ? RoomName(level, other, all) : "";
        std::string line = util::Format(S(w.type == 8 ? Str::LayoutDoor : Str::LayoutWindow), util::CleanForSpeech(w.name).c_str(),
                                        S(side), otherName.c_str(), (int)ox, (int)oy);
        if (w.name.empty()) {  // no name: "puerta , pared" -> "puerta, pared"
            size_t gap = line.find(" ,");
            if (gap != std::string::npos) line.erase(gap, 1);
        }
        lines.push_back(line);
    }
    std::string summary = util::Format(S(Str::LayoutSummary), name.c_str(), std::max(level, 1), count, x0, y0, x1, y1);
    summary += ", " + util::Format(S(Str::LayoutOpenings), doors, windows);
    std::vector<std::string> details = {summary};
    for (const std::string& l : lines) details.push_back(l);
    buffers::SetDetails(details);
    speech::Say(summary);
    Log("layout of room %d floor %d: %d tiles, %d doors, %d windows", room, level, count, doors, windows);
}

std::string RoomNameAt(float x, float y, int level) {
    int room = game::RoomAt(x, y, level);
    return room < 0 ? "" : RoomName(level, room, game::LotObjects());
}

bool LotViewActive() {
    // The live panel's proc or the buy/build panels: the lot view is up.
    Ptr main = game::MainWindow();
    std::vector<Ptr> stack = {main};
    int visited = 0;
    while (!stack.empty() && visited < 400) {
        Ptr w = stack.back();
        stack.pop_back();
        visited++;
        if (!game::Visible(w)) continue;
        std::string proc = game::ProcClass(w);
        if (proc == "cTSWinProcLivePanel" || proc == "cTSWinProcBuyPanel" || proc == "cTSWinProcBuildPanel" ||
            proc == "cTSWinProcSellPanel")  // buy mode while an object of the lot is held
            return true;
        for (Ptr c : game::Children(w)) stack.push_back(c);
    }
    return false;
}

void Reset() {
    g_category = 0;
    g_focusId = -1;
}

void NextObject(int delta) {
    LotObject simObj;
    const LotObject* sim = SimPosition(&simObj) ? &simObj : nullptr;
    std::vector<LotObject> list = ObjectsIn(g_category, sim);
    if (list.empty()) {
        speech::Say(CategoryName(g_category) + ", " + S(Str::NoObjects));
        return;
    }
    int at = -1;
    for (int i = 0; i < (int)list.size(); i++)
        if (list[i].id == g_focusId) at = i;
    int next = at < 0 ? (delta > 0 ? 0 : (int)list.size() - 1) : at + delta;
    if (next < 0) next = 0;
    if (next >= (int)list.size()) next = (int)list.size() - 1;
    g_focusId = list[next].id;
    Speak(list[next], sim, "");
}

void NextRoom(int delta) {
    LotObject simObj;
    const LotObject* sim = SimPosition(&simObj) ? &simObj : nullptr;
    std::vector<LotObject> all = game::LotObjects();
    // The rooms with something listed in them: the Sim's own first, then by floor and id.
    std::vector<std::pair<int, int>> rooms;
    for (const LotObject& o : all)
        if (Listed(o) && o.room >= 0) {
            std::pair<int, int> r = {o.level, o.room};
            if (std::find(rooms.begin(), rooms.end(), r) == rooms.end()) rooms.push_back(r);
        }
    if (rooms.empty()) {
        speech::Say(S(Str::NoObjects));
        return;
    }
    std::sort(rooms.begin(), rooms.end());
    if (sim && sim->room >= 0) {
        auto own = std::find(rooms.begin(), rooms.end(), std::make_pair(sim->level, sim->room));
        if (own != rooms.end()) std::rotate(rooms.begin(), own, own + 1);
    }
    int at = -1;
    for (int i = 0; i < (int)rooms.size(); i++)
        if (g_roomMode && rooms[i] == std::make_pair(g_roomLevel, g_roomId)) at = i;
    int next = at < 0 ? 0 : (at + delta + (int)rooms.size()) % (int)rooms.size();
    g_roomMode = true;
    g_roomLevel = rooms[next].first;
    g_roomId = rooms[next].second;
    std::vector<LotObject> list = ObjectsIn(0, sim);
    std::string name = RoomName(g_roomLevel, g_roomId, all);
    std::string summary = name + ", " + util::Format(S(Str::FloorNumber), std::max(g_roomLevel, 1)) + ", " +
                          (list.size() == 1 ? util::Format(S(Str::RoomObjectsOne), "").substr(2)
                                            : util::Format(S(Str::RoomObjects), "", (int)list.size()).substr(2));
    if (list.empty()) {
        speech::Say(summary);
        return;
    }
    g_focusId = list[0].id;
    Speak(list[0], sim, summary);
}

void NextCategory(int delta) {
    g_roomMode = false;
    LotObject simObj;
    const LotObject* sim = SimPosition(&simObj) ? &simObj : nullptr;
    // Skip empty categories.
    for (int step = 0; step < kCategoryCount; step++) {
        g_category = (g_category + delta + kCategoryCount) % kCategoryCount;
        std::vector<LotObject> list = ObjectsIn(g_category, sim);
        if (list.empty()) continue;
        g_focusId = list[0].id;
        std::string summary = util::Format(S(Str::GroupSummary), CategoryName(g_category).c_str(), (int)list.size());
        Speak(list[0], sim, summary);
        return;
    }
    speech::Say(S(Str::NoObjects));
}

void Interact() {
    LotObject o;
    if (g_focusId < 0 || !game::ReadLotObject(g_focusId, &o)) {
        speech::Say(S(Str::NoObjects));
        return;
    }
    // Buy mode: Enter picks the object up to move (or sell) it.
    if (game::CurrentModeClass() == "cTSUIModeBuy") {
        if (o.person) {
            speech::Say(util::Format(S(Str::PlaceCannotPick), DisplayName(o).c_str()));
            return;
        }
        placement::BeginMove(o.ptr, o.guid, DisplayName(o), o.x, o.y, o.level);
        return;
    }
    Log("interactions of object %d (%s)", o.id, DisplayName(o).c_str());
    if (game::ShowInteractions(o.id)) g_menuAskedAt = GetTickCount64();
}

void WalkTo(bool run) {
    LotObject o;
    if (g_focusId < 0 || !game::ReadLotObject(g_focusId, &o) || o.x < 0) {
        speech::Say(S(Str::NoObjects));
        return;
    }
    if (!game::SelectedSimId()) {
        speech::Say(S(Str::NoSimSelected));
        return;
    }
    // A click on the object's tile with nothing under the cursor: the game offers "go here" and
    // "run here" (and routes to the nearest place it can reach). The click waits for Ctrl to be
    // released: the game tracks it from the key messages, and a Ctrl+click opens no menu.
    Log("walk to %s at %.1f,%.1f floor %d", DisplayName(o).c_str(), o.x, o.y, o.level);
    game::SetPickOverride(true, o.x, o.y, o.level);
    game::ForceNoObjectPick();
    g_walkClickPending = true;
    g_walkAskedAt = GetTickCount64();
    g_walkRun = run;
}

void Tick() {
    if (g_walkClickPending) {
        if (game::ModifierHeld(VK_CONTROL) || game::ModifierHeld(VK_SHIFT)) {
            if (GetTickCount64() - g_walkAskedAt < 5000) return;
        }
        g_walkClickPending = false;
        game::MouseClick(1280, 640);
        g_walkAskedAt = GetTickCount64();
    } else if (g_walkAskedAt) {
        Ptr modal = game::ModalWindow();
        bool menu = modal && game::IsKindOf(modal, "cTSWinPieMenu");
        if (menu || GetTickCount64() - g_walkAskedAt > 1500) {
            game::SetPickOverride(false);
            game::ClearObjectPick();
            g_walkAskedAt = 0;
            Group pie = menu ? PieGroup(modal) : Group();
            size_t choice = g_walkRun ? 1 : 0;
            if (pie.items.size() > choice) {
                Log("walk: choosing \"%s\"", pie.items[choice].label.c_str());
                game::ClickWindow(pie.items[choice].widget);
                speech::Say(S(Str::WalkingThere));
            } else {
                speech::Say(S(Str::CannotWalk));
            }
        }
    }
    if (!g_menuAskedAt) return;
    Ptr modal = game::ModalWindow();
    if (modal && game::IsKindOf(modal, "cTSWinPieMenu")) {
        g_menuAskedAt = 0;
        return;
    }
    if (GetTickCount64() - g_menuAskedAt > 1500) {
        g_menuAskedAt = 0;
        speech::Say(S(Str::NoInteractions));
    }
}

void Repeat() {
    LotObject o;
    if (g_focusId < 0 || !game::ReadLotObject(g_focusId, &o)) {
        speech::Say(S(Str::NoObjects));
        return;
    }
    LotObject simObj;
    Speak(o, SimPosition(&simObj) ? &simObj : nullptr, "");
}

}  // namespace nav::world
