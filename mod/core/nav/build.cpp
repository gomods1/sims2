#include "build.h"

#include <algorithm>
#include <cmath>

#include "../game/buy.h"
#include "../game/hooks.h"
#include "../game/objects.h"
#include "../game/rtti.h"
#include "../speech/speech.h"
#include "../util/log.h"
#include "../util/strings.h"
#include "../util/text.h"
#include "buffers.h"
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
};
State g;

// The dragged tools: walls and fences (cTSUserToolWall); others come later (floors, terrain).
bool IsDragTool(game::Ptr tool) { return game::IsKindOf(tool, "cTSUserToolWall"); }

void SayCorner() {
    std::string line = util::Format(S(Str::BuildCorner), g.x, g.y);
    std::string room = world::RoomNameAt(g.x + 0.5f, g.y + 0.5f, g.level);
    if (!room.empty()) line += ", " + room;
    speech::Say(line);
    std::vector<std::string> details = {line};
    if (g.marked) details.push_back(util::Format(S(Str::BuildMarked), g.markX, g.markY));
    buffers::SetDetails(details);
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

}  // namespace

bool Active() { return g.active; }

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
}

bool OnKey(WPARAM vk, bool shift, bool ctrl) {
    if (!g.active) return false;
    int step = shift ? 5 : 1;
    switch (vk) {
    case VK_UP: if (ctrl) return false; g.y -= step; SayCorner(); return true;
    case VK_DOWN: if (ctrl) return false; g.y += step; SayCorner(); return true;
    case VK_LEFT: if (ctrl) return false; g.x -= step; SayCorner(); return true;
    case VK_RIGHT: if (ctrl) return false; g.x += step; SayCorner(); return true;
    case VK_PRIOR: case VK_NEXT:
        // Level 0 is the terrain, 1 the floor of a house on a foundation (or the floor above).
        g.level = std::max(0, g.level + (vk == VK_PRIOR ? 1 : -1));
        speech::Say(util::Format(S(Str::FloorNumber), g.level));
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
        SayCorner();
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
        game::SetPickOverride(true, (float)(atEnd ? g.x : g.markX), (float)(atEnd ? g.y : g.markY), g.level);
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
        if (funds == g.fundsBefore) buffers::Event(S(Str::BuildNothing));
        else buffers::Event(util::Format(S(g.removing ? Str::BuildRemoved : Str::BuildDone), funds.c_str()));
    }
}

}  // namespace nav::build
