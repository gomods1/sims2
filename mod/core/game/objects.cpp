#include "objects.h"

#include "../util/log.h"
#include "addresses.h"
#include "gz.h"
#include "rtti.h"

namespace game {
namespace {

template <class Fn>
Fn Slot(Ptr obj, int slot) {
    Ptr vt = Rd<Ptr>(obj);
    return InImage(vt) ? (Fn)Rd<Ptr>(vt + slot * 4) : nullptr;
}

Ptr ObjectModule() {
    Ptr g = Rd<Ptr>(Va(addr::globals::kInstance));
    Ptr om = g ? Rd<Ptr>(g + addr::globals::kObjectModule) : 0;
    return IsKindOf(om, "cEdithObjectModule") ? om : 0;
}

}  // namespace

std::string CatalogName(uint32_t guid) {
    Ptr g = Rd<Ptr>(Va(addr::globals::kInstance));
    if (!g) return "";
    typedef Ptr(__thiscall * GetFn)(Ptr);
    typedef Ptr(__thiscall * ByGuidFn)(Ptr, uint32_t);
    typedef bool(__thiscall * NameFn)(Ptr, Ptr*);
    typedef const char*(__thiscall * TextFn)(Ptr);
    typedef uint32_t(__thiscall * ReleaseFn)(Ptr);
    std::string out;
    Guarded([&] {
        Ptr folder = Slot<GetFn>(g, addr::catalog::kGlobalsSlotFolder)(g);
        if (!folder) return;
        Ptr entry = Slot<ByGuidFn>(folder, addr::catalog::kFolderSlotByGuid)(folder, guid);
        if (!entry) return;
        Ptr str = 0;
        if (!Slot<NameFn>(entry, addr::catalog::kEntrySlotName)(entry, &str) || !str) return;
        const char* text = Slot<TextFn>(str, addr::catalog::kStringSlotText)(str);
        char buf[256] = {};
        if (text && Read((Ptr)text, buf, sizeof buf - 1)) out = buf;
        Slot<ReleaseFn>(str, 2)(str);
    });
    return out;
}

namespace {

bool Fill(Ptr o, int id, LotObject* out) {
    if (!IsKindOf(o, "cTSObject")) return false;
    out->id = id;
    out->ptr = o;
    out->person = IsKindOf(o, "cTSPerson");
    typedef const char*(__thiscall * NameFn)(Ptr);
    typedef Ptr(__thiscall * LocationFn)(Ptr);
    typedef short(__thiscall * LevelFn)(Ptr);
    const char* name = nullptr;
    Ptr loc = 0;
    short level = 0;
    if (auto f = Slot<NameFn>(o, addr::object::kSlotName)) Guarded([&] { name = f(o); });
    if (auto f = Slot<LocationFn>(o, addr::object::kSlotLocation)) Guarded([&] { loc = f(o); });
    if (auto f = Slot<LevelFn>(o, addr::object::kSlotLevel)) Guarded([&] { level = f(o); });
    char buf[256] = {};
    if (name && Read((Ptr)name, buf, sizeof buf - 1)) out->name = buf;
    if (loc) {
        out->x = Rd<int>(loc) / 65536.0f;
        out->y = Rd<int>(loc + 4) / 65536.0f;
    }
    out->level = level - 1;
    Ptr def = Rd<Ptr>(o + addr::object::kDefinition);
    if (def) {
        out->guid = Rd<uint32_t>(def + addr::objd::kGuid);
        out->price = Rd<int>(def + addr::objd::kPrice);
        out->functionFlags = Rd<uint16_t>(def + addr::objd::kFunctionFlags);
        out->roomFlags = Rd<uint16_t>(def + addr::objd::kRoomFlags);
    }
    if (out->guid && !out->person) out->catalogName = CatalogName(out->guid);
    if (out->x >= 0) out->room = RoomAt(out->x, out->y, out->level);
    return true;
}

}  // namespace

std::vector<LotObject> LotObjects() {
    std::vector<LotObject> out;
    Ptr om = ObjectModule();
    if (!om) return out;
    Ptr b = Rd<Ptr>(om + addr::objmodule::kById), e = Rd<Ptr>(om + addr::objmodule::kById + 4);
    int n = (int)((e - b) / 4);
    for (int i = 0; i < n && i < 20000; i++) {
        Ptr o = Rd<Ptr>(b + i * 4);
        if (!o || IsKindOf(o, "cEdithMTObject")) continue;  // parts of multi-tile objects
        LotObject lo;
        if (Fill(o, i + 1, &lo)) out.push_back(lo);
    }
    return out;
}

std::vector<WallObject> WallObjects() {
    std::vector<WallObject> out;
    Ptr om = ObjectModule();
    if (!om) return out;
    Ptr b = Rd<Ptr>(om + addr::objmodule::kById), e = Rd<Ptr>(om + addr::objmodule::kById + 4);
    int n = (int)((e - b) / 4);
    for (int i = 0; i < n && i < 20000; i++) {
        Ptr o = Rd<Ptr>(b + i * 4);
        if (!o) continue;
        Ptr def = Rd<Ptr>(o + addr::object::kDefinition);
        if (!def) continue;
        int type = Rd<uint16_t>(def + addr::objd::kType);
        if ((type != 8 && type != 9) || Rd<uint16_t>(def + addr::objd::kSubIndex) != 0) continue;
        LotObject lo;
        if (!Fill(o, i + 1, &lo) || lo.x < 0) continue;
        WallObject w;
        w.type = type;
        w.x = lo.x;
        w.y = lo.y;
        w.level = lo.level;
        w.direction = ((Rd<int16_t>(o + addr::object::kDirection) % 8) + 8) % 8;
        // Parts carry internal names ("*Door - 1 Tile - ..."): no name then.
        w.name = !lo.catalogName.empty() && lo.catalogName[0] != '*' ? lo.catalogName : "";
        out.push_back(w);
    }
    return out;
}

bool ReadLotObject(int id, LotObject* out) {
    Ptr om = ObjectModule();
    if (!om || id < 1) return false;
    Ptr b = Rd<Ptr>(om + addr::objmodule::kById), e = Rd<Ptr>(om + addr::objmodule::kById + 4);
    if (id > (int)((e - b) / 4)) return false;
    return Fill(Rd<Ptr>(b + (id - 1) * 4), id, out);
}

std::vector<int> FamilySimIds() {
    std::vector<int> ids;
    Ptr om = ObjectModule();
    if (!om) return ids;
    Ptr fb = Rd<Ptr>(om + addr::objmodule::kFamily), fe = Rd<Ptr>(om + addr::objmodule::kFamily + 4);
    Ptr b = Rd<Ptr>(om + addr::objmodule::kById), e = Rd<Ptr>(om + addr::objmodule::kById + 4);
    for (Ptr p = fb; p && p < fe && ids.size() < 64; p += 4) {
        Ptr sim = CompleteObject(Rd<Ptr>(p));
        for (int i = 0; i < (int)((e - b) / 4) && i < 20000; i++)
            if (Rd<Ptr>(b + i * 4) == sim) ids.push_back(i + 1);
    }
    return ids;
}

Ptr SelectedSim() {
    Ptr om = ObjectModule();
    if (!om) return 0;
    typedef Ptr(__thiscall * Fn)(Ptr);
    Ptr sim = 0;
    if (auto f = Slot<Fn>(om, addr::objmodule::kSlotSelectedSim)) Guarded([&] { sim = f(om); });
    // The person list holds an interface of cTSPerson (+752): the object starts before it.
    return IsKindOf(sim, "cTSPerson") ? CompleteObject(sim) : 0;
}

int SelectedSimId() {
    Ptr sim = SelectedSim();
    Ptr om = ObjectModule();
    if (!sim || !om) return 0;
    Ptr b = Rd<Ptr>(om + addr::objmodule::kById), e = Rd<Ptr>(om + addr::objmodule::kById + 4);
    for (int i = 0; i < (int)((e - b) / 4) && i < 20000; i++)
        if (Rd<Ptr>(b + i * 4) == sim) return i + 1;
    return 0;
}

namespace {

// A cRZString the game can write into: vtable, std::string (16-byte buffer, size, capacity 15),
// reference count. A name longer than 15 bytes makes the game allocate: that block is left (a
// few bytes per call), since only the game's allocator may free it.
struct RZString {
    Ptr vtable;
    char buf[16];
    uint32_t size;
    uint32_t cap;
    uint32_t refs;
    uint32_t pad[4];
};

std::string Text(const RZString& s) {
    if (s.size == 0 || s.size > 4096) return "";
    Ptr data = s.cap > 15 ? *(const Ptr*)s.buf : (Ptr)s.buf;
    std::string out(s.size, '\0');
    return Read(data, out.data(), s.size) ? out : "";
}

}  // namespace

bool SimName(int objectId, std::string* first, std::string* last) {
    LotObject o;
    if (!ReadLotObject(objectId, &o) || !o.person) return false;
    Ptr iface = o.ptr + addr::person::kInterface;
    if (!IsKindOf(iface, "cTSPerson")) return false;
    typedef Ptr(__thiscall * GetFn)(Ptr);
    typedef void(__thiscall * NameFn)(Ptr, RZString*);
    RZString a = {}, b = {};
    a.vtable = b.vtable = Va(addr::person::kRZStringVtable);
    a.cap = b.cap = 15;
    bool ok = Guarded([&] {
        Ptr sim = Slot<GetFn>(iface, addr::person::kSlotSim)(iface);
        if (!sim) return;
        Ptr nb = Slot<GetFn>(sim, addr::person::kSimSlotNeighbor)(sim);
        if (!nb) return;
        Slot<NameFn>(nb, addr::person::kNeighborSlotFirstName)(nb, &a);
        Slot<NameFn>(nb, addr::person::kNeighborSlotLastName)(nb, &b);
    });
    if (!ok) return false;
    *first = Text(a);
    *last = Text(b);
    return !first->empty() || !last->empty();
}

int RoomAt(float x, float y, int level) {
    static Ptr s_world = 0;
    if (!s_world) {
        Ptr fw = Va(addr::kFramework);
        typedef bool(__thiscall * GetServiceFn)(Ptr, uint32_t, uint32_t, void**);
        void* out = nullptr;
        bool ok = false;
        if (auto get = Slot<GetServiceFn>(fw, addr::kFrameworkGetSystemService))
            Guarded([&] { ok = get(fw, addr::world::kServiceId, addr::world::kIid, &out); });
        if (!ok || !out) return -1;
        s_world = (Ptr)out;  // a service: it outlives the mod's use (the reference is kept)
    }
    struct Spot {
        float x, y;
        int level;
    } spot = {x, y, level};
    typedef Ptr(__thiscall * GetFn)(Ptr);
    typedef short(__thiscall * RoomFn)(Ptr, Spot*, int, int);
    short room = -1;
    bool ok = Guarded([&] {
        Ptr rm = Slot<GetFn>(s_world, addr::world::kSlotRoomManager)(s_world);
        if (rm) room = Slot<RoomFn>(rm, addr::world::kRoomManagerSlotRoomAt)(rm, &spot, 2, 1);
    });
    return ok ? room : -1;
}

bool GamePaused() {
    Ptr sim = Rd<Ptr>(Va(addr::sim::kInstance));
    return sim && Rd<int16_t>(sim + addr::sim::kPaused) != 0;
}

bool SimMotive(int objectId, int index, float* value) {
    LotObject o;
    if (!ReadLotObject(objectId, &o) || !o.person) return false;
    typedef bool(__thiscall * QueryFn)(Ptr, uint32_t, Ptr*);
    typedef float(__thiscall * MotiveFn)(Ptr, int);
    typedef uint32_t(__thiscall * ReleaseFn)(Ptr);
    Ptr iface = 0;
    bool ok = false;
    Guarded([&] {
        if (!Slot<QueryFn>(o.ptr, 0)(o.ptr, addr::person::kIid, &iface) || !iface) return;
        *value = Slot<MotiveFn>(iface, addr::person::kSlotMotive)(iface, index);
        Slot<ReleaseFn>(iface, 2)(iface);
        ok = true;
    });
    return ok;
}

bool ShowInteractions(int objectId) {
    typedef void*(__cdecl * AllocFn)(size_t);
    typedef Ptr(__thiscall * CtorFn)(Ptr);
    auto alloc = (AllocFn)Va(addr::message::kAlloc);
    auto ctor = (CtorFn)Va(addr::message::kCtor);
    Ptr msg = 0;
    if (!Guarded([&] { msg = (Ptr)alloc(addr::message::kSize); }) || !msg) return false;
    bool ok = Guarded([&] { ctor(msg); });
    typedef uint32_t(__thiscall * RefFn)(Ptr);
    typedef bool(__thiscall * SetFn)(Ptr, uint32_t);
    ok = ok && Guarded([&] {
        Slot<RefFn>(msg, 1)(msg);  // AddRef
        Slot<SetFn>(msg, addr::message::kSlotSetType)(msg, addr::message::kShowPieMenu);
        Slot<SetFn>(msg, addr::message::kSlotSetData1)(msg, (uint32_t)objectId);
    });
    Ptr server = Rd<Ptr>(Va(addr::message::kServer));
    typedef bool(__thiscall * PostFn)(Ptr, Ptr, uint32_t);
    bool posted = false;
    if (ok && server) ok = Guarded([&] { posted = Slot<PostFn>(server, addr::message::kSlotPost)(server, msg, 0); });
    Guarded([&] { Slot<RefFn>(msg, 2)(msg); });  // Release: the server holds its own reference
    Log("interaction menu for object %d: %s", objectId, ok ? (posted ? "posted" : "refused") : "crashed");
    return ok && posted;
}

}  // namespace game
