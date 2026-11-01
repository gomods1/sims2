#include "map.h"

#include <algorithm>
#include <string>
#include <vector>

#include "../game/lotgrid.h"
#include "../game/objects.h"
#include "../speech/speech.h"
#include "../util/log.h"
#include "../util/strings.h"
#include "../util/text.h"
#include "buffers.h"
#include "build.h"
#include "cues.h"
#include "placement.h"
#include "world.h"

namespace nav::map {
namespace {

constexpr int kGrid = 80;     // the largest lot is 64 tiles a side; the scan of placing looks this far
constexpr int kMaxLevel = 5;  // level as game::LotObject::level: 0 the terrain, then the floors

struct State {
    bool active = false;
    int x = 0, y = 0, level = 0;
    int lastRoom = -2;
    int lastLevel = 0;
    std::string lastRoomName;
    game::Surface lastSurface = game::Surface::Ground;
    bool haveSurface = false;
};

State g;

// "Cama doble, mirando al sur"; Sims by name.
std::string ThingWords(const game::LotObject& o) {
    std::string name = world::DisplayName(o);
    if (o.person) return name;
    static const Str kFacing[] = {Str::DirWest, Str::DirSouth, Str::DirEast, Str::DirNorth};
    return name + ", " + util::Format(S(Str::PlaceFacing), S(kFacing[(o.direction / 2) & 3]));
}

// The line for the cursor's tile: what stands there, the room when it changes, the walls and
// openings; the tile last (as the build cursor's line).
std::string Line(bool full) {
    std::vector<std::string> parts;
    std::vector<game::LotObject> here = world::ThingsAt(g.x, g.y, g.level);
    for (const game::LotObject& o : here) parts.push_back(ThingWords(o));
    cues::TileSense t = cues::Sense(g.level, g.x, g.y);
    if (t.offLot) parts.push_back(S(Str::PlaceOffLot));
    if (t.stairs) parts.push_back(S(Str::MapStairs));
    std::string roomName = t.room >= 0 ? world::RoomNameAt(g.x + 0.5f, g.y + 0.5f, g.level) : "";
    bool roomChanged = t.room != g.lastRoom || g.level != g.lastLevel;
    if (!roomName.empty() && (full || roomChanged)) {
        if (!full && !g.lastRoomName.empty() && g.lastLevel == g.level && g.lastRoomName != roomName)
            parts.push_back(util::Format(S(Str::RoomChange), g.lastRoomName.c_str(), roomName.c_str()));
        else
            parts.push_back(roomName);
    }
    g.lastRoom = t.room;
    g.lastLevel = g.level;
    if (!roomName.empty()) g.lastRoomName = roomName;
    // The floor when it changes (grass to paving, wood to carpet), always with R.
    if (full || !g.haveSurface || t.surface != g.lastSurface) parts.push_back(cues::SurfaceWord(t.surface));
    g.lastSurface = t.surface;
    g.haveSurface = true;
    std::string edges = cues::EdgesLine(t);
    if (!edges.empty()) parts.push_back(edges);
    if (t.sloped) parts.push_back(S(Str::TerrainSloped));
    std::string line;
    for (const std::string& p : parts) line += (line.empty() ? "" : ", ") + p;
    return line + (line.empty() ? "" : ". ") + util::Format(S(Str::PlaceTile), g.x, g.y);
}

void SayHere(bool full = false) {
    std::string line = Line(full);
    std::vector<std::string> details = {line};
    for (const game::LotObject& o : world::ThingsAt(g.x, g.y, g.level)) {
        std::string where = world::DisplayName(o);
        if (!o.person && o.price > 0) where += ", " + util::Money(o.price);
        details.push_back(where);
    }
    details.push_back(util::Format(S(Str::FloorNumber), g.level));
    buffers::SetDetails(details);
    speech::Say(line);
    cues::Tile(g.level, g.x, g.y);
}

bool AtSim() {
    game::LotObject sim;
    int id = game::SelectedSimId();
    if (!id || !game::ReadLotObject(id, &sim) || sim.x < 0) return false;
    g.x = (int)sim.x;
    g.y = (int)sim.y;
    g.level = std::clamp(sim.level, 0, kMaxLevel);
    return true;
}

// Whether a floor has anything built on it: a floor tile (the terrain always counts).
bool FloorExists(int level) {
    if (level == 0) return true;
    int w = 0, h = 0;
    if (!game::LotSize(&w, &h)) return level == 1;
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
            if (game::FloorPatternAt(level, x, y)) return true;
    return false;
}

void Move(int dx, int dy) {
    int w = kGrid, h = kGrid;
    game::LotSize(&w, &h);
    int x = std::clamp(g.x + dx, 0, w - 1), y = std::clamp(g.y + dy, 0, h - 1);
    if ((x == g.x && y == g.y) || cues::Sense(g.level, x, y).offLot) {
        speech::Say(S(Str::MapLotEdge));
        cues::LotEdge();
        return;
    }
    g.x = x;
    g.y = y;
    SayHere();
}

void ChangeFloor(int delta) {
    int want = g.level + delta;
    if (want < 0 || want > kMaxLevel || !FloorExists(want)) {
        speech::Say(util::Format(S(Str::MapNoFloor), std::max(want, 1)));
        return;
    }
    g.level = want;
    std::string line = Line(true);
    buffers::SetDetails({line});
    speech::Say(util::Format(S(Str::FloorNumber), g.level) + ". " + line);
    cues::Tile(g.level, g.x, g.y);
}

void Activate() {
    std::vector<game::LotObject> here = world::ThingsAt(g.x, g.y, g.level);
    if (here.empty()) {
        speech::Say(S(Str::MapNothingHere));
        return;
    }
    // A Sim standing there first, else the object.
    world::Focus(here.front().id);
    world::Interact();
}

}  // namespace

void Reset() { g = State{}; }

bool Active() { return g.active; }

bool CursorTile(int* x, int* y, int* level) {
    if (!g.active) return false;
    *x = g.x;
    *y = g.y;
    *level = g.level;
    return true;
}

void Tick() {
    if (!g.active) return;
    // An interaction menu or a dialog over the lot only takes the keys for a while (the map's keys
    // work in the lot view alone); leaving the lot or a building tool ends the map.
    if (!world::LotViewActive() || build::Active() || placement::Active()) {
        Log("map: ended (view or tool changed)");
        g.active = false;
    }
}

void Toggle() {
    if (g.active) {
        End();
        return;
    }
    if (!world::LotViewActive()) return;
    if (build::Active() || placement::Active()) return;  // those cursors own the space already
    if (!AtSim()) {
        g.x = g.y = 10;
        g.level = 0;
    }
    g.active = true;
    g.lastRoom = -2;
    g.lastRoomName.clear();
    g.haveSurface = false;
    cues::Reset();
    std::string line = Line(true);
    buffers::SetDetails({line});
    speech::Say(std::string(S(Str::MapOn)) + " " + line);
    cues::Tile(g.level, g.x, g.y);
}

void End() {
    if (!g.active) return;
    g.active = false;
    speech::Say(S(Str::MapOff));
}

bool OnKey(WPARAM vk, bool shift, bool ctrl) {
    if (!g.active) return false;
    int step = shift ? 5 : 1;
    switch (vk) {
    case VK_UP: if (ctrl) return false; Move(0, -step); return true;
    case VK_DOWN: if (ctrl) return false; Move(0, step); return true;
    case VK_LEFT: if (ctrl) return false; Move(-step, 0); return true;
    case VK_RIGHT: if (ctrl) return false; Move(step, 0); return true;
    case VK_HOME:
        if (ctrl) return false;
        if (!AtSim()) speech::Say(S(Str::NoSimSelected));
        else SayHere();
        return true;
    case VK_NEXT: case VK_PRIOR:
        if (!ctrl || !shift) return false;  // the lot's things: the keyboard's own handling
        ChangeFloor(vk == VK_PRIOR ? +1 : -1);
        return true;
    case VK_RETURN:
        if (ctrl) world::WalkToSpot(g.x + 0.5f, g.y + 0.5f, g.level, shift);
        else Activate();
        return true;
    case 'R':
        if (ctrl || shift) return false;
        SayHere(true);
        return true;
    case VK_ESCAPE:
        End();
        return true;
    default:
        return false;
    }
}

void FollowFocus() {
    if (!g.active) return;
    game::LotObject o;
    if (world::FocusedId() < 0 || !game::ReadLotObject(world::FocusedId(), &o) || o.x < 0) return;
    g.x = (int)o.x;
    g.y = (int)o.y;
    g.level = std::clamp(o.level, 0, kMaxLevel);
    g.lastRoom = game::RoomAt(g.x + 0.5f, g.y + 0.5f, g.level);  // the browser named the room already
    g.lastLevel = g.level;
    g.lastRoomName = world::RoomNameAt(g.x + 0.5f, g.y + 0.5f, g.level);
    cues::Tile(g.level, g.x, g.y);
}

}  // namespace nav::map
