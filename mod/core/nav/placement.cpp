#include "placement.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <map>
#include <set>
#include <vector>

#include "../game/addresses.h"
#include "../game/buy.h"
#include "../game/cheats.h"
#include "../game/lotgrid.h"
#include "../game/mouse.h"
#include "../game/objects.h"
#include "../input/keyboard.h"
#include "../speech/speech.h"
#include "../util/log.h"
#include "../util/strings.h"
#include "../util/text.h"
#include "buffers.h"
#include "cues.h"
#include "reach.h"
#include "world.h"

namespace nav::placement {
namespace {

// Where the virtual mouse rests while placing: the middle of the 3D view (client coordinates of
// the 2560x1600 window), away from the panels. The pick answers with the cursor's spot anyway.
constexpr int kSceneX = 1280, kSceneY = 640;
// The scan looks this far from the lot's corner for its edges, and at these floors (0 = the
// terrain; a floor with no tile on the lot ends it).
constexpr int kScanTiles = 80;
constexpr int kScanLevels = 4;

struct Spot {
    int x, y, level;
    int room;
    double distance;  // from the selected Sim (same floor first)
    int dirs = 0;     // the directions it fits in: bit d / 2 for direction d (0 west ... 6 north)
    int serves = 0;   // a seat here, turned to serveDir, is how this thing is used (a computer), 0 none
    int serveDir = 0;
};

struct State {
    bool active = false;
    std::string name;
    int x = 0, y = 0, level = 1;  // the cursor's tile (level as game::LotObject::level)
    bool pending = false;         // a spot was sent; say it once the tool has judged it
    bool sayFacing = false;       // ... with the facing (after a turn)
    uint32_t verdictsBefore = 0;
    ULONGLONG sentAt = 0;
    bool placing = false;         // Enter was pressed: see whether the object went down
    ULONGLONG placedAt = 0;
    bool jitter = false;
    // The spots where the held object fits (in its current direction), found by a scan.
    std::vector<Spot> spots;
    bool scanned = false;
    int scanDirection = -1;
    int spotIndex = -1;           // in the current list (all spots, or one room's)
    bool roomMode = false;
    int roomLevel = 0, roomId = -1;
    // Picking up an object of the lot: the click was sent, see whether the tool holds it.
    bool pickingUp = false;
    uint32_t pickGuid = 0;
    ULONGLONG pickAt = 0;
    bool moving = false;          // the held object came from the lot (no copy follows)
    bool selling = false;         // Delete was pressed
    ULONGLONG sellAt = 0;
    bool removing = false;        // Delete on a door or window: picked up only to be sold
    bool door = false;            // Enter put down a door: is it usable? (doorX, doorY, doorDirection)
    int placedId = 0;             // Enter put down this object (not a door): can Sims get to it?
    int doorX = 0, doorY = 0, doorDirection = 0;
    // The room the last line named (said again when it changes: "leaving X, entering Y").
    int lastRoom = -2, lastRoomLevel = 0;
    std::string lastRoomName;
    bool cueAfter = false;        // an arrow moved it: the tile's sounds after the verdict
    bool sizePending = false;     // the first line says how much floor it takes
    std::string prefix;           // said before the next line ("beside the bed")
    // Ctrl+Shift+PageDown/PageUp: the lot's things nearest the cursor first (ids), and the one
    // the held object was put beside last.
    std::vector<int> things;
    int thing = -1;
    // A moved object: where it stood (its spots come nearest there, as its room).
    bool hasOrigin = false;
    int originX = 0, originY = 0, originLevel = 0;
    std::string wallAfter;        // an arrow stopped at a wall after a few steps: said after the line
};
State g;

void Send() {
    game::SetPickOverride(true, g.x + 0.5f, g.y + 0.5f, g.level);
    // The tool picks on mouse moves: move by a pixel so the game sees one.
    g.jitter = !g.jitter;
    game::MouseMove(kSceneX + (g.jitter ? 1 : 0), kSceneY);
    game::PlacementVerdict(&g.verdictsBefore);
    g.pending = true;
    g.sentAt = GetTickCount64();
}

void Stop() {
    game::SetPickOverride(false);
    game::MouseRelease();
    g = State();
}

int Direction() {
    game::HeldObject held;
    return game::ReadHeldObject(game::CurrentTool(), &held) ? held.direction : 0;
}

// The object's direction as the lot's compass: 0 faces -x (west), 2 +y (south), 4 +x (east),
// 6 -y (north) (verified on a dining set: each chair faces the table).
std::string Facing(int direction) {
    static const Str kNames[] = {Str::DirWest, Str::DirSouth, Str::DirEast, Str::DirNorth};
    return util::Format(S(Str::PlaceFacing), S(kNames[(direction / 2) & 3]));
}

// A door or window: which wall of its room it is on and the room behind it. The object faces its
// direction from the tile in front of the wall; behind it is the tile one step back.
std::string WallPart(int x, int y, int level) {
    game::HeldObject held;
    if (!game::ReadHeldObject(game::CurrentTool(), &held) || (held.objectType != 8 && held.objectType != 9)) return "";
    static const int kDx[] = {-1, 0, 1, 0}, kDy[] = {0, 1, 0, -1};  // 0 -x, 2 +y, 4 +x, 6 -y
    static const Str kSides[] = {Str::DirEast, Str::DirNorth, Str::DirWest, Str::DirSouth};  // behind
    int d = (held.direction / 2) & 3;
    float bx = x + 0.5f - kDx[d], by = y + 0.5f - kDy[d];
    std::string behind = world::RoomNameAt(bx, by, level);
    std::string line = util::Format(S(Str::PlaceWallSide), S(kSides[d]));
    int frontId = game::RoomAt(x + 0.5f, y + 0.5f, level), behindId = game::RoomAt(bx, by, level);
    if (behind.empty() || frontId == behindId) return line;
    // Rooms are named from their contents: two rooms can share a name.
    bool sameName = behind == world::RoomNameAt(x + 0.5f, y + 0.5f, level);
    line += ", " + util::Format(S(sameName ? Str::PlaceLeadsToAnother : Str::PlaceLeadsTo), behind.c_str());
    return line;
}

const int kStepX[] = {-1, 0, 1, 0}, kStepY[] = {0, 1, 0, -1};  // direction / 2: 0 -x, 2 +y, 4 +x, 6 -y

// ---- Seats for what is used sitting down (ported from the Sims 1 mod's tester feedback, 2026-11-06:
// a tester could not get a chair to work for the computer on his desk) ----

// Something used sitting down: standing on a desk or a table (a computer on its desk: its +0xaa,
// the desk's OBJD sub-sort) and sold as electronics, appliances or miscellaneous. A microwave on a
// counter is used standing (it was offered a chair before the sub-sort was read), a lamp on a side
// table is lighting.
bool UsedSitting(const game::LotObject& o) {
    using namespace game::addr::objd;
    if (o.person || !(o.functionFlags & (kFnElectronics | kFnAppliances | kFnMiscellaneous))) return false;
    int container = game::ContainerOf(o.id);
    int sort = container ? game::ObjdField(container, kFieldFunctionSubsort) : -1;
    return sort > 0 && (sort & (kSurfaceDesk | kSurfaceTable)) != 0;
}

// A one-tile seat (a chair, a stool): what can stand at a desk.
bool IsSeat(const game::LotObject& o, const std::vector<game::PlacedPart>& parts) {
    return !o.person && (o.functionFlags & game::addr::objd::kFnComfort) && game::ObjectTiles(o.id, parts).size() <= 1;
}

// The direction from one tile toward another beside it (or nearly): 0 -x, 2 +y, 4 +x, 6 -y.
int DirectionToward(int x, int y, int tx, int ty) {
    int dx = tx - x, dy = ty - y;
    if (std::abs(dx) >= std::abs(dy)) return dx > 0 ? 4 : 0;
    return dy > 0 ? 2 : 6;
}

// The things of a floor used sitting down, with the tiles their seats stand on: the container's
// sitting routing slots (its desk's chair spot), facing the thing's own tile.
struct Sitting {
    int id = 0;
    int x = 0, y = 0;  // the thing's tile
    std::vector<std::pair<int, int>> seatTiles;
};
std::vector<Sitting> SittingThings(int level) {
    std::vector<Sitting> out;
    for (const game::LotObject& o : game::LotObjects()) {
        if (o.level != level || o.x < 0 || !UsedSitting(o)) continue;
        Sitting s;
        s.id = o.id;
        s.x = (int)o.x;
        s.y = (int)o.y;
        int container = game::ContainerOf(o.id);
        std::vector<game::UseSpot> spots = game::UseSpots(o.id);
        if (spots.empty()) spots = game::UseSpots(container);
        for (const game::UseSpot& u : spots)
            if (u.sitting) s.seatTiles.push_back({u.tileX, u.tileY});
        // A desk flags no slot as sitting (read live: eight spots around it): the seat stands on
        // its front spot, facing it (the Contreras' office chair, east of a desk facing east).
        game::LotObject c;
        if (s.seatTiles.empty() && game::ReadLotObject(container, &c)) {
            int d = (c.direction / 2) & 3;
            std::pair<int, int> front = {(int)c.x + kStepX[d], (int)c.y + kStepY[d]};
            for (const game::UseSpot& u : spots)
                if (u.tileX == front.first && u.tileY == front.second) s.seatTiles.push_back(front);
        }
        LogVerbose("used sitting: %s (#%d) at %d,%d, %d seat tiles", o.name.c_str(), o.id, s.x, s.y, (int)s.seatTiles.size());
        if (!s.seatTiles.empty()) out.push_back(s);
    }
    return out;
}

// The thing a seat standing on (x, y) facing `direction` serves, 0 none.
int SeatServes(const std::vector<Sitting>& things, int x, int y, int direction) {
    for (const Sitting& t : things)
        for (const auto& st : t.seatTiles)
            if (st.first == x && st.second == y && DirectionToward(x, y, t.x, t.y) == (direction & 6)) return t.id;
    return 0;
}

std::string NameOfId(int id) {
    game::LotObject o;
    return id && game::ReadLotObject(id, &o) ? world::DisplayName(o) : "";
}

// The held object's own id on the lot (its parts carry it as their lead), 0 when unknown.
int HeldId(const game::HeldObject& held) {
    if (!held.object) return 0;
    for (const game::LotObject& o : game::LotObjects())
        if (o.ptr == held.object) return o.id;
    return 0;
}

// The tiles the held object covers where it stands now: its parts' tiles when they are on the
// cursor's tile with it, else the cursor's tile alone.
std::vector<std::pair<int, int>> HeldTiles(int heldId) {
    std::vector<std::pair<int, int>> tiles = heldId ? game::ObjectTiles(heldId, game::PlacedParts())
                                                    : std::vector<std::pair<int, int>>{};
    if (std::find(tiles.begin(), tiles.end(), std::make_pair(g.x, g.y)) == tiles.end()) tiles = {{g.x, g.y}};
    return tiles;
}

// "ocupa 6 casillas: 3 de oeste a este, 2 de norte a sur" (a multi-tile object; a bed "facing
// north" can lie west to east), empty for one tile.
std::string SizeLine(const std::vector<std::pair<int, int>>& tiles) {
    if (tiles.size() < 2) return "";
    int x0 = tiles[0].first, x1 = x0, y0 = tiles[0].second, y1 = y0;
    for (const auto& t : tiles) {
        x0 = std::min(x0, t.first);
        x1 = std::max(x1, t.first);
        y0 = std::min(y0, t.second);
        y1 = std::max(y1, t.second);
    }
    return util::Format(S(Str::ObjectSize), (int)tiles.size(), x1 - x0 + 1, y1 - y0 + 1);
}

// The names of the things standing on these tiles (the held object left out), each once.
std::string NamesOn(const std::vector<std::pair<int, int>>& tiles, int level, int heldId, bool faced = false) {
    std::vector<std::string> names;
    for (const auto& t : tiles)
        for (const game::LotObject& o : world::ThingsAt(t.first, t.second, level)) {
            if (o.id == heldId) continue;
            // What a thing faces stands on the floor: not a ceiling lamp above the tile.
            if (faced && o.name.find("Ceiling") != std::string::npos) continue;
            std::string n = world::DisplayName(o);
            if (!n.empty() && std::find(names.begin(), names.end(), n) == names.end()) names.push_back(n);
        }
    std::string out;
    for (const std::string& n : names) out += (out.empty() ? "" : ", ") + n;
    return out;
}

// The tiles just in front of the held object (what a chair would face), in the same room: a
// window in the wall behind is not what it faces.
std::vector<std::pair<int, int>> FrontTiles(const std::vector<std::pair<int, int>>& tiles, int direction) {
    std::vector<std::pair<int, int>> out;
    int d = (direction / 2) & 3;
    for (const auto& t : tiles) {
        std::pair<int, int> n = {t.first + kStepX[d], t.second + kStepY[d]};
        if (std::find(tiles.begin(), tiles.end(), n) != tiles.end()) continue;
        if (game::RoomAt(n.first + 0.5f, n.second + 0.5f, g.level) != game::RoomAt(t.first + 0.5f, t.second + 0.5f, g.level))
            continue;
        out.push_back(n);
    }
    return out;
}

// What the held object looks at further off when nothing stands just in front (a tester turning a
// television heard nothing of the sofa three tiles away): the first thing along its facing, up
// to 8 tiles, in its room and before a wall. Its names, and in *distance the tiles to it.
std::string AheadNames(const std::vector<std::pair<int, int>>& tiles, int direction, int heldId, int* distance) {
    int d = (direction / 2) & 3;
    std::vector<std::pair<int, int>> row;  // the front edge's tiles, walked forward together
    for (const auto& t : tiles) {
        std::pair<int, int> n = {t.first + kStepX[d], t.second + kStepY[d]};
        if (std::find(tiles.begin(), tiles.end(), n) == tiles.end()) row.push_back(t);
    }
    std::vector<int> room(row.size());
    for (size_t i = 0; i < row.size(); i++) room[i] = game::RoomAt(row[i].first + 0.5f, row[i].second + 0.5f, g.level);
    std::vector<bool> alive(row.size(), true);
    for (int k = 1; k <= 8; k++) {
        std::vector<std::pair<int, int>> step;
        for (size_t i = 0; i < row.size(); i++) {
            if (!alive[i]) continue;
            std::pair<int, int> n = {row[i].first + kStepX[d], row[i].second + kStepY[d]};
            if (cues::WallStops(g.level, row[i].first, row[i].second, kStepX[d], kStepY[d], nullptr) ||
                game::RoomAt(n.first + 0.5f, n.second + 0.5f, g.level) != room[i]) {
                alive[i] = false;
                continue;
            }
            row[i] = n;
            step.push_back(n);
        }
        if (step.empty()) return "";
        std::string names = NamesOn(step, g.level, heldId, true);
        if (!names.empty()) {
            *distance = k;
            return names;
        }
    }
    return "";
}

// "no cabe, choca con Sofá, saliendo de cocina, entrando en salón, mirando al norte, hacia
// Televisor, casilla 12, 30": the verdict first, what it runs into, the room when the cursor has
// just entered it (or always: full), the wall or the facing, what it faces; the tile last (in the
// Sims 1 mod a tester found coordinates first hard to follow).
std::string SpotLine(bool withFacing, bool full = false) {
    game::HeldObject held;
    bool tool = game::ReadHeldObject(game::CurrentTool(), &held);
    std::string tile = util::Format(S(Str::PlaceTile), g.x, g.y);
    if (tool && !held.overLot) return std::string(S(Str::PlaceOffLot)) + ", " + tile;
    int heldId = tool ? HeldId(held) : 0;
    std::vector<std::pair<int, int>> tiles = HeldTiles(heldId);
    std::vector<std::string> parts;
    int verdict = game::PlacementVerdict();
    if (verdict >= 0) parts.push_back(S(verdict == 1 ? Str::PlaceFits : Str::PlaceBlocked));
    if (verdict == 0) {  // which object is in the way (a tester: "it can't overlap another object, but which?")
        std::string in = NamesOn(tiles, g.level, heldId);
        if (!in.empty()) parts.push_back(util::Format(S(Str::CollidesWith), in.c_str()));
    }
    int room = game::RoomAt(g.x + 0.5f, g.y + 0.5f, g.level);
    if (full || room != g.lastRoom || g.level != g.lastRoomLevel) {
        std::string name = world::RoomNameAt(g.x + 0.5f, g.y + 0.5f, g.level);
        if (!full && !g.lastRoomName.empty() && g.lastRoomLevel == g.level && !name.empty() && name != g.lastRoomName)
            parts.push_back(util::Format(S(Str::RoomChange), g.lastRoomName.c_str(), name.c_str()));
        else if (!name.empty())
            parts.push_back(name);
        g.lastRoom = room;
        g.lastRoomLevel = g.level;
        g.lastRoomName = name;
    }
    std::string wall = WallPart(g.x, g.y, g.level);
    if (!wall.empty()) parts.push_back(wall);
    else {
        std::string edges = cues::EdgesLine(cues::Sense(g.level, g.x, g.y, heldId));  // the walls around it
        if (!edges.empty()) parts.push_back(edges);
        parts.push_back(Facing(held.direction));
    }
    if (wall.empty()) {
        std::string faced = NamesOn(FrontTiles(tiles, held.direction), g.level, heldId, true);
        int distance = 0;
        if (faced.empty()) faced = AheadNames(tiles, held.direction, heldId, &distance);
        if (!faced.empty() && distance > 1)
            parts.push_back(util::Format(S(Str::FacingThingAway), faced.c_str(), distance));
        else if (!faced.empty())
            parts.push_back(util::Format(S(Str::FacingThing), faced.c_str()));
    }
    parts.push_back(tile);
    std::string line;
    for (const std::string& p : parts) line += (line.empty() ? "" : ", ") + p;
    return line;
}

void Say(bool withFacing, bool full = false) {
    std::string line = SpotLine(withFacing, full);
    game::HeldObject held;
    bool tool = game::ReadHeldObject(game::CurrentTool(), &held);
    int heldId = tool ? HeldId(held) : 0;
    std::string size = SizeLine(HeldTiles(heldId));
    if (g.sizePending) {
        g.sizePending = false;
        if (!size.empty()) line = size + ". " + line;
    }
    if (!g.prefix.empty()) {
        line = g.prefix + ": " + line;
        g.prefix.clear();
    }
    if (!g.wallAfter.empty()) {
        line += ". " + g.wallAfter;
        g.wallAfter.clear();
    }
    speech::Say(line);
    std::vector<std::string> details = {line, Facing(Direction()), world::WhereFromSim(g.x + 0.5f, g.y + 0.5f, g.level)};
    if (!size.empty()) details.push_back(size);
    buffers::SetDetails(details);
    if (g.cueAfter) {  // an arrow moved it: the tile's sounds, then whether it fits
        g.cueAfter = false;
        cues::Tile(g.level, g.x, g.y, heldId);
        int verdict = game::PlacementVerdict();
        if (verdict >= 0) cues::Fit(verdict == 1);
    }
}

// An arrow: one tile (Shift five); the verdict, then the line and the tile's sounds. A wall, fence
// or window stops it (a door does not), unless Alt is held: in the Sims 1 mod a tester left a
// room by accident.
void MoveBy(int dx, int dy, bool throughWalls = false) {
    int steps = std::max(std::abs(dx), std::abs(dy));
    int sx = dx > 0 ? 1 : dx < 0 ? -1 : 0, sy = dy > 0 ? 1 : dy < 0 ? -1 : 0;
    int moved = 0;
    std::string wall;
    for (int i = 0; i < steps; i++) {
        if (cues::Sense(g.level, g.x + sx, g.y + sy).offLot) break;
        if (!throughWalls && cues::WallStops(g.level, g.x, g.y, sx, sy, &wall)) break;
        g.x += sx;
        g.y += sy;
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
    g.wallAfter = wall;
    g.spotIndex = -1;
    g.cueAfter = true;
    Send();
}

// Every tile of the lot on every floor where the held object fits, in each of its four directions
// (the tool's rotation is turned for each pass and put back). The tool judges each spot at once
// (game::CheckSpot), about 0.1 ms each.
void Scan() {
    game::Ptr tool = game::CurrentTool();
    g.spots.clear();
    g.scanned = true;
    g.scanDirection = Direction();
    g.spotIndex = -1;
    if (!tool) return;
    game::LotObject sim;
    int simId = game::SelectedSimId();
    bool haveSim = simId && game::ReadLotObject(simId, &sim) && sim.x >= 0;
    // A moved object's spots come nearest where it stood (its own room first); a new one's
    // nearest the Sim. In the Sims 1 mod a tester's moved furniture "jumped" to another room.
    if (g.moving && g.hasOrigin) {
        sim = game::LotObject{};
        sim.x = g.originX + 0.5f;
        sim.y = g.originY + 0.5f;
        sim.level = g.originLevel;
        haveSim = true;
    }
    ULONGLONG t0 = GetTickCount64();
    int checked = 0;
    // The lot is a rectangle from the corner: its size from the picks along two edges.
    int width = 0, depth = 0;
    for (int x = 0; x < kScanTiles; x++) {
        checked++;
        if (game::CheckSpot(tool, x + 0.5f, 1.5f, 0) == -2) {
            if (width) break;
        } else {
            width = x + 1;
        }
    }
    for (int y = 0; y < kScanTiles; y++) {
        checked++;
        if (game::CheckSpot(tool, 1.5f, y + 0.5f, 0) == -2) {
            if (depth) break;
        } else {
            depth = y + 1;
        }
    }
    std::map<long long, size_t> seen;  // spot -> index in g.spots
    int original = Direction();
    for (int pass = 0; pass < 4; pass++) {
    int direction = (original + 2 * pass) % 8;
    game::SetHeldDirection(tool, direction);
    for (int level = 0; level < kScanLevels; level++) {
        int onLot = 0;
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < depth; y++) {
                int v = game::CheckSpot(tool, x + 0.5f, y + 0.5f, level);
                checked++;
                if (v == -2) continue;
                onLot++;
                if (v != 1) continue;
                // Where the tool put it (a door snaps to the nearest wall): several tiles can give
                // the same spot, kept once.
                game::HeldObject held;
                int sx = x, sy = y;
                if (game::ReadHeldObject(tool, &held) && held.x >= 0) {
                    sx = (int)std::floor(held.x);
                    sy = (int)std::floor(held.y);
                }
                long long key = ((long long)level << 40) | ((long long)(sx & 0xfffff) << 20) | (sy & 0xfffff);
                auto known = seen.find(key);
                if (known != seen.end()) {
                    g.spots[known->second].dirs |= 1 << (direction / 2);
                    continue;
                }
                seen[key] = g.spots.size();
                Spot s = {sx, sy, level, game::RoomAt(sx + 0.5f, sy + 0.5f, level), 0, 1 << (direction / 2)};
                if (haveSim) {
                    s.distance = std::hypot(sx + 0.5 - sim.x, sy + 0.5 - sim.y);
                    if (level != sim.level) s.distance += 1000.0 * std::abs(level - sim.level);
                } else {
                    s.distance = sx + sy;
                }
                g.spots.push_back(s);
            }
        }
        if (!onLot) break;
    }
    }
    game::SetHeldDirection(tool, original);
    std::stable_sort(g.spots.begin(), g.spots.end(), [](const Spot& a, const Spot& b) { return a.distance < b.distance; });
    // A seat: the spots where it would be how something is used sitting down (the chair for the
    // computer on a desk) come first, turned to face it.
    game::HeldObject held;
    game::LotObject heldObject;
    std::vector<game::PlacedPart> parts = game::PlacedParts();
    if (game::ReadHeldObject(tool, &held) && game::ReadLotObject(HeldId(held), &heldObject) && IsSeat(heldObject, parts)) {
        std::map<int, std::vector<Sitting>> byLevel;
        int serving = 0;
        for (Spot& sp : g.spots) {
            if (!byLevel.count(sp.level)) byLevel[sp.level] = SittingThings(sp.level);
            for (int d = 0; d < 4 && !sp.serves; d++) {
                if (!(sp.dirs & (1 << d))) continue;
                if (int id = SeatServes(byLevel[sp.level], sp.x, sp.y, 2 * d)) {
                    sp.serves = id;
                    sp.serveDir = 2 * d;
                    serving++;
                }
            }
        }
        std::stable_partition(g.spots.begin(), g.spots.end(), [](const Spot& a) { return a.serves != 0; });
        Log("placement scan: a seat, %d spots serve something used sitting", serving);
    }
    Log("placement scan: lot %dx%d, %d spots fit of %d checked, direction %d, %llu ms", width, depth,
        (int)g.spots.size(), checked, g.scanDirection, GetTickCount64() - t0);
    Send();  // the held object back on the cursor
}

void EnsureScan() {
    if (!g.scanned) Scan();
}

// The directions a spot fits in, as the lot's compass ("mirando al norte o al este").
std::string Directions(int dirs) {
    static const Str kNames[] = {Str::DirWest, Str::DirSouth, Str::DirEast, Str::DirNorth};
    std::string names;
    for (int d = 0; d < 4; d++) {
        if (!(dirs & (1 << d))) continue;
        if (!names.empty()) names += ", ";
        names += S(kNames[d]);
    }
    return util::Format(S(Str::PlaceFacing), names.c_str());
}

std::vector<const Spot*> CurrentList() {
    std::vector<const Spot*> out;
    for (const Spot& s : g.spots)
        if (!g.roomMode || (s.level == g.roomLevel && s.room == g.roomId)) out.push_back(&s);
    return out;
}

void GoTo(const Spot& s, int index, int count, const std::string& prefix) {
    g.x = s.x;
    g.y = s.y;
    g.level = s.level;
    // Turned to a direction it fits in there (the current one if it does); a seat toward what it serves.
    int now = Direction();
    if (s.serves) {
        game::SetHeldDirection(game::CurrentTool(), s.serveDir);
    } else if (!(s.dirs & (1 << (now / 2)))) {
        for (int d = 0; d < 4; d++)
            if (s.dirs & (1 << d)) {
                game::SetHeldDirection(game::CurrentTool(), 2 * d);
                break;
            }
    }
    // A room's summary names the room already.
    std::string line = prefix.empty() ? world::RoomNameAt(s.x + 0.5f, s.y + 0.5f, s.level) : "";
    std::string wall = WallPart(s.x, s.y, s.level);
    if (!wall.empty()) line += (line.empty() ? "" : ", ") + wall;
    // The directions it fits in, unless any (doors and windows face their wall); a seat's spot by
    // what it serves ("para usar Ordenador sentado").
    std::string serves = s.serves ? NameOfId(s.serves) : "";
    if (!serves.empty()) line += (line.empty() ? "" : ", ") + Facing(s.serveDir) + ", " + util::Format(S(Str::SeatFor), serves.c_str());
    else if (wall.empty() && s.dirs != 0xf) line += (line.empty() ? "" : ", ") + Directions(s.dirs);
    std::string where = world::WhereFromSim(s.x + 0.5f, s.y + 0.5f, s.level);
    if (!where.empty()) line += (line.empty() ? "" : ", ") + where;
    line += ", " + util::Format(S(Str::ValueOfMax), index + 1, count);
    g.lastRoom = game::RoomAt(s.x + 0.5f, s.y + 0.5f, s.level);  // the spot names its room already
    g.lastRoomLevel = s.level;
    g.lastRoomName = world::RoomNameAt(s.x + 0.5f, s.y + 0.5f, s.level);
    speech::Say(prefix.empty() ? line : prefix + ". " + line);
    buffers::SetDetails({line, util::Format(S(Str::PlaceTile), s.x, s.y), Facing(Direction())});
    Send();
    g.pending = false;  // said already
}

void NextSpot(int delta) {
    EnsureScan();
    std::vector<const Spot*> list = CurrentList();
    if (list.empty()) {
        speech::Say(S(Str::PlaceNoSpot));
        return;
    }
    int n = (int)list.size();
    g.spotIndex = g.spotIndex < 0 ? (delta > 0 ? 0 : n - 1) : (g.spotIndex + delta + n) % n;
    GoTo(*list[g.spotIndex], g.spotIndex, n, "");
}

void NextRoom(int delta) {
    EnsureScan();
    // Rooms that have spots, in the order of their nearest spot (the Sim's own room first).
    std::vector<std::pair<int, int>> rooms;
    for (const Spot& s : g.spots) {
        std::pair<int, int> r = {s.level, s.room};
        if (std::find(rooms.begin(), rooms.end(), r) == rooms.end()) rooms.push_back(r);
    }
    if (rooms.empty()) {
        speech::Say(S(Str::PlaceNoSpot));
        return;
    }
    int at = -1;
    for (int i = 0; i < (int)rooms.size(); i++)
        if (g.roomMode && rooms[i] == std::make_pair(g.roomLevel, g.roomId)) at = i;
    int n = (int)rooms.size();
    int next = at < 0 ? (delta > 0 ? 0 : n - 1) : (at + delta + n) % n;
    g.roomMode = true;
    g.roomLevel = rooms[next].first;
    g.roomId = rooms[next].second;
    std::vector<const Spot*> list = CurrentList();
    const Spot& first = *list[0];
    std::string name = world::RoomNameAt(first.x + 0.5f, first.y + 0.5f, first.level);
    std::string summary = util::Format(S(Str::PlaceRoomSpots), name.c_str(), (int)list.size());
    g.spotIndex = 0;
    GoTo(first, 0, (int)list.size(), summary);
}

// Ctrl+Shift+PageDown/PageUp: the lot's things nearest the cursor first; the held object goes on a
// free tile beside the chosen one, in its room, facing it when it fits that way, else any way it
// fits; when nothing fits there, the first tile beside it, so the line says why (in the Sims 1
// mod a tester's chair at a desk took twenty spots).
void NextThing(int delta) {
    game::Ptr tool = game::CurrentTool();
    game::HeldObject held;
    if (!game::ReadHeldObject(tool, &held) || !held.holding) return;
    int heldId = HeldId(held);
    if (g.things.empty()) {
        std::vector<std::pair<double, int>> byDistance;
        for (const game::LotObject& o : game::LotObjects()) {
            if (o.person || o.id == heldId || o.level != g.level || o.x < 0 || !world::IsListed(o.id)) continue;
            byDistance.push_back({std::hypot(o.x - (g.x + 0.5), o.y - (g.y + 0.5)), o.id});
        }
        std::sort(byDistance.begin(), byDistance.end());
        for (const auto& d : byDistance) g.things.push_back(d.second);
        g.thing = -1;
    }
    if (g.things.empty()) {
        speech::Say(S(Str::NoObjects));
        return;
    }
    int n = (int)g.things.size();
    g.thing = g.thing < 0 ? (delta > 0 ? 0 : n - 1) : ((g.thing + delta) % n + n) % n;
    game::LotObject target;
    if (!game::ReadLotObject(g.things[g.thing], &target)) {
        g.things.clear();
        speech::Say(S(Str::NoObjects));
        return;
    }
    std::vector<std::pair<int, int>> body = game::ObjectTiles(target.id, game::PlacedParts());
    if (body.empty()) body.push_back({(int)target.x, (int)target.y});
    int original = Direction();
    bool found = false;
    int bestX = -1, bestY = -1, bestDir = original;
    for (int pass = 0; pass < 2 && !found; pass++)
        for (const auto& f : body) {
            for (int side = 0; side < 4 && !found; side++) {
                int tx = f.first + kStepX[side], ty = f.second + kStepY[side];
                if (std::find(body.begin(), body.end(), std::make_pair(tx, ty)) != body.end()) continue;
                if (game::RoomAt(tx + 0.5f, ty + 0.5f, g.level) != game::RoomAt(f.first + 0.5f, f.second + 0.5f, g.level))
                    continue;  // not through a wall
                int facing = (2 * side + 4) % 8;  // from that tile toward the thing
                if (bestX < 0) {
                    bestX = tx;
                    bestY = ty;
                    bestDir = facing;
                }
                for (int k = 0; k < (pass == 0 ? 1 : 4) && !found; k++) {
                    int d = (facing + 2 * k) % 8;
                    game::SetHeldDirection(tool, d);
                    if (game::CheckSpot(tool, tx + 0.5f, ty + 0.5f, g.level) != 1) continue;
                    found = true;
                    bestX = tx;
                    bestY = ty;
                    bestDir = d;
                }
            }
            if (found) break;
        }
    if (bestX < 0) {
        game::SetHeldDirection(tool, original);
        speech::Say(util::Format(S(Str::BesideThing), world::DisplayName(target).c_str()) + ": " + S(Str::PlaceBlocked));
        return;
    }
    game::SetHeldDirection(tool, bestDir);
    g.x = bestX;
    g.y = bestY;
    g.spotIndex = -1;
    g.cueAfter = true;
    g.prefix = util::Format(S(Str::BesideThing), world::DisplayName(target).c_str());
    Log("beside %s (%d): tile %d,%d direction %d, %s", world::DisplayName(target).c_str(), target.id, bestX, bestY, bestDir,
        found ? "fits" : "nothing fits");
    Send();
}

}  // namespace

