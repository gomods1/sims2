#include "cues.h"

#include <windows.h>

#include <algorithm>
#include <cmath>
#include <vector>

#include "../game/objects.h"
#include "../speech/speech.h"
#include "../util/log.h"
#include "../util/settings.h"
#include "../util/text.h"
#include "world.h"

namespace nav::cues {
namespace {

int g_enabled = -1;   // read from the ini on first use
int g_volume = -1;    // percent
int g_lastRoom = -2;  // the room the last tile cue was in (-2: none yet)
int g_lastLevel = 0;

// Each side's sound: west to the left, east to the right, north a little higher, south lower.
struct Placement {
    float pan, pitch;
};
const Placement kPlace[kSides] = {{0.0f, 1.22f}, {0.8f, 1.0f}, {0.0f, 0.82f}, {-0.8f, 1.0f}};
const int kDx[kSides] = {0, 1, 0, -1}, kDy[kSides] = {-1, 0, 1, 0};  // -y is north
// An object's direction (0 faces -x, 2 +y, 4 +x, 6 -y) as the side it faces.
int SideOfDirection(int direction) {
    switch ((direction / 2) & 3) {
    case 0: return kWest;
    case 1: return kSouth;
    case 2: return kEast;
    default: return kNorth;
    }
}

float Volume() {
    if (g_volume < 0) g_volume = util::GetSetting("cue_volume", 80);
    return std::clamp(g_volume, 0, 100) / 100.0f;
}

void Play(const char* name, float pan = 0, float pitch = 1, float gain = 1, int delay = 0) {
    if (!Enabled()) return;
    speech::Cue(name, pan, pitch, gain * Volume(), delay);
}

const char* SurfaceSound(game::Surface s) {
    switch (s) {
    case game::Surface::Wood: return "floor_wood";
    case game::Surface::Tile: return "floor_tile";
    case game::Surface::Carpet: return "floor_carpet";
    case game::Surface::Stone: case game::Surface::Gravel: case game::Surface::PoolRim: return "floor_stone";
    case game::Surface::Water: return "floor_water";
    case game::Surface::NoFloor: return "floor_none";
    default: return "floor_grass";
    }
}

const char* EdgeSound(Edge e) {
    switch (e) {
    case Edge::Fence: case Edge::Railing: return "fence";
    case Edge::Door: return "door";
    case Edge::Window: return "window";
    default: return "wall";
    }
}

// The lot's parts, read at most every half second: a step senses up to three tiles, and each read
// goes through every object of the lot.
struct Snapshot {
    ULONGLONG at = 0;
    std::vector<game::PlacedPart> parts;
} g_snap;

const std::vector<game::PlacedPart>& Parts() {
    ULONGLONG now = GetTickCount64();
    if (!g_snap.at || now - g_snap.at > 500) {
        g_snap.parts = game::PlacedParts();
        g_snap.at = now;
    }
    return g_snap.parts;
}

// What stands between tile (x, y) and the next tile on `side`: a door or window in that wall (one
// of its front parts stands on one of the two tiles with the wall behind it), else what the walls'
// layers say (game/lotgrid.h): a wall (load-bearing or a plain wall), a railing (a half wall, or a
// fence upstairs), a fence.
Edge EdgeOf(int level, int x, int y, int side) {
    int nx = x + kDx[side], ny = y + kDy[side];
    for (const game::PlacedPart& p : Parts()) {
        if (p.level != level || (p.type != 8 && p.type != 9) || (p.sub & 0xff) != 0) continue;
        int back = SideOfDirection(p.direction + 4);  // the wall behind the part
        bool here = p.tileX == x && p.tileY == y && back == side;
        bool there = p.tileX == nx && p.tileY == ny && back == (side + 2) % kSides;
        if (here || there) return p.type == 8 ? Edge::Door : Edge::Window;
    }
    // Layer 7 holds everything drawn, 5 everything that blocks: neither, nothing stands there.
    bool layer[8] = {};
    layer[7] = game::WallBetween(level, x, y, nx, ny, 7);
    layer[5] = game::WallBetween(level, x, y, nx, ny, 5);
    if (!layer[7] && !layer[5]) return Edge::None;
    for (int l : {0, 2, 3, 4}) layer[l] = game::WallBetween(level, x, y, nx, ny, l);
    if (layer[0] || layer[4]) return Edge::Wall;
    if (layer[2] || layer[3]) return Edge::Railing;
    if (layer[5] && layer[7]) return level > 0 ? Edge::Railing : Edge::Fence;
    return Edge::None;
}

}  // namespace

TileSense Sense(int level, int x, int y, int ignore) {
    TileSense s;
    int w = 0, h = 0;
    s.offLot = game::LotSize(&w, &h) && (x < 0 || y < 0 || x >= w || y >= h);
    if (s.offLot) return s;
    s.room = game::RoomAt(x + 0.5f, y + 0.5f, level);
    s.surface = game::SurfaceAt(level, x, y);
    for (int side = 0; side < kSides; side++) s.sides[side] = EdgeOf(level, x, y, side);
    for (const game::PlacedPart& p : Parts()) {
        if (p.level != level || p.tileX != x || p.tileY != y) continue;
        if (p.type == 8 || p.type == 9 || (ignore && (p.lead == ignore || p.id == ignore))) continue;
        if (p.stairs) s.stairs = true;
        else if (p.person) s.person = true;
        else if (p.lead && world::IsListed(p.lead)) s.object = true;
    }
    float c[4];
    if (level <= 0 && game::TerrainHeight(x, y, &c[0]) && game::TerrainHeight(x + 1, y, &c[1]) &&
        game::TerrainHeight(x, y + 1, &c[2]) && game::TerrainHeight(x + 1, y + 1, &c[3]))
        s.sloped = std::max({c[0], c[1], c[2], c[3]}) - std::min({c[0], c[1], c[2], c[3]}) > 0.05f;
    return s;
}

bool Enabled() {
    if (g_enabled < 0) g_enabled = util::GetSetting("cues", 1) ? 1 : 0;
    return g_enabled == 1;
}

void Toggle() {
    g_enabled = Enabled() ? 0 : 1;
    util::SetSetting("cues", g_enabled);
    speech::Say(S(g_enabled ? Str::CuesOn : Str::CuesOff));
    if (g_enabled) Play("fits");
}

void Reset() {
    g_snap = Snapshot{};
    g_lastRoom = -2;
    g_lastLevel = 0;
}

namespace {

// The sounds of a sensed tile, with the sides given (a tile's own, or a corner's lines).
void PlayTile(const TileSense& s, int level, const Edge sides[kSides]) {
    if (s.offLot) {
        Play("edge");
        return;
    }
    Play(SurfaceSound(s.surface), 0, 1, 1, 0);
    if (g_lastRoom != -2 && (s.room != g_lastRoom || level != g_lastLevel)) Play("room", 0, 1, 0.8f, 0);
    g_lastRoom = s.room;
    g_lastLevel = level;
    int delay = 70;
    for (int side = 0; side < kSides; side++) {
        if (sides[side] == Edge::None) continue;
        Play(EdgeSound(sides[side]), kPlace[side].pan, kPlace[side].pitch, 1, delay);
        delay += 45;
    }
    if (s.stairs) {
        Play("stairs", 0, 1, 0.9f, delay);
        delay += 60;
    }
    if (s.person) {
        Play("person", 0, 1, 0.8f, delay);
        delay += 60;
    } else if (s.object) {
        Play("object", 0, 1, 0.9f, delay);
        delay += 60;
    }
    if (s.sloped) Play("slope", 0, 1, 0.6f, delay);
}

}  // namespace

void Tile(int level, int x, int y, int ignore) {
    if (!Enabled()) return;
    TileSense s = Sense(level, x, y, ignore);
    PlayTile(s, level, s.sides);
}

void Corner(int level, int x, int y) {
    if (!Enabled()) return;
    TileSense s = Sense(level, x, y);  // the tile south-east of the corner: its floor, its things
    // The four grid lines leaving the corner, as edges of the tiles beside them: north is the west
    // edge of the tile north of it, east the north edge of its own tile, south its own west edge,
    // west the north edge of the tile west of it.
    Edge lines[kSides];
    lines[kNorth] = EdgeOf(level, x, y - 1, kWest);
    lines[kEast] = s.sides[kNorth];
    lines[kSouth] = s.sides[kWest];
    lines[kWest] = EdgeOf(level, x - 1, y, kNorth);
    PlayTile(s, level, lines);
}

void LotEdge() { Play("edge"); }

void Fit(bool fits) { Play(fits ? "fits" : "nofit", 0, 1, 0.7f, 0); }

std::string SurfaceWord(game::Surface s) {
    switch (s) {
    case game::Surface::Grass: return S(Str::SurfaceGrass);
    case game::Surface::Wood: return S(Str::SurfaceWood);
    case game::Surface::Tile: return S(Str::SurfaceTile);
    case game::Surface::Carpet: return S(Str::SurfaceCarpet);
    case game::Surface::Stone: return S(Str::SurfacePaving);
    case game::Surface::Gravel: return S(Str::SurfaceGravel);
    case game::Surface::PoolRim: return S(Str::SurfacePoolRim);
    case game::Surface::Water: return S(Str::SurfaceWater);
    case game::Surface::NoFloor: return S(Str::SurfaceNone);
    default: return S(Str::SurfaceGround);
    }
}

Str SideWord(int side) {
    const Str words[kSides] = {Str::DirNorth, Str::DirEast, Str::DirSouth, Str::DirWest};
    return words[side & 3];
}

namespace {

std::string SidesLine(const Edge sides[kSides]) {
    std::string line;
    for (int side = 0; side < kSides; side++) {
        Str form;
        switch (sides[side]) {
        case Edge::Wall: form = Str::EdgeWall; break;
        case Edge::Fence: form = Str::EdgeFence; break;
        case Edge::Railing: form = Str::EdgeRailing; break;
        case Edge::Door: form = Str::EdgeDoor; break;
        case Edge::Window: form = Str::EdgeWindow; break;
        default: continue;
        }
        line += (line.empty() ? "" : ", ") + util::Format(S(form), S(SideWord(side)));
    }
    return line;
}

}  // namespace

std::string EdgesLine(const TileSense& t) { return SidesLine(t.sides); }

std::string CornerEdgesLine(int level, int x, int y) {
    Edge lines[kSides] = {EdgeOf(level, x, y - 1, kWest), EdgeOf(level, x, y, kNorth), EdgeOf(level, x, y, kWest),
                          EdgeOf(level, x - 1, y, kNorth)};
    return SidesLine(lines);
}

}  // namespace nav::cues
