#include "buy.h"

#include <map>

#include "../util/log.h"
#include "addresses.h"
#include "gz.h"
#include "hooks.h"
#include "rtti.h"

namespace game {
namespace {

bool g_override = false;
float g_x = 0, g_y = 0;
int g_level = 0;
Ptr g_tool = 0;
int g_verdict = -1;
uint32_t g_verdicts = 0;

// cTSUserToolBase::slot31 (thiscall: screen x, y, float out[3] {x, y, level 1-based}, flag) -> bool
typedef char(__fastcall* PickFn)(Ptr self, void* edx, int sx, int sy, float* out, int flag);
PickFn g_pick = nullptr;
char __fastcall PickDetour(Ptr self, void* edx, int sx, int sy, float* out, int flag) {
    if (g_override && out) {
        out[0] = g_x;
        out[1] = g_y;
        ((int*)out)[2] = g_level;
        if (self != g_tool) Log("pick by %p (%s)", (void*)self, ClassName(self).c_str());
        g_tool = self;
        return 1;
    }
    return g_pick(self, edx, sx, sy, out, flag);
}

// cTSUserToolObject::slot86 (thiscall: bool fits): tints the held object.
typedef void(__fastcall* VerdictFn)(Ptr self, void* edx, uint32_t fits);
VerdictFn g_verdictFn = nullptr;
void __fastcall VerdictDetour(Ptr self, void* edx, uint32_t fits) {
    g_verdict = (fits & 0xff) ? 1 : 0;
    g_verdicts++;
    g_verdictFn(self, edx, fits);
}

// cTSUserToolBase::slot29 (thiscall: sx, sy, 1, a, b) -> the scene node under the cursor.
Ptr g_forcedPick = 0;
bool g_forceNothing = false;
typedef Ptr(__fastcall* ObjectPickFn)(Ptr self, void* edx, int sx, int sy, int one, int a, int b);
ObjectPickFn g_objectPick = nullptr;
Ptr __fastcall ObjectPickDetour(Ptr self, void* edx, int sx, int sy, int one, int a, int b) {
    if (g_forcedPick) return g_forcedPick;
    if (g_forceNothing) return 0;
    return g_objectPick(self, edx, sx, sy, one, a, b);
}

// The scene node of a lot object: the node holds the object at +0x88. Found by scanning the
// process's private writable memory for that pointer (a fraction of a second), then cached.
std::map<Ptr, Ptr> g_nodes;
Ptr NodeInterface(Ptr node) {
    std::string cls = ClassName(node);
    for (const auto& k : addr::scenenode::kPickInterface)
        if (cls == k.cls) return node + k.offset;
    return 0;
}
Ptr FindNode(Ptr object) {
    auto it = g_nodes.find(object);
    if (it != g_nodes.end() && Rd<Ptr>(it->second + addr::scenenode::kObject) == object && NodeInterface(it->second))
        return it->second;
    ULONGLONG t0 = GetTickCount64();
    MEMORY_BASIC_INFORMATION mbi;
    Ptr found = 0;
    for (Ptr p = 0x10000; p < 0x7fff0000 && !found; p += mbi.RegionSize) {
        if (!VirtualQuery((void*)p, &mbi, sizeof mbi)) break;
        p = (Ptr)mbi.BaseAddress;
        if (mbi.State != MEM_COMMIT || !(mbi.Protect & (PAGE_READWRITE | PAGE_EXECUTE_READWRITE | PAGE_WRITECOPY)) ||
            (mbi.Protect & PAGE_GUARD))
            continue;
        const uint32_t* w = (const uint32_t*)mbi.BaseAddress;
        size_t n = mbi.RegionSize / 4;
        for (size_t i = 0x88 / 4; i < n; i++) {
            if (w[i] != (uint32_t)object) continue;
            Ptr node = (Ptr)&w[i] - addr::scenenode::kObject;
            if (NodeInterface(node)) {
                found = node;
                break;
            }
        }
    }
    Log("scene node of %p: %p (%s), %llu ms", (void*)object, (void*)found, ClassName(found).c_str(), GetTickCount64() - t0);
    if (found) g_nodes[object] = found;
    return found;
}

// cTSUserToolBase::slot48 (thiscall): the cursor left the 3D view. The real cursor is elsewhere
// while the mod places (the player's other windows), so the tool would drop the spot at once.
typedef void(__fastcall* LeaveFn)(Ptr self, void* edx);
LeaveFn g_leave = nullptr;
void __fastcall LeaveDetour(Ptr self, void* edx) {
    if (g_override) return;
    g_leave(self, edx);
}

Ptr MainToolbar() {
    static Ptr s_toolbar = 0;
    if (s_toolbar) return s_toolbar;
    Ptr fw = Va(addr::kFramework);
    typedef bool(__thiscall * GetServiceFn)(Ptr, uint32_t, uint32_t, void**);
    Ptr vt = Rd<Ptr>(fw);
    auto get = InImage(vt) ? (GetServiceFn)Rd<Ptr>(vt + addr::kFrameworkGetSystemService * 4) : nullptr;
    void* out = nullptr;
    bool ok = false;
    if (get) Guarded([&] { ok = get(fw, addr::toolbar::kServiceId, addr::toolbar::kIidUnknown, &out); });
    if (!ok || !IsKindOf((Ptr)out, "cTSMainToolbar")) {
        Log("main toolbar not found (%p %s)", out, ClassName((Ptr)out).c_str());
        return 0;
    }
    s_toolbar = CompleteObject((Ptr)out);  // a service: it lives as long as the game (the reference is kept)
    return s_toolbar;
}

}  // namespace

bool ForceObjectPick(Ptr object) {
    Ptr node = FindNode(object);
    g_forcedPick = node ? NodeInterface(node) : 0;
    return g_forcedPick != 0;
}

void ForceNoObjectPick() {
    g_forcedPick = 0;
    g_forceNothing = true;
}

void ClearObjectPick() {
    g_forcedPick = 0;
    g_forceNothing = false;
}

Ptr CurrentTool() {
    Ptr tb = MainToolbar();
    Ptr h = tb ? Rd<Ptr>(tb + addr::toolbar::kToolHandler) : 0;
    return h ? Rd<Ptr>(h - addr::toolbar::kHandlerInterface + addr::toolbar::kHandlerTool) : 0;
}

std::string CurrentModeClass() {
    Ptr tb = MainToolbar();
    return tb ? ClassName(Rd<Ptr>(tb + addr::toolbar::kMode)) : "";
}

bool ReadProduct(Ptr thumbnail, Product* out) {
    Ptr proc = Rd<Ptr>(thumbnail + addr::win::kProc);
    if (!IsKindOf(proc, "cTSWinProcCatalogThumbnail")) return false;
    Ptr iface = Rd<Ptr>(proc + addr::product::kThumbnailProduct);
    if (!iface) return false;
    Ptr p = iface - addr::product::kInterfaceOffset;
    if (!IsKindOf(p, "cTSProductObject")) return false;
    out->name = ReadStdStringUtf8(p + addr::product::kName);
    out->description = ReadStdStringUtf8(p + addr::product::kDescription);
    out->internalName = ReadStdString(p + addr::product::kInternalName);
    out->price = Rd<int>(p + addr::product::kPrice);
    Ptr selector = Rd<Ptr>(p + addr::product::kSelector);
    Ptr objd = selector ? Rd<Ptr>(selector + addr::product::kSelectorObjd) : 0;
    if (objd) {
        out->guid = Rd<uint32_t>(objd + addr::objd::kGuid);
        for (int i = 0; i < kRatingCount; i++) out->ratings[i] = Rd<int16_t>(objd + addr::objd::kRatings + 2 * i);
    }
    return !out->name.empty() || !out->internalName.empty();
}

void InitBuyHooks() {
    HookGame("cTSUserToolBase::Pick", addr::tool::kPick, (void*)PickDetour, (void**)&g_pick);
    HookGame("cTSUserToolObject::SetFits", addr::tool::kObjectVerdict, (void*)VerdictDetour, (void**)&g_verdictFn);
    HookGame("cTSUserToolBase::ObjectPick", addr::tool::kObjectPick, (void*)ObjectPickDetour, (void**)&g_objectPick);
    HookGame("cTSUserToolBase::MouseLeave", addr::tool::kMouseLeave, (void*)LeaveDetour, (void**)&g_leave);
}

void SetPickOverride(bool on, float x, float y, int level) {
    g_override = on;
    g_x = x;
    g_y = y;
    g_level = level;
    if (!on) g_tool = 0;
}

Ptr PickTool() { return g_tool; }

int PlacementVerdict(uint32_t* counter) {
    if (counter) *counter = g_verdicts;
    return g_verdict;
}

bool ReadHeldObject(Ptr tool, HeldObject* out) {
    if (!IsKindOf(tool, "cTSUserToolObject")) return false;
    out->guid = Rd<uint32_t>(tool + addr::tool::kHeldGuid);
    out->x = Rd<float>(tool + addr::tool::kSnapped);
    out->y = Rd<float>(tool + addr::tool::kSnapped + 4);
    out->level = Rd<int>(tool + addr::tool::kSnapped + 8);
    out->overLot = Rd<uint8_t>(tool + addr::tool::kOverLot) != 0;
    out->direction = ((Rd<int>(tool + addr::tool::kRotation) % 8) + 8) % 8;
    typedef char(__thiscall * BoolFn)(Ptr);
    typedef Ptr(__thiscall * PtrFn)(Ptr);
    if (FunctionOk(addr::tool::kIsHolding) && FunctionOk(addr::tool::kHeldObject)) {
        Guarded([&] {
            out->holding = ((BoolFn)Va(addr::tool::kIsHolding))(tool) != 0;
            out->object = out->holding ? ((PtrFn)Va(addr::tool::kHeldObject))(tool) : 0;
        });
    }
    return true;
}

int CheckSpot(Ptr tool, float x, float y, int level) {
    if (!IsKindOf(tool, "cTSUserToolObject")) return -1;
    bool wasOn = g_override;
    float ox = g_x, oy = g_y;
    int ol = g_level;
    g_override = true;
    g_x = x;
    g_y = y;
    g_level = level;
    uint32_t before = g_verdicts;
    bool off = false;
    // What the tool's message dispatcher (0xa078dc91) does for a mouse move: the pick, then the move.
    typedef char(__thiscall * MessageFn)(Ptr, int, int*);
    typedef char(__thiscall * MoveFn)(Ptr, int*);
    int pos[2] = {1280, 640};
    Guarded([&] {
        Ptr vt = Rd<Ptr>(tool);
        auto message = (MessageFn)Rd<Ptr>(vt + addr::tool::kSlotMouseMessage * 4);
        auto move = (MoveFn)Rd<Ptr>(vt + addr::tool::kSlotMouseMove * 4);
        if (message && move) {
            message(tool, 3, pos);
            if (!Rd<uint8_t>(tool + addr::tool::kOverLot)) off = true;
            else move(tool, pos);
        }
    });
    g_override = wasOn;
    g_x = ox;
    g_y = oy;
    g_level = ol;
    if (off) return -2;
    return g_verdicts != before ? g_verdict : -1;
}

}  // namespace game