namespace {

// After placing: how it is used sitting down. A seat: what it serves ("para usar Ordenador
// sentado"); a thing used sitting: the seat it is used from ("se usa desde Silla"). Empty otherwise.
std::string UsedHow(int placedId) {
    game::LotObject o;
    if (!game::ReadLotObject(placedId, &o)) return "";
    std::vector<game::PlacedPart> parts = game::PlacedParts();
    std::vector<Sitting> things = SittingThings(o.level);
    if (IsSeat(o, parts)) {
        std::string name = NameOfId(SeatServes(things, (int)o.x, (int)o.y, o.direction));
        return name.empty() ? "" : util::Format(S(Str::SeatFor), name.c_str());
    }
    for (const Sitting& t : things) {
        if (t.id != placedId) continue;
        for (const game::LotObject& seat : game::LotObjects())
            if (seat.level == o.level && seat.x >= 0 && IsSeat(seat, parts) &&
                SeatServes({t}, (int)seat.x, (int)seat.y, seat.direction) == t.id)
                return util::Format(S(Str::UsedFromSeat), world::DisplayName(seat).c_str());
    }
    return "";
}

}  // namespace

bool Active() { return g.active; }

void Move(int dx, int dy, bool throughWalls) {
    if (!g.active || g.pickingUp || g.removing) return;
    MoveBy(dx, dy, throughWalls);
}

