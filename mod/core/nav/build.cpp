#include "build.h"

#include <algorithm>
#include <cmath>

#include "../game/buy.h"
#include "../game/cheats.h"
#include "../game/hooks.h"
#include "../game/lotgrid.h"
#include "../game/objects.h"
#include "../game/rtti.h"
#include "../speech/speech.h"
#include "../util/log.h"
#include "../util/strings.h"
#include "../util/text.h"
#include "buffers.h"
#include "cues.h"
#include "live.h"
#include "world.h"

namespace nav::build {
namespace {

struct State {
    bool active = false;
    std::string name;
    int x = 0, y = 0, level = 1;  // the cursor's corner (level as game::LotObject::level)
    bool marked = false;
    int markX = 0, markY = 0;
    ULONGLONG begunAt = 0;
    int dragStep = 0;             // a drag in progress, one mouse message per tick (0 none)
    bool dragCtrl = false;
    bool waiting = false;         // a drag was made: say what it cost once the funds show it
    bool removing = false;
    std::string fundsBefore;
    ULONGLONG draggedAt = 0;
    int lastRoom = -2, lastRoomLevel = 0;  // the room the cursor's line named last
    std::string lastRoomName;
};
State g;

// The dragged tools: walls, fences and rooms, foundations and decks go from corner to corner;
// floors and pools cover tiles.
enum class Kind { None, Corners, Tiles };
Kind KindOf(game::Ptr tool) {
    if (game::IsKindOf(tool, "cTSUserToolWall") || game::IsKindOf(tool, "cTSUserToolDeckFoundation"))
        return Kind::Corners;
    if (game::IsKindOf(tool, "cTSUserToolFloorPattern") || game::IsKindOf(tool, "cTSUserToolCurvedPool"))
        return Kind::Tiles;
    return Kind::None;
}
bool IsDragTool(game::Ptr tool) { return KindOf(tool) != Kind::None; }

// The funds as a number, from the control panel's text ("49.129 simoleones"); -1 when unknown.
long long FundsValue(const std::string& text) {
    long long v = 0;
    bool any = false;
    for (char c : text)
        if (c >= '0' && c <= '9') {
            v = v * 10 + (c - '0');
            any = true;
        }
    return any ? v : -1;
}

// What is at the cursor, in the order a builder needs it (in the Sims 1 mod a tester found
// coordinates first hard to follow): what stands on the tile south-east of the corner, the room
// when the cursor has just entered it ("leaving X, entering Y"; always with `full`), the walls,
// doors and windows; the coordinates last. Then the corner's sounds (nav/cues.h).
void SayCorner(bool full = false) {
    bool tiles = KindOf(game::CurrentTool()) == Kind::Tiles;
    std::vector<std::string> parts;
    for (const game::LotObject& o : world::ThingsAt(g.x, g.y, g.level)) parts.push_back(world::DisplayName(o));
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
    cues::TileSense t = cues::Sense(g.level, g.x, g.y);
    if (tiles) parts.push_back(cues::SurfaceWord(t.surface));  // what the floor or pool tool would cover
    std::string edges = tiles ? cues::EdgesLine(t) : cues::CornerEdgesLine(g.level, g.x, g.y);
    if (!edges.empty()) parts.push_back(edges);
    if (t.sloped) parts.push_back(S(Str::TerrainSloped));
    std::string line;
    for (const std::string& p : parts) line += (line.empty() ? "" : ", ") + p;
    line += (line.empty() ? "" : ". ") + util::Format(S(tiles ? Str::PlaceTile : Str::BuildCorner), g.x, g.y);
    speech::Say(line);
    std::vector<std::string> details = {line};
    if (g.marked) details.push_back(util::Format(S(Str::BuildMarked), g.markX, g.markY));
    buffers::SetDetails(details);
    if (tiles) cues::Tile(g.level, g.x, g.y);
    else cues::Corner(g.level, g.x, g.y);
}

// An arrow: one corner (Shift five), not past the lot's edge.
void MoveBy(int dx, int dy) {
    int w = 80, h = 80;
    game::LotSize(&w, &h);
    int x = std::clamp(g.x + dx, 0, w), y = std::clamp(g.y + dy, 0, h);  // corners: one more than tiles
    if (x == g.x && y == g.y) {
        speech::Say(S(Str::MapLotEdge));
        cues::LotEdge();
        return;
    }
    g.x = x;
    g.y = y;
    SayCorner();
}

void Drag(bool remove) {
    if (!g.marked) {
        speech::Say(S(Str::BuildNoMark));
        return;
    }
    game::Ptr tool = game::CurrentTool();
    if (!IsDragTool(tool)) {
        Log("build: the current tool is %s", game::ClassName(tool).c_str());
        g = State();
        return;
    }
    g.fundsBefore = live::FundsText();
    g.removing = remove;
    g.dragCtrl = remove;
    g.dragStep = 1;  // the ticks send the mouse messages, a frame apart as a real drag's are
    Log("build: drag from %d,%d to %d,%d floor %d%s", g.markX, g.markY, g.x, g.y, g.level, remove ? " (remove)" : "");
}

// After a wall is built: a Sim of the family indoors in a room with no door is shut in (the
// game lets it happen; Ctrl+Z undoes the wall).
void CheckShutIn() {
    std::vector<game::WallObject> walls = game::WallObjects();
    static const int kDx[] = {-1, 0, 1, 0}, kDy[] = {0, 1, 0, -1};  // 0 -x, 2 +y, 4 +x, 6 -y
    for (int id : game::FamilySimIds()) {
        game::LotObject sim;
        if (!game::ReadLotObject(id, &sim) || sim.x < 0) continue;
        int room = game::RoomAt(sim.x, sim.y, sim.level);
        if (room < 0) continue;
        std::string outside = world::RoomNameAt(sim.x, sim.y, sim.level);
        bool door = false;
        for (const game::WallObject& w : walls) {
            if (w.type != 8 || w.level != sim.level) continue;
            int d = (w.direction / 2) & 3;
            if (game::RoomAt(w.x, w.y, w.level) == room || game::RoomAt(w.x - kDx[d], w.y - kDy[d], w.level) == room) door = true;
        }
        LogVerbose("shut-in check: sim %d in room %d (%s), door %d", id, room, outside.c_str(), door ? 1 : 0);
        if (door) continue;
        // A room open to the outside (no walls all round) is the lot itself: RoomAt gives the
        // outdoors a room of its own, which never has doors of its own either; skip rooms
        // bigger than a house could be.
        int tiles = 0;
        for (int tx = 0; tx < 80 && tiles < 400; tx++)
            for (int ty = 0; ty < 80; ty++)
                if (game::RoomAt(tx + 0.5f, ty + 0.5f, sim.level) == room) tiles++;
        if (tiles >= 400) continue;
        std::string first, last;
        game::SimName(id, &first, &last);
        buffers::Event(util::Format(S(Str::BuildShutIn), first.c_str(), outside.c_str()));
    }
}

}  // namespace

bool Active() { return g.active; }

void CheckShutInNow() { CheckShutIn(); }

bool CursorTile(int* x, int* y, int* level) {
    if (!g.active) return false;
    *x = g.x;
    *y = g.y;
    *level = g.level;
    return true;
}

void Begin(const std::string& toolName) {
    g = State();
    g.active = true;
    g.name = toolName;
    g.begunAt = GetTickCount64();
    game::LotObject sim;
    int id = game::SelectedSimId();
    if (id && game::ReadLotObject(id, &sim) && sim.x >= 0) {
        g.x = (int)std::lround(sim.x);
        g.y = (int)std::lround(sim.y);
        g.level = sim.level;
    } else {
        g.x = g.y = 10;
    }
    Log("build cursor for %s from %d,%d floor %d", toolName.c_str(), g.x, g.y, g.level);
    speech::Say(util::Format(S(Str::BuildBegin), toolName.c_str()));
    cues::Reset();
}

bool OnKey(WPARAM vk, bool shift, bool ctrl) {
    if (!g.active) return false;
    int step = shift ? 5 : 1;
    switch (vk) {
    case VK_UP: if (ctrl) return false; MoveBy(0, -step); return true;
    case VK_DOWN: if (ctrl) return false; MoveBy(0, step); return true;
    case VK_LEFT: if (ctrl) return false; MoveBy(-step, 0); return true;
    case VK_RIGHT: if (ctrl) return false; MoveBy(step, 0); return true;
    case VK_PRIOR: case VK_NEXT:
        // Level 0 is the terrain, 1 the floor of a house on a foundation (or the floor above).
        g.level = std::max(0, g.level + (vk == VK_PRIOR ? 1 : -1));
        speech::Say(util::Format(S(Str::FloorNumber), g.level));
        SayCorner(true);
        return true;
    case VK_SPACE:
        g.marked = true;
        g.markX = g.x;
        g.markY = g.y;
        speech::Say(util::Format(S(Str::BuildMarked), g.x, g.y));
        return true;
    case VK_RETURN:
        Drag(ctrl);
        return true;
    case VK_HOME: {
        game::LotObject sim;
        int id = game::SelectedSimId();
        if (id && game::ReadLotObject(id, &sim) && sim.x >= 0) {
            g.x = (int)std::lround(sim.x);
            g.y = (int)std::lround(sim.y);
            g.level = sim.level;
        }
        SayCorner();
        return true;
    }
    case 'R':
        if (ctrl) return false;
        SayCorner(true);
        return true;
    case VK_ESCAPE:
        g = State();
        speech::Say(S(Str::Cancelled));
        return false;
    default:
        return false;
    }
}

void Tick() {
    if (game::g_shutInCheckRequested) {
        game::g_shutInCheckRequested = false;
        CheckShutIn();
    }
    if (!g.active) return;
    if (!world::LotViewActive()) {
        g = State();
        return;
    }
    // The tool changed (another style, another build tool, buy mode): the cursor is done.
    game::Ptr tool = game::CurrentTool();
    if (GetTickCount64() - g.begunAt > 1000 && !IsDragTool(tool)) {
        Log("build cursor ends: tool %s", game::ClassName(tool).c_str());
        g = State();
        return;
    }
    if (g.dragStep) {
        // 1: move to the start, 2: press, 3-4: move to the end, 5: release.
        int step = g.dragStep++;
        bool atEnd = step >= 3;
        float centre = KindOf(tool) == Kind::Tiles ? 0.5f : 0.0f;  // tiles by their centre, corners exactly
        game::SetPickOverride(true, (atEnd ? g.x : g.markX) + centre, (atEnd ? g.y : g.markY) + centre, g.level);
        if (g.dragCtrl) game::SetSyntheticModifier(VK_CONTROL, true);
        int message = step == 2 ? 4 : step == 5 ? 5 : 3;
        game::ToolMessage(tool, step == 1 ? 0 : 1, message, 1280 + step, 640);
        if (step == 5) {
            if (g.dragCtrl) game::SetSyntheticModifier(VK_CONTROL, false);
            game::SetPickOverride(false);
            g.dragStep = 0;
            g.waiting = true;
            g.draggedAt = GetTickCount64();
            // The next wall usually starts where this one ended.
            g.markX = g.x;
            g.markY = g.y;
        }
        return;
    }
    if (g.waiting && GetTickCount64() - g.draggedAt > 400) {
        g.waiting = false;
        std::string funds = live::FundsText();
        long long before = FundsValue(g.fundsBefore), after = FundsValue(funds);
        if (funds == g.fundsBefore || before < 0 || after < 0) {
            buffers::Event(S(Str::BuildNothing));
        } else {
            // What it cost (or what came back), then the funds left.
            std::string change = util::Money((int)std::llabs(after - before));
            buffers::Event(util::Format(S(after < before ? Str::BuildDone : Str::BuildRemoved), change.c_str(), funds.c_str()));
            if (KindOf(game::CurrentTool()) == Kind::Corners) CheckShutIn();
        }
    }
}

}  // namespace nav::build
