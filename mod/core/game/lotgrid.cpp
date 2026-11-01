#include "lotgrid.h"

#include <cstdlib>
#include <map>

#include "../util/log.h"
#include "addresses.h"
#include "game_strings.h"
#include "memory.h"
#include "objects.h"

namespace game {
namespace {

template <class Fn>
Fn Slot(Ptr obj, int slot) {
    Ptr vt = Rd<Ptr>(obj);
    return InImage(vt) ? (Fn)Rd<Ptr>(vt + slot * 4) : nullptr;
}

Ptr Primary() {
    Ptr w = WorldDB();
    return w ? w - addr::grid::kInterface : 0;
}

// The floor pattern array: c3DArray<cFloorPatternStorageTile>, [level + 1][x][y], 8 bytes a tile
// (the pattern of each quarter).
struct Floors {
    Ptr data = 0;
    int nx = 0, ny = 0, nl = 0;
};

bool ReadFloors(Floors* f) {
    Ptr p = Primary();
    Ptr arr = p ? Rd<Ptr>(p + addr::grid::kFloorPatterns) : 0;
    if (!arr) return false;
    f->nx = Rd<int>(arr + 0x08);
    f->ny = Rd<int>(arr + 0x0c);
    f->nl = Rd<int>(arr + 0x10);
    f->data = Rd<Ptr>(arr + 0x14);
    return f->data && f->nx > 0 && f->nx <= 256 && f->ny > 0 && f->ny <= 256 && f->nl > 0 && f->nl <= 16;
}

std::map<int, Surface> g_bySurface;  // pattern id -> surface, for the lot loaded (names can repeat)
Ptr g_namesFor = 0;

// From the floor's catalog definition, "surfacetype/subsort": the subsort says what a player
// would call it (carpet, wood, tile, linoleum, stone, brick, poured), the surface type what its
// footsteps sound like when the subsort is "other".
Surface FromSurfaceType(const std::string& both) {
    size_t slash = both.find('/');
    std::string t = both.substr(0, slash), sub = slash == std::string::npos ? "" : both.substr(slash + 1);
    if (sub == "carpet") return Surface::Carpet;
    if (sub == "wood") return Surface::Wood;
    if (sub == "tile" || sub == "lino") return Surface::Tile;
    if (sub == "stone" || sub == "brick" || sub == "poured") return Surface::Stone;
    if (t == "cpet") return Surface::Carpet;
    if (t == "wood" || t == "wdeck") return Surface::Wood;
    if (t == "cment") return Surface::Stone;
    if (t == "gravel") return Surface::Gravel;
    if (t == "grass") return Surface::Grass;
    return Surface::Tile;
}

// A built-in floor by its name ("sidewalk", "road_asphalt", "floor_plywoodfoundation",
// "floor_poolroundedlipblue"), or a catalog floor by its GUID in hex.
Surface FromName(const std::string& name) {
    if (name.rfind("floor_pool", 0) == 0) return Surface::PoolRim;
    if (name.find("wood") != std::string::npos) return Surface::Wood;
    if (name.rfind("sidewalk", 0) == 0 || name.rfind("road", 0) == 0) return Surface::Stone;
    char* end = nullptr;
    unsigned long guid = strtoul(name.c_str(), &end, 16);
    if (end && *end == 0 && !name.empty()) return FromSurfaceType(FloorSurfaceType((uint32_t)guid));
    return Surface::Tile;
}

}  // namespace

int FloorPatternAt(int level, int x, int y) {
    Floors f;
    if (!ReadFloors(&f)) return 0;
    int l = level + 1;
    if (x < 0 || y < 0 || x >= f.nx || y >= f.ny || l < 0 || l >= f.nl) return 0;
    uint16_t q[4] = {};
    Read(f.data + (((size_t)l * f.nx + x) * f.ny + y) * 8, q, sizeof q);
    for (uint16_t v : q)
        if (v) return v;
    return 0;
}

std::string FloorPatternName(int id) {
    Ptr p = Primary();
    Ptr names = p ? Rd<Ptr>(p + addr::grid::kPatternNames) : 0;
    Ptr head = names ? Rd<Ptr>(names + addr::grid::kPatternById) : 0;
    if (!head) return "";
    // std::map<u16, const char*>: left +0, parent +4, right +8, is-nil byte +0xd, key +0x10, value +0x14
    Ptr node = Rd<Ptr>(head + 4);
    for (int depth = 0; node && node != head && !Rd<uint8_t>(node + 0xd) && depth < 64; depth++) {
        int key = Rd<uint16_t>(node + 0x10);
        if (key == id) {
            // the name's characters (a c_str() into the map's own string)
            char buf[64] = {};
            Ptr s = Rd<Ptr>(node + 0x14);
            if (!s || !Read(s, buf, sizeof buf - 1)) return "";
            return buf;
        }
        node = Rd<Ptr>(node + (id < key ? 0 : 8));
    }
    return "";
}

Surface SurfaceAt(int level, int x, int y) {
    // A pool: its floor lies a level below the ground ("floor_pool..." at level -1); a pond: the
    // game's own water test.
    if (level == 0) {
        int below = FloorPatternAt(-1, x, y);
        if (below && FloorPatternName(below).rfind("floor_pool", 0) == 0) return Surface::Water;
        if (WaterOnTile(x, y)) return Surface::Water;
    }
    int id = FloorPatternAt(level, x, y);
    if (!id) return level > 0 ? Surface::NoFloor : Surface::Ground;
    Ptr p = Primary();
    if (p != g_namesFor) {  // another lot: the ids name other floors
        g_bySurface.clear();
        g_namesFor = p;
    }
    auto it = g_bySurface.find(id);
    if (it != g_bySurface.end()) return it->second;
    Surface s = FromName(FloorPatternName(id));
    g_bySurface[id] = s;
    return s;
}

bool WallBetween(int level, int x0, int y0, int x1, int y1, int layer) {
    Ptr w = WorldDB();
    if (!w) return false;
    struct Point {
        float x, y;
        int level;
    } a = {x0 + 0.5f, y0 + 0.5f, level}, b = {x1 + 0.5f, y1 + 0.5f, level};
    typedef Ptr(__thiscall * GetFn)(Ptr);
    typedef Ptr(__thiscall * LayerFn)(Ptr, int);
    typedef bool(__thiscall * BetweenFn)(Ptr, Point*, Point*, int, int, int);
    bool wall = false;
    Guarded([&] {
        Ptr pm = Slot<GetFn>(w, addr::world::kSlotRoomManager)(w);
        Ptr walls = pm ? Slot<LayerFn>(pm, addr::grid::kPartitionSlotLayer)(pm, layer) : 0;
        if (walls) wall = Slot<BetweenFn>(walls, addr::grid::kLayerSlotBetween)(walls, &a, &b, 0, 0, 0);
    });
    return wall;
}

bool WaterOnTile(int x, int y) {
    if (!FunctionOk(addr::grid::kWaterOnTile)) return false;
    typedef bool(__cdecl * WaterFn)(const int*);
    int tile[3] = {x, y, 0};
    bool water = false;
    Guarded([&] { water = ((WaterFn)Va(addr::grid::kWaterOnTile))(tile); });
    return water;
}

bool TerrainHeight(int x, int y, float* out) {
    Ptr w = WorldDB();
    if (!w) return false;
    typedef float(__thiscall * HeightFn)(Ptr, int, int);
    return Guarded([&] { *out = Slot<HeightFn>(w, addr::grid::kSlotElevation)(w, x, y); });
}

bool LotSize(int* width, int* depth) {
    Floors f;
    if (!ReadFloors(&f)) return false;
    *width = f.nx;
    *depth = f.ny;
    return true;
}

}  // namespace game
