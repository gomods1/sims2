#include "reach.h"

#include <windows.h>

#include <algorithm>
#include <map>
#include <set>

#include "../game/lotgrid.h"
#include "../game/objects.h"
#include "../util/log.h"
#include "world.h"

namespace nav::reach {
namespace {

const int kStepX[] = {-1, 0, 1, 0}, kStepY[] = {0, 1, 0, -1};  // direction / 2: 0 -x, 2 +y, 4 +x, 6 -y

// One floor of the lot as a Sim walks it.
struct Floor {
    int w = 0, h = 0, level = 0;
    std::vector<int> blocked;                 // the whole object in the way, by its id (0 free)
    std::vector<char> stairs;                 // stairs on this floor or coming up from below
    std::set<std::pair<int, int>> doorways;   // a door's two sides, by their cells, the lower first
    std::vector<char> reached;                // the flood from the entrance

    bool Inside(int x, int y) const { return x >= 0 && y >= 0 && x < w && y < h; }
    int At(int x, int y) const { return x * h + y; }
    bool Standable(int x, int y) const {
        return Inside(x, y) && !blocked[At(x, y)] && (level == 0 || game::FloorPatternAt(level, x, y) != 0);
    }
    // A wall, fence or railing between two neighbouring tiles that is not a door's opening.
    bool WallBetween(int x0, int y0, int x1, int y1) const {
        return game::WallBetween(level, x0, y0, x1, y1) && !doorways.count(std::minmax(At(x0, y0), At(x1, y1)));
    }
    // The lot's entrance: its edge on the ground, beside the stairs upstairs.
    bool IsEntrance(int x, int y) const {
        if (level == 0) return x == 0 || y == 0 || x == w - 1 || y == h - 1;
        for (int ax = x - 1; ax <= x + 1; ax++)
            for (int ay = y - 1; ay <= y + 1; ay++)
                if (Inside(ax, ay) && stairs[At(ax, ay)]) return true;
        return false;
    }
};

Floor Read(int level, const std::vector<game::PlacedPart>& parts) {
    Floor f;
    f.level = level;
    f.w = f.h = 80;
    game::LotSize(&f.w, &f.h);
    if (f.w < 1 || f.h < 1 || f.w > 256 || f.h > 256) f.w = f.h = 0;
    f.blocked.assign(f.w * f.h, 0);
    f.stairs.assign(f.w * f.h, 0);
    for (const game::PlacedPart& p : parts) {
        if (!f.Inside(p.tileX, p.tileY)) continue;
        if (p.stairs && (p.level == level || p.level == level - 1)) f.stairs[f.At(p.tileX, p.tileY)] = 1;
        if (p.level != level || p.person) continue;
        if (p.type == 8) {
            int pd = (p.direction / 2) & 3;
            int bx = p.tileX - kStepX[pd], by = p.tileY - kStepY[pd];
            if (f.Inside(bx, by)) f.doorways.insert(std::minmax(f.At(p.tileX, p.tileY), f.At(bx, by)));
        }
        if (!p.walkThrough && !p.stairs) f.blocked[f.At(p.tileX, p.tileY)] = p.lead ? p.lead : p.id;
    }
    return f;
}

// Every tile a Sim can get to from the entrance.
void Flood(Floor& f) {
    f.reached.assign(f.w * f.h, 0);
    std::vector<std::pair<int, int>> queue;
    for (int x = 0; x < f.w; x++)
        for (int y = 0; y < f.h; y++)
            if (f.IsEntrance(x, y) && f.Standable(x, y)) {
                f.reached[f.At(x, y)] = 1;
                queue.push_back({x, y});
            }
    for (size_t i = 0; i < queue.size(); i++) {
        int tx = queue[i].first, ty = queue[i].second;
        for (int k = 0; k < 4; k++) {
            int nx = tx + kStepX[k], ny = ty + kStepY[k];
            if (!f.Inside(nx, ny) || f.reached[f.At(nx, ny)] || !f.Standable(nx, ny)) continue;
            if (f.WallBetween(tx, ty, nx, ny)) continue;
            f.reached[f.At(nx, ny)] = 1;
            queue.push_back({nx, ny});
        }
    }
}

// The floors read and flooded, kept a few seconds (the lists describe every thing of the lot).
struct Cache {
    ULONGLONG at = 0;
    game::Ptr world = 0;
    std::map<int, Floor> floors;
    std::vector<game::PlacedPart> parts;
    std::map<int, int> answers;  // object id -> CanUse's answer
} g_cache;
constexpr ULONGLONG kCacheMs = 3000;

Floor& FloorOf(int level) {
    ULONGLONG now = GetTickCount64();
    if (!g_cache.at || now - g_cache.at > kCacheMs || g_cache.world != game::WorldDB()) {
        g_cache.floors.clear();
        g_cache.answers.clear();
        g_cache.parts = game::PlacedParts();
        g_cache.world = game::WorldDB();
        g_cache.at = now;
    }
    auto it = g_cache.floors.find(level);
    if (it != g_cache.floors.end()) return it->second;
    ULONGLONG t0 = GetTickCount64();
    Floor f = Read(level, g_cache.parts);
    Flood(f);
    LogVerbose("reach: floor %d, %dx%d, flooded in %llu ms", level, f.w, f.h, GetTickCount64() - t0);
    return g_cache.floors.emplace(level, std::move(f)).first->second;
}

// The tiles a Sim would use an object from: its routing slots (a computer: its desk's), else each
// tile beside it, not through a wall.
std::vector<std::pair<int, int>> UseTiles(const Floor& f, int objectId, const std::vector<std::pair<int, int>>& body) {
    std::vector<std::pair<int, int>> out;
    int container = game::ContainerOf(objectId);
    std::vector<game::UseSpot> spots = game::UseSpots(objectId);
    if (spots.empty() && container) spots = game::UseSpots(container);
    for (const game::UseSpot& u : spots)
        if (f.Inside(u.tileX, u.tileY) && std::find(out.begin(), out.end(), std::make_pair(u.tileX, u.tileY)) == out.end())
            out.push_back({u.tileX, u.tileY});
    if (!out.empty()) return out;
    for (const auto& t : body)
        for (int k = 0; k < 4; k++) {
            std::pair<int, int> n = {t.first + kStepX[k], t.second + kStepY[k]};
            if (!f.Inside(n.first, n.second) || std::find(body.begin(), body.end(), n) != body.end()) continue;
            if (std::find(out.begin(), out.end(), n) != out.end()) continue;
            if (f.Inside(t.first, t.second) && f.WallBetween(t.first, t.second, n.first, n.second)) continue;
            out.push_back(n);
        }
    return out;
}

int CanUseOn(Floor& f, int objectId) {
    game::LotObject o;
    if (!game::ReadLotObject(objectId, &o) || o.person || o.x < 0 || o.type == 8 || o.type == 9 || f.w == 0) return 0;
    std::vector<std::pair<int, int>> body = game::ObjectTiles(objectId, g_cache.parts);
    if (body.empty()) body.push_back({(int)o.x, (int)o.y});
    bool open = false;
    for (const auto& u : UseTiles(f, objectId, body)) {
        if (f.reached[f.At(u.first, u.second)]) return 1;
        // A tile beside it taken by furniture is how it is used (a chair at a desk): only open
        // floor nobody can get to makes it unusable.
        open |= f.Standable(u.first, u.second);
    }
    return open ? 2 : 0;
}

}  // namespace

int CanUse(int objectId) {
    game::LotObject o;
    if (!game::ReadLotObject(objectId, &o)) return 0;
    return CanUseOn(FloorOf(o.level), objectId);
}

bool Unreachable(int objectId) {
    game::LotObject o;
    if (!game::ReadLotObject(objectId, &o) || o.person) return false;
    Floor& f = FloorOf(o.level);  // refreshes the cache when it is old
    auto known = g_cache.answers.find(objectId);
    if (known != g_cache.answers.end()) return known->second == 2;
    int answer = CanUseOn(f, objectId);
    g_cache.answers[objectId] = answer;
    return answer == 2;
}

void Forget() { g_cache.at = 0; }

int CheckDoor(int x, int y, int direction, int level, std::string* blocker) {
    Forget();
    Floor& f = FloorOf(level);
    if (f.w == 0) return 0;
    int d = (direction / 2) & 3;
    int bx = x - kStepX[d], by = y - kStepY[d];
    for (auto side : {std::make_pair(x, y), std::make_pair(bx, by)}) {
        if (!f.Inside(side.first, side.second) || f.Standable(side.first, side.second)) continue;
        int id = f.blocked[f.At(side.first, side.second)];
        game::LotObject o;
        if (blocker) *blocker = id && game::ReadLotObject(id, &o) ? world::DisplayName(o) : "";
        return id ? 1 : 2;  // no floor on that side: no way through it either
    }
    // From the door outward: the flood from the entrance reaches its front.
    return f.Inside(x, y) && f.reached[f.At(x, y)] ? 0 : 2;
}

}  // namespace nav::reach