void Begin(const std::string& productName) {
    g = State();
    g.active = true;
    g.name = productName;
    // Start where the selected Sim stands (the player knows that place), else the lot's corner.
    game::LotObject sim;
    int id = game::SelectedSimId();
    if (id && game::ReadLotObject(id, &sim) && sim.x >= 0) {
        g.x = (int)std::floor(sim.x);
        g.y = (int)std::floor(sim.y);
        g.level = sim.level;
    } else {
        g.x = g.y = 10;
    }
    g.sizePending = true;
    cues::Reset();
    Log("placing %s from %d,%d floor %d", productName.c_str(), g.x, g.y, g.level);
    speech::Say(util::Format(S(Str::PlaceBegin), productName.c_str()));
    Send();
}

void BeginMove(game::Ptr object, uint32_t guid, const std::string& name, float x, float y, int level) {
    g = State();
    g.name = name;
    g.x = (int)std::floor(x);
    g.y = (int)std::floor(y);
    g.level = level;
    g.hasOrigin = true;
    g.originX = g.x;
    g.originY = g.y;
    g.originLevel = level;
    if (!game::ForceObjectPick(object)) {
        speech::Say(util::Format(S(Str::PlaceCannotPick), name.c_str()));
        return;
    }
    // The hand picks up what is under the cursor on a click: the object pick answers with this
    // object, the ground pick with its tile.
    game::SetPickOverride(true, g.x + 0.5f, g.y + 0.5f, g.level);
    game::MouseClick(kSceneX, kSceneY);
    g.active = true;
    g.pickingUp = true;
    g.pickGuid = guid;
    g.pickAt = GetTickCount64();
    Log("picking up %s (%p) at %d,%d floor %d", name.c_str(), (void*)object, g.x, g.y, g.level);
}

