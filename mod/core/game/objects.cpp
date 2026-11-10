#include "objects.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <map>

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
    // A multi-tile object's lead part has a part's definition ("*Sofa - Club (0,0)"): the whole
    // object's (catalog name, price, buy categories) is its selector's master's.
    if (IsKindOf(o, "cEdithMTObject")) {
        Ptr sel = Rd<Ptr>(o + addr::object::kSelector);
        Ptr master = IsKindOf(sel, "cTSObjectSelector") ? Rd<Ptr>(sel + addr::object::kSelectorMaster) : 0;
        Ptr mdef = IsKindOf(master, "cTSObjectSelector") ? Rd<Ptr>(master + addr::product::kSelectorObjd) : 0;
        if (mdef) {
            int type = def ? Rd<uint16_t>(def + addr::objd::kType) : 0;
            def = mdef;
            out->type = type;  // the part's own (a door's lead part is type 8)
        }
    }
    if (def) {
        out->guid = Rd<uint32_t>(def + addr::objd::kGuid);
        out->price = Rd<int>(def + addr::objd::kPrice);
        out->functionFlags = Rd<uint16_t>(def + addr::objd::kFunctionFlags);
        out->roomFlags = Rd<uint16_t>(def + addr::objd::kRoomFlags);
    }
    if (out->guid && !out->person) out->catalogName = CatalogName(out->guid);
    if (out->x >= 0) out->room = RoomAt(out->x, out->y, out->level);
    out->direction = ((Rd<int16_t>(o + addr::object::kDirection) % 8) + 8) % 8;
    if (def && !out->type) out->type = Rd<uint16_t>(def + addr::objd::kType);
    out->broken = Rd<int16_t>(o + addr::objvar::kBase + 2 * addr::objvar::kBroken) > 0;
    out->dirt = Rd<int16_t>(o + addr::objvar::kBase + 2 * addr::objvar::kDirt);
    return true;
}

