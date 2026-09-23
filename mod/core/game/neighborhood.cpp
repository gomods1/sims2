#include "neighborhood.h"

#include "../util/log.h"
#include "addresses.h"
#include "buy.h"
#include "hooks.h"
#include "mouse.h"
#include "rtti.h"
#include "gz.h"
#include "rtti.h"

namespace game {
namespace {

Ptr FindWindowById(Ptr w, uint32_t id, int depth) {
    if (!w || depth > 30) return 0;
    if (WinId(w) == id) return w;
    for (Ptr c : Children(w))
        if (Ptr found = FindWindowById(c, id, depth + 1)) return found;
    return 0;
}

Ptr Globals() { return Rd<Ptr>(Va(addr::globals::kInstance)); }

void WalkMap(Ptr node, Ptr head, std::vector<std::pair<int, Ptr>>& out, int depth) {
    if (!node || node == head || depth > 64 || out.size() > 5000) return;
    if (Rd<uint8_t>(node + 0xd)) return;  // the nil node
    WalkMap(Rd<Ptr>(node), head, out, depth + 1);
    out.push_back({Rd<int>(node + 0x10), Rd<Ptr>(node + 0x14)});
    WalkMap(Rd<Ptr>(node + 8), head, out, depth + 1);
}

}  // namespace

std::vector<LotEntry> Lots() {
    std::vector<LotEntry> out;
    Ptr g = Globals();
    Ptr nh = g ? Rd<Ptr>(g + addr::globals::kNeighborhood) : 0;
    if (!IsKindOf(nh, "cTSNeighborhood")) return out;
    Ptr head = Rd<Ptr>(nh + addr::nhood::kLotMap);
    std::vector<std::pair<int, Ptr>> entries;
    WalkMap(Rd<Ptr>(head + 4), head, entries, 0);  // root = head's parent link
    for (const auto& e : entries) {
        Ptr lot = e.second - addr::lot::kInterface;
        if (!IsKindOf(lot, "cTSLotInfo")) continue;
        Ptr base = Rd<Ptr>(lot + addr::lot::kBase) - addr::baselot::kInterface;
        LotEntry l;
        l.id = e.first;
        l.x = Rd<int>(lot + addr::lot::kX);
        l.y = Rd<int>(lot + addr::lot::kY);
        l.type = Rd<int>(base + addr::baselot::kType);
        l.width = Rd<int>(base + addr::baselot::kWidth);
        l.depth = Rd<int>(base + addr::baselot::kDepth);
        l.flags = Rd<uint32_t>(base + addr::baselot::kFlags);
        l.name = ReadStdString(base + addr::baselot::kName + 4);
        out.push_back(l);
    }
    return out;
}

std::vector<FamilyEntry> Families() {
    std::vector<FamilyEntry> out;
    Ptr g = Globals();
    Ptr fm = g ? Rd<Ptr>(g + addr::globals::kFamilyManager) : 0;
    if (!IsKindOf(fm, "cTSFamilyManager")) return out;
    Ptr b = Rd<Ptr>(fm + addr::familymgr::kBegin), e = Rd<Ptr>(fm + addr::familymgr::kEnd);
    for (Ptr p = b; p && p < e && out.size() < 2000; p += 4) {
        Ptr f = Rd<Ptr>(p);
        if (!IsKindOf(f, "cTSFamily")) continue;
        FamilyEntry fe;
        fe.id = Rd<int>(f + addr::family::kId);
        fe.lot = Rd<int>(f + addr::family::kLot);
        fe.name = ReadStdString(f + addr::family::kName + 4);
        fe.description = ReadStdString(f + addr::family::kDescription + 4);
        fe.funds = Rd<int>(f + addr::family::kFunds);
        fe.members = (int)((Rd<Ptr>(f + addr::family::kMembersEnd) - Rd<Ptr>(f + addr::family::kMembersBegin)) / 4);
        out.push_back(fe);
    }
    return out;
}

Ptr LotInfoProc() {
    Ptr win = FindWindowById(MainWindow(), addr::lotinfo::kWindowId, 0);
    if (!win) return 0;
    Ptr proc = Rd<Ptr>(win + addr::win::kProc);
    return IsKindOf(proc, "cTSWinProcLotInfoDialog") ? proc : 0;
}

namespace {
int g_forcedLot = -1;
typedef char(__fastcall* LotAtScreenFn)(Ptr self, void* edx, int* pos, int* lot, Ptr name);
LotAtScreenFn g_lotAtScreen = nullptr;
char __fastcall LotAtScreenDetour(Ptr self, void* edx, int* pos, int* lot, Ptr name) {
    if (g_forcedLot >= 0 && lot) {
        *lot = g_forcedLot;
        return 1;
    }
    return g_lotAtScreen(self, edx, pos, lot, name);
}
}  // namespace

void InitNeighborhoodHooks() {
    HookGame("cTSUserToolNeighborhood::LotAtScreen", addr::nhoodtool::kLotAtScreen, (void*)LotAtScreenDetour,
             (void**)&g_lotAtScreen);
}

bool ClickLotWithTool(int lotId) {
    Ptr tool = CurrentTool();
    if (!IsKindOf(tool, "cTSUserToolNeighborhoodMoveIn")) return false;
    typedef void(__thiscall * DispatchFn)(Ptr, int, int, int*);
    Ptr vt = Rd<Ptr>(tool);
    auto dispatch = InImage(vt) ? (DispatchFn)Rd<Ptr>(vt + addr::nhoodtool::kSlotDispatch * 4) : nullptr;
    if (!dispatch) return false;
    int pos[2] = {1280, 640};
    MouseMove(pos[0], pos[1]);  // the tool reads mouse input only while a cursor shows
    g_forcedLot = lotId;
    Log("move-in tool %p: click on lot %d", (void*)tool, lotId);
    // A dialog the click opens runs its loop inside this call; the mod keeps ticking meanwhile.
    bool ok = Guarded([&] { dispatch(tool, 0, 4, pos); });
    ok = ok && Guarded([&] { dispatch(tool, 0, 5, pos); });
    g_forcedLot = -1;
    return ok;
}

bool ShowLotInfo(int lotId, int arg2, int mode) {
    Ptr proc = LotInfoProc();
    if (!proc) {
        Log("lot info dialog not found");
        return false;
    }
    Ptr iface = proc + addr::lotinfo::kInterface;
    typedef void(__thiscall * ShowFn)(Ptr, int, int, int);
    auto show = (ShowFn)Va(addr::lotinfo::kShow);
    bool ok = Guarded([&] { show(iface, lotId, arg2, mode); });
    Log("ShowLotInfo(%d, %d, %d): %s", lotId, arg2, mode, ok ? "ok" : "crashed");
    return ok;
}

}  // namespace game