void BeginRemove(game::Ptr object, uint32_t guid, const std::string& name, float x, float y, int level) {
    BeginMove(object, guid, name, x, y, level);
    g.removing = g.pickingUp;
    if (!g.removing) speech::Say(util::Format(S(Str::PortalNotRemoved), name.c_str()));
}

bool OnKey(WPARAM vk, bool shift, bool ctrl) {
    if (!g.active || g.pickingUp || g.removing) return false;
    int step = shift ? 5 : 1;
    switch (vk) {
    case VK_UP: if (ctrl) return false; MoveBy(0, -step); return true;
    case VK_DOWN: if (ctrl) return false; MoveBy(0, step); return true;
    case VK_LEFT: if (ctrl) return false; MoveBy(-step, 0); return true;
    case VK_RIGHT: if (ctrl) return false; MoveBy(step, 0); return true;
    // A spatial tool (NAVIGATION.md): the arrows move in space; its targets (the spots where it
    // fits) on PageDown / PageUp as the lot's things are (Tab too), Ctrl the rooms, Shift turns it.
    case VK_TAB:
        if (ctrl) return false;
        NextSpot(shift ? -1 : +1);
        return true;
    case VK_NEXT: case VK_PRIOR:
        if (ctrl && shift) NextThing(vk == VK_NEXT ? +1 : -1);  // beside the lot's things, nearest first
        else if (ctrl) NextRoom(vk == VK_NEXT ? +1 : -1);
        else if (shift) {  // the game's rotation key, judged again once it has turned (as , and .)
            g.pending = true;
            g.sayFacing = true;
            g.sentAt = GetTickCount64() + 150;
            game::PlacementVerdict(&g.verdictsBefore);
            input::PostGameKey(vk == VK_NEXT ? VK_OEM_PERIOD : VK_OEM_COMMA);
        } else NextSpot(vk == VK_NEXT ? +1 : -1);
        return true;
    case VK_HOME: {
        game::LotObject sim;
        int id = game::SelectedSimId();
        if (!id || !game::ReadLotObject(id, &sim) || sim.x < 0) return true;
        g.x = (int)std::floor(sim.x);
        g.y = (int)std::floor(sim.y);
        g.level = sim.level;
        g.spotIndex = -1;
        g.roomMode = false;
        Send();
        return true;
    }
    case VK_OEM_COMMA: case VK_OEM_PERIOD:
        // The game turns the held object on the key: judge the spot again once it has.
        g.pending = true;
        g.sayFacing = true;
        g.sentAt = GetTickCount64() + 150;
        game::PlacementVerdict(&g.verdictsBefore);
        return false;
    case 'R':
        if (ctrl) return false;
        Say(true, true);
        return true;
    case VK_RETURN: {
        if (ctrl) return false;
        int verdict = game::PlacementVerdict();
        if (verdict != 1) {
            speech::Say(S(Str::PlaceBlocked));
            return true;
        }
        Log("placing at %d,%d floor %d, direction %d", g.x, g.y, g.level, Direction());
        game::HeldObject held;
        g.door = game::ReadHeldObject(game::CurrentTool(), &held) && held.objectType == 8;
        g.doorX = held.x >= 0 ? (int)std::floor(held.x) : g.x;
        g.doorY = held.y >= 0 ? (int)std::floor(held.y) : g.y;
        g.doorDirection = held.direction;
        g.placedId = g.door || held.objectType == 9 ? 0 : HeldId(held);
        game::MouseClick(kSceneX + (g.jitter ? 1 : 0), kSceneY);
        g.placing = true;
        g.placedAt = GetTickCount64();
        return true;
    }
    case VK_DELETE:
        g.selling = true;
        g.sellAt = GetTickCount64();
        return false;  // the game sells the held object
    case VK_ESCAPE:
        Log("placing cancelled");
        Stop();
        speech::Say(S(Str::Cancelled));
        return false;  // the game's Escape drops the held object
    default:
        return false;
    }
}