// The lead part of a multi-tile object's part (the part itself for the lead), 0 when o is no part.
// A lead placed since the lot loaded keeps 0 in its own +0x2f4 (its parts point at it; the game
// fills it in when the lot loads again): a tester's new bed was missing from every list until he
// left the lot (read live 2026-11-08: a bought bed's lead 425 held 0, its parts 464 and 465 it).
Ptr LeadOf(Ptr o) {
    if (!IsKindOf(o, "cEdithMTObject")) return 0;
    Ptr lead = Rd<Ptr>(o + addr::object::kLeadPart);
    return lead ? lead - addr::object::kLeadInterface : o;
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
        // a multi-tile object counts once, as its lead part
        if (!o || (IsKindOf(o, "cEdithMTObject") && LeadOf(o) != o)) continue;
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

namespace {

Ptr ObjectById(int id) {
    Ptr om = ObjectModule();
    if (!om || id < 1) return 0;
    Ptr b = Rd<Ptr>(om + addr::objmodule::kById), e = Rd<Ptr>(om + addr::objmodule::kById + 4);
    if (id > (int)((e - b) / 4)) return 0;
    Ptr o = Rd<Ptr>(b + (id - 1) * 4);
    return o && IsKindOf(o, "cTSObject") ? o : 0;
}

// A virtual slot of an object, only when it is the function the disassembly found.
template <class Fn>
Fn CheckedSlot(Ptr o, int slot, uintptr_t expected) {
    Ptr vt = o ? Rd<Ptr>(o) : 0;
    Ptr fn = vt ? Rd<Ptr>(vt + 4 * slot) : 0;
    return fn && fn == Va(expected) ? (Fn)fn : nullptr;
}

// One object's (or part's) routing slots in lot tiles.
void AddUseSpots(Ptr o, std::vector<UseSpot>* out) {
    typedef int(__thiscall * CountFn)(Ptr);
    typedef Ptr(__thiscall * GetFn)(Ptr, int);
    typedef int(__thiscall * PositionsFn)(Ptr, Ptr, bool);
    typedef float*(__thiscall * OffsetFn)(Ptr, float*, Ptr, int, float);
    typedef Ptr(__thiscall * LocationFn)(Ptr);
    auto count = CheckedSlot<CountFn>(o, addr::object::kSlotRoutingCount, addr::object::kRoutingCount);
    auto get = CheckedSlot<GetFn>(o, addr::object::kSlotRoutingSlot, addr::object::kRoutingSlot);
    auto positions = CheckedSlot<PositionsFn>(o, addr::object::kSlotSlotPositions, addr::object::kSlotPositions);
    auto offset = CheckedSlot<OffsetFn>(o, addr::object::kSlotSlotOffset, addr::object::kSlotOffset);
    if (!count || !get || !positions || !offset) {
        static int s_logged = 0;
        if (s_logged++ < 5) Log("use spots: %s has other routing slot functions: not read", ClassName(o).c_str());
        return;
    }
    Ptr loc = 0;
    if (auto f = Slot<LocationFn>(o, addr::object::kSlotLocation)) Guarded([&] { loc = f(o); });
    if (!loc) return;
    float ox = Rd<int>(loc) / 65536.0f, oy = Rd<int>(loc + 4) / 65536.0f;
    int dir = ((Rd<int16_t>(o + addr::object::kDirection) % 8) + 8) % 8;
    int n = 0;
    Guarded([&] { n = count(o); });
    for (int i = 0; i < n && i < 64; i++) {
        Ptr p = 0;
        Guarded([&] { p = get(o, i); });
        if (!p) continue;
        Ptr base = p - 0x20;
        uint32_t flags = Rd<uint32_t>(p + 0x10);
        bool sitting = Rd<int>(p + 8) != 0;
        int d = (flags & 0x200) ? 0 : dir;
        int spots = 0;
        Guarded([&] { spots = positions(o, base, false); });
        for (int k = 0; k < spots && k < 16; k++) {
            float local[3] = {};
            bool ok = Guarded([&] { offset(o, local, base, k, 0.0f); });
            if (!ok) continue;
            // The game's composition (route goals 0xa04e0f10): (a, b) = (-y, x) turned by the facing
            // in quarter turns (0xa04e5e80): 2 (b, -a), 4 (-a, -b), 6 (-b, a). Local +y is the front.
            float a = -local[1], b = local[0], wx = a, wy = b;
            switch (d & 6) {
            case 2: wx = b; wy = -a; break;
            case 4: wx = -a; wy = -b; break;
            case 6: wx = -b; wy = a; break;
            }
            UseSpot u;
            u.x = ox + wx;
            u.y = oy + wy;
            u.tileX = (int)std::floor(u.x);
            u.tileY = (int)std::floor(u.y);
            u.sitting = sitting;
            out->push_back(u);
        }
    }
}

}  // namespace

std::vector<UseSpot> UseSpots(int objectId) {
    std::vector<UseSpot> out;
    Ptr lead = ObjectById(objectId);
    if (!lead) return out;
    AddUseSpots(lead, &out);
    // The parts of a multi-tile object load their own slots.
    Ptr om = ObjectModule();
    Ptr b = Rd<Ptr>(om + addr::objmodule::kById), e = Rd<Ptr>(om + addr::objmodule::kById + 4);
    int n = (int)((e - b) / 4);
    for (int i = 0; i < n && i < 20000; i++) {
        Ptr o = Rd<Ptr>(b + i * 4);
        if (!o || o == lead || !IsKindOf(o, "cEdithMTObject") || LeadOf(o) != lead) continue;
        AddUseSpots(o, &out);
    }
    // Each tile once (a sitting spot wins).
    std::vector<UseSpot> unique;
    for (const UseSpot& u : out) {
        auto same = std::find_if(unique.begin(), unique.end(),
                                 [&](const UseSpot& v) { return v.tileX == u.tileX && v.tileY == u.tileY; });
        if (same == unique.end()) unique.push_back(u);
        else same->sitting |= u.sitting;
    }
    return unique;
}

int ObjdField(int objectId, int index) {
    Ptr o = ObjectById(objectId);
    Ptr def = o ? Rd<Ptr>(o + addr::object::kDefinition) : 0;
    // A multi-tile part's own OBJD is the part's: the whole object's is its master selector's.
    if (o && IsKindOf(o, "cEdithMTObject")) {
        Ptr sel = Rd<Ptr>(o + addr::object::kSelector);
        Ptr master = IsKindOf(sel, "cTSObjectSelector") ? Rd<Ptr>(sel + addr::object::kSelectorMaster) : 0;
        Ptr mdef = IsKindOf(master, "cTSObjectSelector") ? Rd<Ptr>(master + addr::product::kSelectorObjd) : 0;
        if (mdef) def = mdef;
    }
    return def && index >= 0 && index < 128 ? Rd<uint16_t>(def + 4 + 2 * index) : -1;
}

int ContainerOf(int objectId) {
    Ptr o = ObjectById(objectId);
    return o ? Rd<int16_t>(o + addr::object::kContainer) : 0;
}

std::vector<PlacedPart> PlacedParts() {
    std::vector<PlacedPart> out;
    Ptr om = ObjectModule();
    if (!om) return out;
    Ptr b = Rd<Ptr>(om + addr::objmodule::kById), e = Rd<Ptr>(om + addr::objmodule::kById + 4);
    int n = (int)((e - b) / 4);
    std::map<Ptr, int> ids;  // object -> id, for the parts' leads
    for (int i = 0; i < n && i < 20000; i++)
        if (Ptr o = Rd<Ptr>(b + i * 4)) ids[o] = i + 1;
    for (int i = 0; i < n && i < 20000; i++) {
        Ptr o = Rd<Ptr>(b + i * 4);
        if (!o || !IsKindOf(o, "cTSObject")) continue;
        // Position, floor and type only (no catalog name: this runs on every cursor step).
        typedef Ptr(__thiscall * LocationFn)(Ptr);
        typedef short(__thiscall * LevelFn)(Ptr);
        Ptr loc = 0;
        short level = 0;
        if (auto f = Slot<LocationFn>(o, addr::object::kSlotLocation)) Guarded([&] { loc = f(o); });
        if (auto f = Slot<LevelFn>(o, addr::object::kSlotLevel)) Guarded([&] { level = f(o); });
        if (!loc) continue;
        float x = Rd<int>(loc) / 65536.0f, y = Rd<int>(loc + 4) / 65536.0f;
        if (x < 0 || y < 0) continue;
        PlacedPart p;
        p.id = i + 1;
        Ptr lead = LeadOf(o);
        auto found = lead ? ids.find(lead) : ids.end();
        p.lead = !lead ? p.id : found != ids.end() ? found->second : 0;
        p.tileX = (int)x;
        p.tileY = (int)y;
        p.level = level - 1;
        p.person = IsKindOf(o, "cTSPerson");
        Ptr def = Rd<Ptr>(o + addr::object::kDefinition);
        p.type = def ? Rd<uint16_t>(def + addr::objd::kType) : 0;
        p.sub = def ? Rd<uint16_t>(def + addr::objd::kSubIndex) : 0;
        p.direction = ((Rd<int16_t>(o + addr::object::kDirection) % 8) + 8) % 8;
        p.walkThrough = (Rd<int16_t>(o + addr::objvar::kBase + 2 * addr::objvar::kFlags) & addr::objvar::kWalkThrough) != 0;
        typedef const char*(__thiscall * NameFn)(Ptr);
        const char* name = nullptr;
        if (auto f = Slot<NameFn>(o, addr::object::kSlotName)) Guarded([&] { name = f(o); });
        char head[6] = {};
        p.stairs = name && Read((Ptr)name, head, 5) && !strncmp(head, "Stair", 5);
        out.push_back(p);
    }
    return out;
}

std::vector<std::pair<int, int>> ObjectTiles(int id, const std::vector<PlacedPart>& parts) {
    std::vector<std::pair<int, int>> tiles;
    for (const PlacedPart& p : parts) {
        if (p.lead != id) continue;
        std::pair<int, int> t = {p.tileX, p.tileY};
        if (std::find(tiles.begin(), tiles.end(), t) == tiles.end()) tiles.push_back(t);
    }
    return tiles;
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
    // A service Sim is made from a template and named a moment later: a name starting with "*"
    // ("*TemplatePerson", a maid in the Sims 1 game, heard by a tester) is no name to say.
    if (!first->empty() && (*first)[0] == '*') first->clear();
    if (!last->empty() && (*last)[0] == '*') last->clear();
    return !first->empty() || !last->empty();
}

namespace {

// Person data 8 (the outfit worn) as the outfit flag the outfit data takes (0xa023f55f's jump
// table, the Lua OutfitType table): everyday 1, naked 0x80, swimwear 8, athletic 0x200, formal
// 0x20, sleepwear 0x10, try-on 0x400, maternity 0x100, everyday 2 and 3, underwear 0x40, outerwear
// 0x1000.
int OutfitFlagOf(int outfit) {
    static const int kFlags[] = {1, 0x80, 8, 0x200, 0x20, 0x10, 0x400, 1, 0x100, 2, 4, 0x40, 1, 0x1000};
    return outfit >= 0 && outfit < (int)(sizeof kFlags / sizeof kFlags[0]) ? kFlags[outfit] : 1;
}

}  // namespace

bool ObjectAttribute(int objectId, int index, int* value, bool semi) {
    Ptr o = ObjectById(objectId);
    if (!o) return false;
    int count = Rd<int16_t>(o + (semi ? addr::object::kSemiAttributeCount : addr::object::kAttributeCount));
    Ptr values = Rd<Ptr>(o + (semi ? addr::object::kSemiAttributes : addr::object::kAttributes));
    if (index < 0 || index >= count || !values) return false;
    *value = Rd<int16_t>(values + 2 * index);
    return true;
}

bool PersonDatum(int objectId, int index, int* value) {
    LotObject o;
    if (!ReadLotObject(objectId, &o) || !o.person) return false;
    Ptr iface = o.ptr + addr::person::kInterface;
    if (!IsKindOf(iface, "cTSPerson")) return false;
    typedef short(__thiscall * DataFn)(Ptr, int, int);
    auto fn = Slot<DataFn>(iface, addr::person::kSlotPersonData);
    if (!fn) return false;
    short v = 0;
    if (!Guarded([&] { v = fn(iface, index, 1); })) return false;
    *value = v;
    return true;
}

bool SimLook(int objectId, SimLooks* out) {
    LotObject o;
    if (!ReadLotObject(objectId, &o) || !o.person) return false;
    Ptr iface = o.ptr + addr::person::kInterface;
    if (!IsKindOf(iface, "cTSPerson")) return false;
    typedef short(__thiscall * DataFn)(Ptr, int, int);
    auto data = [&](int index) {
        short v = 0;
        Guarded([&] { v = Slot<DataFn>(iface, addr::person::kSlotPersonData)(iface, index, 1); });
        return (int)v;
    };
    SimLooks l;
    l.human = data(addr::person::kDataSpecies) == 0;
    l.ageDays = data(addr::person::kDataAge);
    l.youngAdult = data(addr::person::kDataYoungAdult) != 0;
    l.gender = data(addr::person::kDataGender);
    l.bodyFlags = data(addr::person::kDataBodyFlags);
    l.outfit = data(addr::person::kDataOutfit);
    int neighborId = data(addr::person::kDataNeighborId);
    // The neighbor (as NeighborName finds it), then its slot 44: only when it is the function
    // found in the disassembly.
    Ptr g = Rd<Ptr>(Va(addr::globals::kInstance));
    Ptr mgr = g ? Rd<Ptr>(g + addr::globals::kNeighborManager) : 0;
    if (l.human && mgr && neighborId > 0 && IsKindOf(mgr, "cTSNeighborManager")) {
        typedef Ptr(__thiscall * FindFn)(Ptr, int, int);
        typedef uint16_t(__thiscall * PropFn)(Ptr, uint16_t, uint16_t);
        Ptr nb = 0;
        Guarded([&] { nb = Slot<FindFn>(mgr, addr::neighbors::kSlotFind)(mgr, neighborId, 1); });
        Ptr vt = nb ? Rd<Ptr>(nb) : 0;
        Ptr fn = vt ? Rd<Ptr>(vt + 4 * addr::looks::kNeighborSlotOutfitProperty) : 0;
        if (nb && IsKindOf(nb, "cTSNeighbor") && fn == Va(addr::looks::kOutfitProperty)) {
            uint16_t flag = (uint16_t)OutfitFlagOf(l.outfit);
            auto prop = [&](int p) {
                uint16_t v = 0;
                Guarded([&] { v = ((PropFn)fn)(nb, flag, (uint16_t)p); });
                return (int)v;
            };
            l.hair = prop(addr::looks::kHair);
            l.skin = prop(addr::looks::kSkin);
            l.eyes = prop(addr::looks::kEyes);
            l.facialHair = prop(addr::looks::kFacialHair) != 0;
            l.glasses = prop(addr::looks::kGlasses) != 0;
            l.makeup = prop(addr::looks::kMakeup) != 0;
            l.fullMakeup = prop(addr::looks::kFullMakeup) != 0;
            l.hat = prop(addr::looks::kHat) != 0;
            l.jewelry = prop(addr::looks::kJewelry) != 0;
        } else {
            Log("looks: neighbor %d is %s, slot 44 %p: not read", neighborId, ClassName(nb).c_str(), (void*)fn);
        }
    }
    LogVerbose("looks of %d: age %d%s, gender %d, body %x, outfit %d, hair %d, skin %d, eyes %d", objectId, l.ageDays,
               l.youngAdult ? " (young adult)" : "", l.gender, l.bodyFlags, l.outfit, l.hair, l.skin, l.eyes);
    *out = l;
    return true;
}

bool NeighborName(int neighborId, std::string* first, std::string* last) {
    Ptr g = Rd<Ptr>(Va(addr::globals::kInstance));
    Ptr mgr = g ? Rd<Ptr>(g + addr::globals::kNeighborManager) : 0;
    if (!mgr || neighborId < 1 || !IsKindOf(mgr, "cTSNeighborManager")) return false;
    typedef Ptr(__thiscall * FindFn)(Ptr, int, int);
    typedef void(__thiscall * NameFn)(Ptr, RZString*);
    RZString a = {}, b = {};
    a.vtable = b.vtable = Va(addr::person::kRZStringVtable);
    a.cap = b.cap = 15;
    bool ok = Guarded([&] {
        Ptr nb = Slot<FindFn>(mgr, addr::neighbors::kSlotFind)(mgr, neighborId, 1);
        Ptr sel = nb ? Rd<Ptr>(nb + addr::neighbors::kSelector) : 0;
        if (!sel || !IsKindOf(sel, "cTSObjectSelector")) return;
        Slot<NameFn>(sel, addr::person::kNeighborSlotFirstName)(sel, &a);
        if (last) Slot<NameFn>(sel, addr::person::kNeighborSlotLastName)(sel, &b);
    });
    if (!ok) return false;
    *first = Text(a);
    if (last) *last = Text(b);
    if (!first->empty() && (*first)[0] == '*') first->clear();  // a template's label
    if (last && !last->empty() && (*last)[0] == '*') last->clear();
    return !first->empty();
}

Ptr WorldDB() {
    static Ptr s_world = 0;
    if (!s_world) {
        Ptr fw = Va(addr::kFramework);
        typedef bool(__thiscall * GetServiceFn)(Ptr, uint32_t, uint32_t, void**);
        void* out = nullptr;
        bool ok = false;
        if (auto get = Slot<GetServiceFn>(fw, addr::kFrameworkGetSystemService))
            Guarded([&] { ok = get(fw, addr::world::kServiceId, addr::world::kIid, &out); });
        if (!ok || !out) return 0;
        s_world = (Ptr)out;  // a service: it outlives the mod's use (the reference is kept)
    }
    return s_world;
}

int RoomAt(float x, float y, int level) {
    Ptr s_world = WorldDB();
    if (!s_world) return -1;
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

bool LightAt(float x, float y, int level, float* light) {
    Ptr sg = Rd<Ptr>(Va(addr::light::kSceneGraph));
    if (!sg) sg = Rd<Ptr>(Va(addr::light::kSceneGraphCached));
    Ptr lm = sg ? Rd<Ptr>(sg + addr::light::kLightingManager) : 0;
    if (!lm || Rd<Ptr>(lm) != Va(addr::light::kLightingManagerVtable)) return false;
    struct Spot {
        float x, y;
        int level;
    } spot = {x, y, level};
    typedef float(__thiscall * LightFn)(Ptr, Spot*);
    float v = -1;
    if (!Guarded([&] { v = Slot<LightFn>(lm, addr::light::kSlotLightAt)(lm, &spot); })) return false;
    if (!(v >= 0 && v < 100)) return false;
    *light = v;
    return true;
}

void LightThresholds(float* dark, float* lit) {
    *dark = 0.55f;
    *lit = 0.9f;
    Ptr g = Rd<Ptr>(Va(addr::globals::kInstance));
    Ptr env = g ? Rd<Ptr>(g + addr::light::kGlobalsEnvironment) : 0;
    if (!env) return;
    float d = Rd<float>(env + addr::light::kEnvDark, -1), l = Rd<float>(env + addr::light::kEnvLit, -1);
    if (d >= 0 && l > d && l < 10) {
        *dark = d;
        *lit = l;
    }
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

int QueueIconObject(Ptr icon) {
    Ptr interaction = Rd<Ptr>(icon + addr::interaction::kIconInteraction);
    return IsKindOf(interaction, "cTSInteraction") ? Rd<int16_t>(interaction + addr::interaction::kObjectId) : 0;
}

namespace {

// The lot's camera: no path to it from the game's globals was found, so it is found by its vtable
// in the process's private memory once a lot (a fraction of a second), and kept while it is alive.
Ptr FindCamera(uint32_t vtable) {
    ULONGLONG t0 = GetTickCount64();
    uint32_t wanted = (uint32_t)Va(vtable);
    MEMORY_BASIC_INFORMATION mbi;
    Ptr best = 0;
    int bestSteps = 0;
    for (Ptr p = 0x10000; p < 0x7fff0000; p += mbi.RegionSize) {
        if (!VirtualQuery((void*)p, &mbi, sizeof mbi)) break;
        p = (Ptr)mbi.BaseAddress;
        if (mbi.State != MEM_COMMIT || !(mbi.Protect & (PAGE_READWRITE | PAGE_WRITECOPY)) || (mbi.Protect & PAGE_GUARD)) continue;
        const uint32_t* w = (const uint32_t*)mbi.BaseAddress;
        size_t n = mbi.RegionSize / 4;
        for (size_t i = 0; i < n; i++) {
            if (w[i] != wanted) continue;
            Ptr camera = (Ptr)&w[i];
            int steps = (int)((Rd<Ptr>(camera + addr::camera::kZoomSteps + 4) - Rd<Ptr>(camera + addr::camera::kZoomSteps)) / 4);
            if (steps <= bestSteps || steps > 32) continue;
            bestSteps = steps;
            best = camera;
        }
    }
    Log("camera: %p, %d zoom steps, %llu ms", (void*)best, bestSteps, GetTickCount64() - t0);
    return best;
}

}  // namespace

namespace {

// The lot's cCameraLive, found once by its vtable (and again when it goes away).
Ptr LiveCamera() {
    static Ptr s_camera = 0;
    static ULONGLONG s_triedAt = 0;
    uint32_t vtable = addr::camera::kLiveVtable;
    if (!s_camera || Rd<uint32_t>(s_camera) != (uint32_t)Va(vtable)) {
        s_camera = 0;
        ULONGLONG now = GetTickCount64();
        if (s_triedAt && now - s_triedAt < 10000) return 0;
        s_triedAt = now;
        s_camera = FindCamera(vtable);
    }
    return s_camera;
}

}  // namespace

bool CenterCamera(float x, float y, bool jump) {
    Ptr camera = LiveCamera();
    if (!camera) return false;
    Ptr vbtable = Rd<Ptr>(camera + addr::camera::kVbtable);
    if (!vbtable || Rd<int>(vbtable + 4) != addr::camera::kEventBaseOffset) {
        Log("camera: unexpected layout (vbtable %p)", (void*)vbtable);
        return false;
    }
    Ptr events = camera + addr::camera::kEventInterface;
    struct VectorEvent {
        Ptr vtable;
        float x, y, z;
    } e = {Va(addr::camera::kVectorEventVtable), x, y, Rd<float>(camera + addr::camera::kLookAt + 8)};
    typedef bool(__thiscall * HandleFn)(Ptr, uint32_t, uint32_t, VectorEvent*);
    Ptr vt = Rd<Ptr>(events);
    if (!InImage(vt)) return false;
    auto handle = (HandleFn)Rd<Ptr>(vt + 4 * 4);
    bool ok = false;
    Guarded([&] { ok = handle(events, jump ? addr::camera::kEventJump : addr::camera::kEventGlide, 0, &e); });
    LogVerbose("camera: look at %.2f,%.2f,%.2f (%s): %d", x, y, e.z, jump ? "jump" : "glide", ok ? 1 : 0);
    return true;
}

bool CameraView(int* zoom, int* steps, int* compass) {
    Ptr camera = LiveCamera();
    if (!camera) return false;
    Ptr begin = Rd<Ptr>(camera + addr::camera::kZoomSteps), end = Rd<Ptr>(camera + addr::camera::kZoomSteps + 4);
    int n = (int)((end - begin) / 4);
    if (n < 1 || n > 32) return false;
    float target = Rd<float>(camera + addr::camera::kZoomTarget);
    int nearest = 0;
    float best = 1e9f;
    for (int i = 0; i < n; i++) {
        float d = std::fabs(Rd<float>(begin + i * 4) - target);
        if (d < best) {
            best = d;
            nearest = i;
        }
    }
    *zoom = nearest;
    *steps = n;
    float qx = Rd<float>(camera + addr::camera::kOrientation), qy = Rd<float>(camera + addr::camera::kOrientation + 4);
    float qz = Rd<float>(camera + addr::camera::kOrientation + 8), qw = Rd<float>(camera + addr::camera::kOrientation + 12);
    // The view's direction on the lot (the world's x and y are the tiles': x east, y south).
    float fx = 2 * (qx * qy - qw * qz), fy = 1 - 2 * (qx * qx + qz * qz);
    double degrees = std::atan2((double)fx, (double)-fy) * 180.0 / 3.14159265358979;  // 0 north, 90 east
    *compass = ((int)std::lround(degrees / 45.0) % 8 + 8) % 8;
    return true;
}

bool PostGameMessage(uint32_t type, uint32_t data1) {
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
        Slot<SetFn>(msg, addr::message::kSlotSetType)(msg, type);
        Slot<SetFn>(msg, addr::message::kSlotSetData1)(msg, data1);
    });
    Ptr server = Rd<Ptr>(Va(addr::message::kServer));
    typedef bool(__thiscall * PostFn)(Ptr, Ptr, uint32_t);
    bool posted = false;
    if (ok && server) ok = Guarded([&] { posted = Slot<PostFn>(server, addr::message::kSlotPost)(server, msg, 0); });
    Guarded([&] { Slot<RefFn>(msg, 2)(msg); });  // Release: the server holds its own reference
    LogVerbose("message %08x (%u): %s", type, data1, ok ? (posted ? "posted" : "refused") : "crashed");
    return ok && posted;
}

bool ShowInteractions(int objectId) {
    bool posted = PostGameMessage(addr::message::kShowPieMenu, (uint32_t)objectId);
    Log("interaction menu for object %d: %s", objectId, posted ? "posted" : "not posted");
    return posted;
}

}  // namespace game
