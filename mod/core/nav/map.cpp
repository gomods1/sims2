#include "map.h"

#include <algorithm>
#include <cstdlib>
#include <string>
#include <vector>

#include "../game/addresses.h"
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

// What each step says (comma / period cycle them; a tester of the Sims 1 mod asked for it, after
// Oxygen Not Included's overlays: everything at once was too much on a furnished lot).
enum Layer { kAll, kObjects, kFloors, kWalls, kRooms, kLayers };

struct State {
    bool active = false;
    int layer = kAll;
    std::string lastFloor;
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

// What stands on the tile: Sims first, then what stands on something (the computer on its desk:
// what Enter takes), then the rest. Surfaces (tables, desks, counters) are what the others stand on.
std::vector<game::LotObject> ThingsHere() {
    std::vector<game::LotObject> here = world::ThingsAt(g.x, g.y, g.level);
    std::stable_partition(here.begin(), here.end(), [](const game::LotObject& o) {
        return !(o.functionFlags & game::addr::objd::kFnSurfaces);
    });
    std::stable_partition(here.begin(), here.end(), [](const game::LotObject& o) { return o.person; });
    return here;
}

// The floor in words: its catalog name ("Baldosa de granito claro"), else what it is made of.
std::string FloorWords(const cues::TileSense& t) {
    std::string name = util::CleanForSpeech(game::FloorName(g.level, g.x, g.y));
    return name.empty() ? cues::SurfaceWord(t.surface) : name;
}

// The line for the cursor's tile: what stands there, the room when it changes, the walls and
// openings; the tile last (as the build cursor's line). The layer chosen with comma / period
// keeps one kind of these.
std::string Line(bool full) {
    std::vector<std::string> parts;
    int layer = g.layer;
    bool all = layer == kAll;
    if (all || layer == kObjects) {
        std::vector<game::LotObject> here = ThingsHere();
        for (const game::LotObject& o : here) parts.push_back(ThingWords(o));
        if (here.empty() && layer == kObjects) parts.push_back(S(Str::MapNothingHere));
    }
    cues::TileSense t = cues::Sense(g.level, g.x, g.y);
    if (t.offLot) parts.push_back(S(Str::PlaceOffLot));
    if (t.stairs && (all || layer == kObjects || layer == kWalls)) parts.push_back(S(Str::MapStairs));
    std::string roomName = t.room >= 0 ? world::RoomNameAt(g.x + 0.5f, g.y + 0.5f, g.level) : "";
    bool roomChanged = t.room != g.lastRoom || g.level != g.lastLevel;
    if (!roomName.empty() && layer == kRooms) {
        parts.push_back(roomName);
    } else if (!roomName.empty() && layer != kFloors && (full || roomChanged)) {
        if (!full && !g.lastRoomName.empty() && g.lastLevel == g.level && g.lastRoomName != roomName)
            parts.push_back(util::Format(S(Str::RoomChange), g.lastRoomName.c_str(), roomName.c_str()));
        else
            parts.push_back(roomName);
    }
    if (!roomName.empty() && layer != kFloors && (full || roomChanged || layer == kRooms)) {
        std::string light = world::RoomLight(g.x, g.y, g.level);
        if (!light.empty()) parts.push_back(light);
    }
    g.lastRoom = t.room;
    g.lastLevel = g.level;
    if (!roomName.empty()) g.lastRoomName = roomName;
    // The floor when it changes (grass to paving, one tile to another), always with R.
    std::string floor = FloorWords(t);
    if (layer == kFloors || (all && (full || !g.haveSurface || floor != g.lastFloor))) parts.push_back(floor);
    g.lastSurface = t.surface;
    g.lastFloor = floor;
    g.haveSurface = true;
    if (all || layer == kWalls) {
        std::string edges = cues::EdgesLine(t);
        if (!edges.empty()) parts.push_back(edges);
        else if (layer == kWalls) parts.push_back(S(Str::MapNoWalls));
    }
    if (t.sloped && (all || layer == kFloors)) parts.push_back(S(Str::TerrainSloped));
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

void NextLayer(int delta) {
    g.layer = ((g.layer + delta) % kLayers + kLayers) % kLayers;
    static const Str kNames[kLayers] = {Str::MapLayerAll, Str::MapLayerObjects, Str::MapLayerFloors, Str::MapLayerWalls,
                                        Str::MapLayerRooms};
    std::string line = Line(true);
    buffers::SetDetails({line});
    speech::Say(std::string(S(kNames[g.layer])) + ". " + line);
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
    std::vector<game::LotObject> here = ThingsHere();
    if (here.empty()) {
        speech::Say(S(Str::MapNothingHere));
        return;
    }
    // A Sim standing there first, else what stands on something (the computer, not its desk).
    world::Focus(here.front().id);
    world::Interact();
}

}  // namespace

void Reset() { g = State{}; }

void Move(int dx, int dy, bool throughWalls) {
    if (!g.active) return;
    int w = kGrid, h = kGrid;
    game::LotSize(&w, &h);
    // One tile at a time: a wall, fence or window stops the cursor (a door does not), as walking
    // would; a tester of the Sims 1 mod left rooms by accident. Alt with the arrow goes through.
    int steps = std::max(std::abs(dx), std::abs(dy));
    int sx = dx > 0 ? 1 : dx < 0 ? -1 : 0, sy = dy > 0 ? 1 : dy < 0 ? -1 : 0;
    int moved = 0;
    std::string wall;
    for (int i = 0; i < steps; i++) {
        int x = std::clamp(g.x + sx, 0, w - 1), y = std::clamp(g.y + sy, 0, h - 1);
        if ((x == g.x && y == g.y) || cues::Sense(g.level, x, y).offLot) break;
        if (!throughWalls && cues::WallStops(g.level, g.x, g.y, sx, sy, &wall)) break;
        g.x = x;
        g.y = y;
        moved++;
    }
    if (moved == 0) {
        if (!wall.empty()) {
            speech::Say(wall);
            cues::Blocked();
        } else {
            speech::Say(S(Str::MapLotEdge));
            cues::LotEdge();
        }
        return;
    }
    SayHere();
    if (!wall.empty()) speech::Say(wall);  // stopped before it after a few steps (Shift)
}

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
    case VK_OEM_COMMA: case VK_OEM_PERIOD:  // the layer each step says
        if (ctrl) return false;
        NextLayer(vk == VK_OEM_PERIOD ? +1 : -1);
        return true;
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