void Tick() {
    if (game::g_doorCheckRequested) {
        game::g_doorCheckRequested = false;
        for (const game::WallObject& w : game::WallObjects()) {
            if (w.type != 8) continue;
            std::string blocker;
            int check = reach::CheckDoor((int)w.x, (int)w.y, w.direction, w.level, &blocker);
            Log("  door %s at %.1f,%.1f direction %d floor %d: check %d (%s)", w.name.c_str(), w.x, w.y, w.direction, w.level, check, blocker.c_str());
        }
    }
    if (game::g_reachCheckRequested) {
        game::g_reachCheckRequested = false;
        reach::Forget();
        int unreachable = 0, checked = 0;
        ULONGLONG t0 = GetTickCount64();
        for (const game::LotObject& o : game::LotObjects()) {
            if (o.person || !world::IsListed(o.id)) continue;
            checked++;
            int use = reach::CanUse(o.id);
            if (use == 2) unreachable++;
            Log("  reach %s (#%d) at %.1f,%.1f floor %d: %d, %s", world::DisplayName(o).c_str(), o.id, o.x, o.y, o.level, use,
                UsedHow(o.id).c_str());
        }
        Log("reach: %d of %d things no Sim can reach, %llu ms", unreachable, checked, GetTickCount64() - t0);
    }
    if (!g.active) return;
    ULONGLONG now = GetTickCount64();
    if (!world::LotViewActive()) {
        Stop();
        return;
    }
    if (g.pickingUp) {
        if (now - g.pickAt < 400) return;
        game::ClearObjectPick();
        g.pickingUp = false;
        game::HeldObject held;
        bool holding = game::ReadHeldObject(game::CurrentTool(), &held) && held.holding && held.guid == g.pickGuid;
        Log("pick up: tool %p holding guid %08x", (void*)game::CurrentTool(), held.guid);
        if (!holding) {
            std::string name = g.name;
            bool removing = g.removing;
            Stop();
            speech::Say(util::Format(S(removing ? Str::PortalNotRemoved : Str::PlaceCannotPick), name.c_str()));
            return;
        }
        if (g.removing) {  // the game sells what the hand holds on its own Delete, and refunds it
            input::PostGameKey(VK_DELETE);
            g.selling = true;
            g.sellAt = now;
            return;
        }
        g.moving = true;
        g.sizePending = true;
        cues::Reset();
        speech::Say(util::Format(S(Str::PlaceMoving), g.name.c_str()));
        Send();
        return;
    }
    if (g.selling) {
        if (now - g.sellAt < 400) return;
        g.selling = false;
        game::HeldObject held;
        bool still = game::ReadHeldObject(game::CurrentTool(), &held) && held.holding;
        if (g.removing) {
            std::string name = g.name;
            Stop();
            if (still) input::PostGameKey(VK_ESCAPE);  // put it back
            if (still) speech::Say(util::Format(S(Str::PortalNotRemoved), name.c_str()));
            else buffers::Event(util::Format(S(Str::PortalRemoved), name.c_str()));
            return;
        }
        if (!still) {
            buffers::Event(util::Format(S(Str::PlaceSold), g.name.c_str()));
            Stop();
        }
        return;
    }
    if (g.placing) {
        if (now - g.placedAt < 500) return;
        // The game hands over another copy right away: placing goes on until Escape.
        game::HeldObject held;
        bool holding = game::ReadHeldObject(game::CurrentTool(), &held) && held.holding;
        Log("after placing: tool %p holding %d", (void*)game::CurrentTool(), holding ? 1 : 0);
        std::string line = util::Format(S(Str::PlacePlaced), g.name.c_str());
        if (g.door) {
            std::string blocker;
            int check = reach::CheckDoor(g.doorX, g.doorY, g.doorDirection, g.level, &blocker);
            Log("door at %d,%d direction %d floor %d: check %d (%s)", g.doorX, g.doorY, g.doorDirection, g.level, check, blocker.c_str());
            line += ". " + (check == 1 ? util::Format(S(Str::DoorBlocked), blocker.c_str())
                                       : std::string(S(check == 2 ? Str::DoorNoWay : Str::DoorUsable)));
            g.door = false;
        } else if (g.placedId) {
            // How it can be used, as the door check says for doors: in the Sims 1 mod a tester could
            // not tell whether his Sim would reach what he had placed.
            reach::Forget();
            std::string with = UsedHow(g.placedId);
            int use = reach::CanUse(g.placedId);
            Log("placed object %d: reach %d, %s", g.placedId, use, with.c_str());
            if (!with.empty()) line += ". " + with;
            else if (use == 2) line += ". " + std::string(S(Str::NobodyReaches));
            else if (use == 1) line += ". " + std::string(S(Str::SimsCanUse));
            g.placedId = 0;
        }
        buffers::Event(line);
        g.placing = false;
        g.scanned = false;  // that spot is taken now
        g.things.clear();   // and the things beside it changed
        if (!holding) Stop();
        else Send();
        return;
    }
    if (g.pending && now > g.sentAt) {
        uint32_t n = 0;
        game::PlacementVerdict(&n);
        if (n != g.verdictsBefore || now - g.sentAt > 400) {
            g.pending = false;
            Say(g.sayFacing);
            g.sayFacing = false;
        }
    }
}

}  // namespace nav::placement
