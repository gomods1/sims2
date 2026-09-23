#include "placement.h"

#include <algorithm>
#include <cmath>
#include <vector>

#include "../game/buy.h"
#include "../game/mouse.h"
#include "../game/objects.h"
#include "../speech/speech.h"
#include "../util/log.h"
#include "../util/strings.h"
#include "../util/text.h"
#include "buffers.h"
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

std::string SpotLine(bool withFacing) {
    game::HeldObject held;
    bool tool = game::ReadHeldObject(game::CurrentTool(), &held);
    std::string line = util::Format(S(Str::PlaceTile), g.x, g.y);
    if (tool && !held.overLot) return line + ", " + S(Str::PlaceOffLot);
    std::string room = world::RoomNameAt(g.x + 0.5f, g.y + 0.5f, g.level);
    if (!room.empty()) line += ", " + room;
    if (withFacing) line += ", " + Facing(held.direction);
    int verdict = game::PlacementVerdict();
    if (verdict >= 0) line += ", " + std::string(S(verdict == 1 ? Str::PlaceFits : Str::PlaceBlocked));
    return line;
}

void Say(bool withFacing) {
    std::string line = SpotLine(withFacing);
    speech::Say(line);
    buffers::SetDetails({line, Facing(Direction()), world::WhereFromSim(g.x + 0.5f, g.y + 0.5f, g.level)});
}

// Every tile of the lot on every floor where the held object fits as it is turned now. The tool
// judges each spot at once (game::CheckSpot), about 0.1 ms each.
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
    for (int level = 0; level < kScanLevels; level++) {
        int onLot = 0;
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < depth; y++) {
                int v = game::CheckSpot(tool, x + 0.5f, y + 0.5f, level);
                checked++;
                if (v == -2) continue;
                onLot++;
                if (v != 1) continue;
                Spot s = {x, y, level, game::RoomAt(x + 0.5f, y + 0.5f, level), 0};
                if (haveSim) {
                    s.distance = std::hypot(x + 0.5 - sim.x, y + 0.5 - sim.y);
                    if (level != sim.level) s.distance += 1000.0 * std::abs(level - sim.level);
                } else {
                    s.distance = x + y;
                }
                g.spots.push_back(s);
            }
        }
        if (!onLot) break;
    }
    std::stable_sort(g.spots.begin(), g.spots.end(), [](const Spot& a, const Spot& b) { return a.distance < b.distance; });
    Log("placement scan: lot %dx%d, %d spots fit of %d checked, direction %d, %llu ms", width, depth,
        (int)g.spots.size(), checked, g.scanDirection, GetTickCount64() - t0);
    Send();  // the held object back on the cursor
}

void EnsureScan() {
    if (!g.scanned || g.scanDirection != Direction()) Scan();
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
    // A room's summary names the room already.
    std::string line = prefix.empty() ? world::RoomNameAt(s.x + 0.5f, s.y + 0.5f, s.level) : "";
    std::string where = world::WhereFromSim(s.x + 0.5f, s.y + 0.5f, s.level);
    if (!where.empty()) line += (line.empty() ? "" : ", ") + where;
    line += ", " + util::Format(S(Str::ValueOfMax), index + 1, count);
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

}  // namespace

bool Active() { return g.active; }

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
    Log("placing %s from %d,%d floor %d", productName.c_str(), g.x, g.y, g.level);
    speech::Say(util::Format(S(Str::PlaceBegin), productName.c_str()));
    Send();
}

bool OnKey(WPARAM vk, bool shift, bool ctrl) {
    if (!g.active) return false;
    int step = shift ? 5 : 1;
    switch (vk) {
    case VK_UP: if (ctrl) return false; g.y -= step; g.spotIndex = -1; Send(); return true;
    case VK_DOWN: if (ctrl) return false; g.y += step; g.spotIndex = -1; Send(); return true;
    case VK_LEFT: if (ctrl) return false; g.x -= step; g.spotIndex = -1; Send(); return true;
    case VK_RIGHT: if (ctrl) return false; g.x += step; g.spotIndex = -1; Send(); return true;
    case VK_NEXT: case VK_PRIOR:
        if (ctrl) NextRoom(vk == VK_NEXT ? +1 : -1);
        else NextSpot(vk == VK_NEXT ? +1 : -1);
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
        Say(true);
        return true;
    case VK_RETURN: {
        if (ctrl) return false;
        int verdict = game::PlacementVerdict();
        if (verdict != 1) {
            speech::Say(S(Str::PlaceBlocked));
            return true;
        }
        Log("placing at %d,%d floor %d, direction %d", g.x, g.y, g.level, Direction());
        game::MouseClick(kSceneX + (g.jitter ? 1 : 0), kSceneY);
        g.placing = true;
        g.placedAt = GetTickCount64();
        return true;
    }
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
    if (!g.active) return;
    ULONGLONG now = GetTickCount64();
    if (!world::LotViewActive()) {
        Stop();
        return;
    }
    if (g.placing) {
        if (now - g.placedAt < 500) return;
        // The game hands over another copy right away: placing goes on until Escape.
        game::HeldObject held;
        bool holding = game::ReadHeldObject(game::CurrentTool(), &held) && held.guid;
        Log("after placing: tool %p holding %d", (void*)game::CurrentTool(), holding ? 1 : 0);
        buffers::Event(util::Format(S(Str::PlacePlaced), g.name.c_str()));
        g.placing = false;
        g.scanned = false;  // that spot is taken now
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
