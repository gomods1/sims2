#include "buy.h"

#include <algorithm>
#include <map>
#include <vector>

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

// cTSUserToolBase::slot32 (thiscall: screen x, y, int level, float out[3]) -> bool: the pick at a
// fixed level, which the tools use once a pick is stored (a wall's drag).
typedef char(__fastcall* PickLevelFn)(Ptr self, void* edx, int sx, int sy, int level, float* out);
PickLevelFn g_pickLevel = nullptr;
char __fastcall PickLevelDetour(Ptr self, void* edx, int sx, int sy, int level, float* out) {
    if (g_override && out) {
        out[0] = g_x;
        out[1] = g_y;
        ((int*)out)[2] = g_level;
        return 1;
    }
    return g_pickLevel(self, edx, sx, sy, level, out);
}

// cTSUserToolObject::slot86 (thiscall: bool fits): tints the held object.
typedef void(__fastcall* VerdictFn)(Ptr self, void* edx, uint32_t fits);
VerdictFn g_verdictFn = nullptr;
void __fastcall VerdictDetour(Ptr self, void* edx, uint32_t fits) {
    int v = (fits & 0xff) ? 1 : 0;
    if (v != g_verdict)
        LogVerbose("verdict %d -> %d at %.2f,%.2f level %d, rotation %d (override %d at %.2f,%.2f)", g_verdict, v,
                   Rd<float>(self + addr::tool::kSnapped), Rd<float>(self + addr::tool::kSnapped + 4),
                   Rd<int>(self + addr::tool::kSnapped + 8), Rd<int>(self + addr::tool::kRotation), g_override ? 1 : 0, g_x, g_y);
    g_verdict = v;
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

namespace {

// The multi-tile groups of every object definition the object folder holds: (resource file,
// master id) -> the parts' sub indexes. Rebuilt when the folder's count changes (a pack's objects
// load with the lot).
// The sub indexes of every part of the multi-tile group a selector belongs to (the master's -1
// included), nullptr when unknown.
const std::vector<uint16_t>* GroupSubs(Ptr selector) {
    Ptr objd = selector ? Rd<Ptr>(selector + addr::product::kSelectorObjd) : 0;
    if (!objd) return nullptr;
    uint16_t master = Rd<uint16_t>(objd + addr::objd::kMasterId);
    if (master == 0) return nullptr;
    using Key = std::pair<Ptr, uint16_t>;
    static Ptr cachedFolder = 0;
    static size_t cachedCount = 0;
    static std::map<Key, std::vector<uint16_t>> groups;
    Ptr g = Rd<Ptr>(Va(addr::globals::kInstance));
    if (!g) return nullptr;
    typedef Ptr(__thiscall * GetFn)(Ptr);
    Ptr folder = 0;
    if (!Guarded([&] { folder = ((GetFn)Rd<Ptr>(Rd<Ptr>(g) + 4 * addr::catalog::kGlobalsSlotFolder))(g); }) || !folder ||
        !Rd<uint8_t>(folder + addr::catalog::kFolderLoaded))
        return nullptr;
    Ptr b1 = Rd<Ptr>(folder + addr::catalog::kFolderSelectors), e1 = Rd<Ptr>(folder + addr::catalog::kFolderSelectors + 4);
    Ptr more = Rd<Ptr>(folder + addr::catalog::kFolderMoreSelectors);
    Ptr b2 = more ? Rd<Ptr>(more) : 0, e2 = more ? Rd<Ptr>(more + 4) : 0;
    size_t count = (e1 >= b1 ? (e1 - b1) / 4 : 0) + (e2 >= b2 ? (e2 - b2) / 4 : 0);
    if (count > 200000) return nullptr;
    if (folder != cachedFolder || count != cachedCount) {
        groups.clear();
        cachedFolder = folder;
        cachedCount = count;
        auto add = [&](Ptr begin, Ptr end) {
            if (!begin || end < begin) return;
            std::vector<Ptr> list((end - begin) / 4);
            if (list.empty() || !Read(begin, list.data(), list.size() * 4)) return;
            for (Ptr s : list) {
                Ptr d = s ? Rd<Ptr>(s + addr::product::kSelectorObjd) : 0;
                uint16_t m = d ? Rd<uint16_t>(d + addr::objd::kMasterId) : 0;
                if (m) groups[{Rd<Ptr>(s + addr::catalog::kSelectorFile), m}].push_back(Rd<uint16_t>(d + addr::objd::kSubIndex));
            }
        };
        add(b1, e1);
        add(b2, e2);
        Log("object folder: %u definitions, %u multi-tile groups", (unsigned)count, (unsigned)groups.size());
    }
    auto hit = groups.find({Rd<Ptr>(selector + addr::catalog::kSelectorFile), master});
    return hit == groups.end() ? nullptr : &hit->second;
}

bool ProductSize(Ptr selector, int* across, int* deep) {
    Ptr objd = selector ? Rd<Ptr>(selector + addr::product::kSelectorObjd) : 0;
    if (!objd) return false;
    if (Rd<uint16_t>(objd + addr::objd::kMasterId) == 0) {
        *across = *deep = 1;
        return true;
    }
    const std::vector<uint16_t>* subs = GroupSubs(selector);
    if (!subs) return false;
    // a part's sub index is its tile offset, one signed byte for each axis; -1 is the master,
    // which takes no tile
    int a0 = 1 << 30, a1 = -(1 << 30), c0 = a0, c1 = a1;
    for (uint16_t sub : *subs) {
        if (sub == 0xffff) continue;
        int a = (int8_t)(sub & 0xff), c = (int8_t)(sub >> 8);
        a0 = std::min(a0, a);
        a1 = std::max(a1, a);
        c0 = std::min(c0, c);
        c1 = std::max(c1, c);
    }
    if (a0 > a1) return false;
    *across = a1 - a0 + 1;
    *deep = c1 - c0 + 1;
    return true;
}

}  // namespace

std::vector<std::pair<int, int>> HeldFootprint(Ptr object, int direction, int x, int y) {
    std::vector<std::pair<int, int>> out;
    Ptr def = object ? Rd<Ptr>(object + addr::object::kDefinition) : 0;
    const std::vector<uint16_t>* subs = object ? GroupSubs(Rd<Ptr>(object + addr::object::kSelector)) : nullptr;
    if (!def || !subs) return out;
    uint16_t own = Rd<uint16_t>(def + addr::objd::kSubIndex);
    if (own == 0xffff) return out;
    // Read live on a double bed facing north (2026-11-10, !parts): sub (column << 8) | row, row 0
    // the front row, columns toward the object's left; a part's tile is the lead's moved back one
    // tile a row and left one a column from the lead's own.
    static const int kFx[] = {-1, 0, 1, 0}, kFy[] = {0, 1, 0, -1};  // facing: 0 -x, 2 +y, 4 +x, 6 -y
    int d = (direction / 2) & 3;
    int fx = kFx[d], fy = kFy[d], rx = -fy, ry = fx;  // forward, and right of it
    int lc = (int8_t)(own >> 8), lr = (int8_t)(own & 0xff);
    for (uint16_t sub : *subs) {
        if (sub == 0xffff) continue;
        int c = (int8_t)(sub >> 8), r = (int8_t)(sub & 0xff);
        std::pair<int, int> t = {x + (lc - c) * rx + (lr - r) * fx, y + (lc - c) * ry + (lr - r) * fy};
        if (std::find(out.begin(), out.end(), t) == out.end()) out.push_back(t);
    }
    return out;
}

bool ReadProductInterface(Ptr iface, Product* out);

bool ReadProduct(Ptr thumbnail, Product* out) {
    Ptr proc = Rd<Ptr>(thumbnail + addr::win::kProc);
    if (!IsKindOf(proc, "cTSWinProcCatalogThumbnail")) return false;
    Ptr iface = Rd<Ptr>(proc + addr::product::kThumbnailProduct);
    return iface && ReadProductInterface(iface, out);
}

bool ReadRewardProduct(Ptr thumbnail, Product* out) {
    Ptr proc = CompleteObject(Rd<Ptr>(thumbnail + addr::win::kProc));
    if (!IsKindOf(proc, "cTSWinProcAspirationRewardThumbnail") && !IsKindOf(proc, "cTSWinProcCareerRewardThumbnail"))
        return false;
    int index = Rd<int>(proc + addr::reward::kThumbnailIndex, -1);
    for (Ptr w = Parent(thumbnail); w; w = Parent(w)) {
        Ptr catalog = CompleteObject(Rd<Ptr>(w + addr::win::kProc));
        if (!IsKindOf(catalog, "cTSWinProcAspirationRewardsCatalog") && !IsKindOf(catalog, "cTSWinProcCareerRewardsCatalog"))
            continue;
        Ptr begin = Rd<Ptr>(catalog + addr::reward::kCatalogItems), end = Rd<Ptr>(catalog + addr::reward::kCatalogItems + 4);
        if (index < 0 || end < begin || index >= (int)((end - begin) / 4)) return false;
        Ptr iface = Rd<Ptr>(begin + 4 * index);
        return iface && ReadProductInterface(iface, out);
    }
    return false;
}

bool ReadProductInterface(Ptr iface, Product* out) {
    // Every catalog product (objects, walls, floors, roofs...) is a cTSProduct: name,
    // description and price where cTSProduct keeps them; ratings only for objects.
    Ptr p = CompleteObject(iface);
    if (!IsKindOf(p, "cTSProduct")) return false;
    out->isObject = IsKindOf(p, "cTSProductObject");
    out->name = ReadStdStringUtf8(p + addr::product::kName);
    out->description = ReadStdStringUtf8(p + addr::product::kDescription);
    out->internalName = ReadStdString(p + addr::product::kInternalName);
    out->price = Rd<int>(p + addr::product::kPrice);
    Ptr selector = out->isObject ? Rd<Ptr>(p + addr::product::kSelector) : 0;
    Ptr objd = selector ? Rd<Ptr>(selector + addr::product::kSelectorObjd) : 0;
    if (objd) {
        out->guid = Rd<uint32_t>(objd + addr::objd::kGuid);
        for (int i = 0; i < kRatingCount; i++) out->ratings[i] = Rd<int16_t>(objd + addr::objd::kRatings + 2 * i);
        if (!ProductSize(selector, &out->across, &out->deep)) out->across = out->deep = 0;
    }
    return !out->name.empty() || !out->internalName.empty();
}

void InitBuyHooks() {
    HookGame("cTSUserToolBase::Pick", addr::tool::kPick, (void*)PickDetour, (void**)&g_pick);
    HookGame("cTSUserToolBase::PickAtLevel", addr::tool::kPickAtLevel, (void*)PickLevelDetour, (void**)&g_pickLevel);
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

bool ToolMessage(Ptr tool, int buttons, int message, int x, int y) {
    typedef char(__thiscall * DispatchFn)(Ptr, int, int, int*, int);
    Ptr vt = Rd<Ptr>(tool);
    auto dispatch = InImage(vt) ? (DispatchFn)Rd<Ptr>(vt + addr::tool::kSlotDispatch * 4) : nullptr;
    if (!dispatch) return false;
    int pos[2] = {x, y};
    return Guarded([&] { dispatch(tool, buttons, message, pos, 0); });
}

bool DragTool(Ptr tool, float x0, float y0, float x1, float y1, int level, bool ctrl) {
    typedef char(__thiscall * DispatchFn)(Ptr, int, int, int*, int);
    Ptr vt = Rd<Ptr>(tool);
    auto dispatch = InImage(vt) ? (DispatchFn)Rd<Ptr>(vt + addr::tool::kSlotDispatch * 4) : nullptr;
    if (!dispatch) return false;
    bool wasOn = g_override;
    float ox = g_x, oy = g_y;
    int ol = g_level;
    int pos[2] = {1280, 640};
    if (ctrl) SetSyntheticModifier(VK_CONTROL, true);
    g_override = true;
    g_level = level;
    g_x = x0;
    g_y = y0;
    bool ok = Guarded([&] { dispatch(tool, 0, 3, pos, 0); });
    ok = ok && Guarded([&] { dispatch(tool, 1, 4, pos, 0); });
    g_x = x1;
    g_y = y1;
    pos[0] += 2;  // a move the tool notices
    ok = ok && Guarded([&] { dispatch(tool, 1, 3, pos, 0); });
    ok = ok && Guarded([&] { dispatch(tool, 1, 5, pos, 0); });
    if (ctrl) SetSyntheticModifier(VK_CONTROL, false);
    g_override = wasOn;
    g_x = ox;
    g_y = oy;
    g_level = ol;
    Log("drag %s from %.1f,%.1f to %.1f,%.1f floor %d%s: %s", ClassName(tool).c_str(), x0, y0, x1, y1, level,
        ctrl ? " (Ctrl)" : "", ok ? "ok" : "crashed");
    return ok;
}

bool ReadHeldObject(Ptr tool, HeldObject* out) {
    if (!IsKindOf(tool, "cTSUserToolObject")) return false;
    out->guid = Rd<uint32_t>(tool + addr::tool::kHeldGuid);
    out->x = Rd<float>(tool + addr::tool::kSnapped);
    out->y = Rd<float>(tool + addr::tool::kSnapped + 4);
    out->level = Rd<int>(tool + addr::tool::kSnapped + 8);
    out->overLot = Rd<uint8_t>(tool + addr::tool::kOverLot) != 0;
    int rotation = ((Rd<int>(tool + addr::tool::kRotation) % 8) + 8) % 8;
    out->direction = (10 - rotation) % 8;  // addr::tool::kRotation: the object faces 2 - rotation
    typedef char(__thiscall * BoolFn)(Ptr);
    typedef Ptr(__thiscall * PtrFn)(Ptr);
    if (FunctionOk(addr::tool::kIsHolding) && FunctionOk(addr::tool::kHeldObject)) {
        Guarded([&] {
            out->holding = ((BoolFn)Va(addr::tool::kIsHolding))(tool) != 0;
            out->object = out->holding ? ((PtrFn)Va(addr::tool::kHeldObject))(tool) : 0;
        });
        Ptr objd = out->object ? Rd<Ptr>(out->object + addr::object::kDefinition) : 0;
        if (objd) out->objectType = Rd<uint16_t>(objd + addr::objd::kType);
        // the object's own direction variable: only the game's turning keys update it while it is
        // held (the mod's rotation writes do not), so `direction` comes from the rotation
        if (out->object) out->facing = ((Rd<int16_t>(out->object + addr::object::kDirection) % 8) + 8) % 8;
    }
    return true;
}

std::string PlacementReason(Ptr tool, int* code) {
    if (!IsKindOf(tool, "cTSUserToolObject")) return "";
    uint32_t count = Rd<uint32_t>(tool + addr::tool::kErrorCount);
    Ptr head = Rd<Ptr>(tool + addr::tool::kErrorSet);
    Ptr first = head ? Rd<Ptr>(head) : 0;
    if (count == 0 || count > 1000 || !first || first == head) return "";
    if (code) *code = Rd<int>(first + addr::tool::kErrorValue);
    typedef bool(__thiscall * TextFn)(Ptr value, Ptr* out);
    typedef const char*(__thiscall * StrFn)(Ptr);
    typedef uint32_t(__thiscall * ReleaseFn)(Ptr);
    std::string text;
    Ptr str = 0;
    Guarded([&] {
        if (!((TextFn)Va(addr::tool::kErrorText))(first + addr::tool::kErrorValue, &str) || !str) return;
        Ptr vt = Rd<Ptr>(str);
        // a cIGZString: slot 5 its text (as the catalog names', addr::catalog::kStringSlotText), 7 its length
        const char* s = ((StrFn)Rd<Ptr>(vt + 4 * addr::catalog::kStringSlotText))(str);
        uint32_t n = ((uint32_t(__thiscall*)(Ptr))Rd<Ptr>(vt + 4 * 7))(str);
        if (s && n > 0 && n < 2000) {
            text.resize(n);
            if (!Read((Ptr)s, &text[0], n)) text.clear();
        }
    });
    if (str) Guarded([&] { ((ReleaseFn)Rd<Ptr>(Rd<Ptr>(str) + 8))(str); });
    // the list of places (kind 2) bullets each: "Debe colocarse en uno de estos lugares: •sobre el
    // suelo. •sobre el terreno" (read live); a screen reader says the bullet
    // (the places are separated by white space and a bullet: "lugares: sobre el suelo, sobre el terreno")
    static const char kBullet[] = "\xe2\x80\xa2";  // U+2022 in UTF-8
    if (text.find(kBullet) != std::string::npos) {
        std::string out;
        size_t from = 0;
        for (size_t at = text.find(kBullet);; at = text.find(kBullet, from)) {
            std::string piece = text.substr(from, at == std::string::npos ? std::string::npos : at - from);
            size_t a = piece.find_first_not_of(" \t\r\n"), z = piece.find_last_not_of(" \t\r\n.");
            piece = a == std::string::npos ? "" : piece.substr(a, z - a + 1);
            if (!piece.empty()) out += (out.empty() ? "" : (out.back() == ':' ? " " : ", ")) + piece;
            if (at == std::string::npos) break;
            from = at + 3;
        }
        text = out;
    }
    LogVerbose("placement reason: %u errors, first code 0x%x kind %d: %s", count, Rd<int>(first + addr::tool::kErrorValue),
               Rd<int>(first + addr::tool::kErrorValue + 0x14), text.c_str());
    return text;
}

int HeldRotation(Ptr tool) { return IsKindOf(tool, "cTSUserToolObject") ? Rd<int>(tool + addr::tool::kRotation) : 0; }

bool SetHeldRotation(Ptr tool, int rotation) {
    return IsKindOf(tool, "cTSUserToolObject") && WriteU32(tool + addr::tool::kRotation, (uint32_t)rotation);
}

bool SetHeldDirection(Ptr tool, int direction) {
    if (!IsKindOf(tool, "cTSUserToolObject")) return false;
    int r = Rd<int>(tool + addr::tool::kRotation);
    int now = ((r % 8) + 8) % 8;
    int want = (10 - (direction & 7)) % 8;  // the rotation that makes the object face `direction`
    int turn = ((want - now) % 8 + 8) % 8;  // the rotation is not wrapped: keep its sense
    return WriteU32(tool + addr::tool::kRotation, (uint32_t)(r + turn));
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
