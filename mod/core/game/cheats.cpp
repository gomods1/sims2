#include "cheats.h"

#include <windows.h>

#include "../util/log.h"
#include "addresses.h"
#include "buy.h"
#include "gz.h"
#include "lotgrid.h"
#include "rtti.h"
#include "memory.h"
#include "mouse.h"
#include "objects.h"
#include "neighborhood.h"

#include <cstdio>
#include <cstring>
#include <map>
#include <string>

namespace game {

bool g_screenDumpRequested = false;
bool g_doorCheckRequested = false;
bool g_reachCheckRequested = false;
bool g_shutInCheckRequested = false;
namespace {

template <class Fn>
Fn Slot(Ptr obj, int slot) {
    Ptr vt = Rd<Ptr>(obj);
    return InImage(vt) ? (Fn)Rd<Ptr>(vt + slot * 4) : nullptr;
}

Ptr CheatSystem() {
    Ptr fw = Va(addr::kFramework);
    typedef bool(__thiscall * GetServiceFn)(Ptr, uint32_t, uint32_t, void**);
    auto get = Slot<GetServiceFn>(fw, addr::kFrameworkGetSystemService);
    void* out = nullptr;
    bool ok = false;
    if (!get || !Guarded([&] { ok = get(fw, addr::cheat::kServiceId, addr::cheat::kIid, &out); }) || !ok) return 0;
    return (Ptr)out;
}

ULONGLONG g_lastPoll = 0;

// Development commands (tools/game.py cheat "!name args"): calls into the game to test them.
void DevCommand(const std::string& line) {
    char name[64] = {};
    int a = 0, b = 0, c = 0, d = 0, e = 0;
    int n = sscanf_s(line.c_str(), "%63s %d %d %d %d %d", name, (unsigned)sizeof name, &a, &b, &c, &d, &e);
    Log("dev command %s (%d args)", name, n - 1);
    if (!strcmp(name, "lotinfo")) ShowLotInfo(a, b, c);
    else if (!strcmp(name, "click")) MouseClick(a, b, c != 0);
    else if (!strcmp(name, "move")) MouseMove(a, b);
    else if (!strcmp(name, "release")) {
        MouseRelease();
        SetPickOverride(false);
        ClearObjectPick();
    } else if (!strcmp(name, "ground")) {
        // !ground x y level: a click on that tile (nothing else under the cursor)
        SetPickOverride(true, a + 0.5f, b + 0.5f, c);
        ForceNoObjectPick();
        MouseClick(1280, 640);
    }
    else if (!strcmp(name, "pie")) ShowInteractions(a);
    else if (!strcmp(name, "fam")) {
        for (int id : FamilySimIds()) {
            std::string first, last;
            SimName(id, &first, &last);
            Log("  family sim %d: %s %s", id, first.c_str(), last.c_str());
        }
    }
    else if (!strcmp(name, "nb")) {
        // !nb id [count]: neighbor names from id on
        for (int id = a; id < a + (b ? b : 1); id++) {
            std::string first, last;
            if (NeighborName(id, &first, &last)) Log("  neighbor %d: %s %s", id, first.c_str(), last.c_str());
        }
    }
    else if (!strcmp(name, "movein")) ClickLotWithTool(a);
    else if (!strcmp(name, "screen")) g_screenDumpRequested = true;
    else if (!strcmp(name, "doors")) g_doorCheckRequested = true;
    else if (!strcmp(name, "reach")) g_reachCheckRequested = true;
    else if (!strcmp(name, "objd")) {
        // !objd <id>: the object's OBJD u16 fields 0..119, ten a line
        for (int row = 0; row < 120; row += 10) {
            std::string line;
            for (int i = row; i < row + 10; i++) line += " " + std::to_string(ObjdField(a, i));
            Log("  objd %d [%d]:%s", a, row, line.c_str());
        }
    }
    else if (!strcmp(name, "lots")) {
        // !lots: the neighborhood's lots (id, type, size, flags, name)
        for (const LotEntry& l : Lots())
            Log("  lot %d type %d at %d,%d size %dx%d flags %08x %s", l.id, l.type, l.x, l.y, l.width, l.depth, l.flags,
                l.name.c_str());
    }
    else if (!strcmp(name, "pd")) {
        // !pd <index>: that person data of every Sim on the lot
        for (const LotObject& o : LotObjects()) {
            int v = 0;
            if (o.person && PersonDatum(o.id, a, &v)) Log("  pd %d of %d %s: %d", a, o.id, o.name.c_str(), v);
        }
    }
    else if (!strcmp(name, "msg")) {
        // !msg <type hex> <data1>: posts a message to the game's message server
        unsigned type = 0;
        int data = 0;
        sscanf_s(line.c_str(), "%*s %x %d", &type, &data);
        Log("message %08x %d: %d", type, data, PostGameMessage(type, (uint32_t)data) ? 1 : 0);
    }
    else if (!strcmp(name, "camera")) Log("camera to %d,%d: %d", a, b, CenterCamera(a + 0.5f, b + 0.5f, c != 0) ? 1 : 0);
    else if (!strcmp(name, "slots")) {
        // !slots <object id>: where Sims stand or sit to use it (its routing slots), what it stands on
        LotObject o;
        bool known = ReadLotObject(a, &o);
        Log("slots of %d (%s) at %.2f,%.2f direction %d, container %d:", a, known ? o.name.c_str() : "?", o.x, o.y,
            o.direction, ContainerOf(a));
        for (const UseSpot& u : UseSpots(a))
            Log("  spot %.2f,%.2f tile %d,%d%s", u.x, u.y, u.tileX, u.tileY, u.sitting ? " sitting" : "");
    }
    else if (!strcmp(name, "look")) {
        // !look <object id, 0 the selected Sim>: what the Sim looks like, as read
        SimLooks l;
        int id = a ? a : SelectedSimId();
        if (!SimLook(id, &l)) Log("look of %d: no Sim", id);
        else
            Log("look of %d: human %d, age %d%s, gender %d, body %x, outfit %d, hair %d, skin %d, eyes %d, facial hair %d, "
                "glasses %d, makeup %d/%d, hat %d, jewelry %d",
                id, l.human, l.ageDays, l.youngAdult ? " young adult" : "", l.gender, l.bodyFlags, l.outfit, l.hair, l.skin,
                l.eyes, l.facialHair, l.glasses, l.makeup, l.fullMakeup, l.hat, l.jewelry);
    }

    else if (!strcmp(name, "shutin")) g_shutInCheckRequested = true;
    else if (!strcmp(name, "press")) Press((Ptr)(unsigned)a);  // !press <window pointer in decimal>
    else if (!strcmp(name, "wall")) DragTool(CurrentTool(), (float)a, (float)b, (float)c, (float)d, e, false);
    else if (!strcmp(name, "families")) {
        for (const FamilyEntry& f : Families())
            Log("  family %d \"%s\" lot %d members %d funds %d", f.id, f.name.c_str(), f.lot, f.members, f.funds);
    } else if (!strcmp(name, "lots")) {
        for (const LotEntry& l : Lots())
            Log("  lot %d \"%s\" type %d size %dx%d flags %x", l.id, l.name.c_str(), l.type, l.width, l.depth, l.flags);
    }
    else if (!strcmp(name, "check")) {
        // !check x y n: time n spot checks along x from (x, y)
        LARGE_INTEGER f, t0, t1;
        QueryPerformanceFrequency(&f);
        QueryPerformanceCounter(&t0);
        std::string verdicts;
        for (int i = 0; i < (c ? c : 1); i++)
            verdicts += std::to_string(CheckSpot(CurrentTool(), a + i + 0.5f, b + 0.5f, 1)) + " ";
        QueryPerformanceCounter(&t1);
        Log("check: tool %p (%s): %s in %.1f ms", (void*)CurrentTool(), (ClassName(CurrentTool()) + " mode " + CurrentModeClass()).c_str(), verdicts.c_str(), (t1.QuadPart - t0.QuadPart) * 1000.0 / f.QuadPart);
    }
    else if (!strcmp(name, "world")) {
        Ptr w = WorldDB();
        Log("world iface %p (%s), primary %p", (void*)w, ClassName(w).c_str(), (void*)(w - 0x1c));
        for (int off = 0; off <= 0xd0; off += 4) {
            Ptr v = Rd<Ptr>(w - 0x1c + off);
            Log("  +%02x %08x %s", off, (unsigned)v, ClassName(v).c_str());
        }
    }
    else if (!strcmp(name, "entry")) {
        // !entry <guid in decimal>: the catalog folder's entry for a GUID (products, patterns)
        Ptr g = Rd<Ptr>(Va(addr::globals::kInstance));
        typedef Ptr(__thiscall * GetFn)(Ptr);
        typedef Ptr(__thiscall * ByGuidFn)(Ptr, uint32_t);
        Ptr entry = 0;
        Guarded([&] {
            Ptr folder = Slot<GetFn>(g, addr::catalog::kGlobalsSlotFolder)(g);
            if (folder) entry = Slot<ByGuidFn>(folder, addr::catalog::kFolderSlotByGuid)(folder, (uint32_t)a);
        });
        Log("entry for %08x: %p %s", (unsigned)a, (void*)entry, ClassName(entry).c_str());
        for (int i = 0; entry && i < (b ? b : 64); i++) {
            Ptr v = Rd<Ptr>(entry + i * 4);
            Log("  +%03x: %08x %s", i * 4, (unsigned)v, ClassName(v).c_str());
        }
    }
    else if (!strcmp(name, "probe")) {
        // !probe x y level: the lot grid's reads at a tile
        int id = FloorPatternAt(c, a, b);
        float h[4] = {};
        TerrainHeight(a, b, &h[0]);
        TerrainHeight(a + 1, b, &h[1]);
        TerrainHeight(a, b + 1, &h[2]);
        TerrainHeight(a + 1, b + 1, &h[3]);
        Log("probe %d,%d level %d: room %d, floor %d \"%s\" (%s) surface %d, water %d, heights %.2f %.2f %.2f %.2f", a, b, c,
            RoomAt(a + 0.5f, b + 0.5f, c), id, FloorPatternName(id).c_str(), FloorName(c, a, b).c_str(), (int)SurfaceAt(c, a, b), WaterOnTile(a, b) ? 1 : 0,
            h[0], h[1], h[2], h[3]);
        const int dx[] = {0, 1, 0, -1}, dy[] = {-1, 0, 1, 0};
        const char* side[] = {"north", "east", "south", "west"};
        for (int s = 0; s < 4; s++) {
            std::string layers;
            for (int l = 0; l < 8; l++) layers += WallBetween(c, a, b, a + dx[s], b + dy[s], l) ? '1' : '0';
            Log("  %s: layers 0-7 %s", side[s], layers.c_str());
        }
    }
    else if (!strcmp(name, "walls")) {
        // !walls level: every east and south edge of the lot by its layer signature, with a sample
        int w = 0, h = 0;
        LotSize(&w, &h);
        std::map<std::string, std::pair<int, std::string>> kinds;
        for (int x = 0; x < w; x++)
            for (int y = 0; y < h; y++)
                for (int s = 0; s < 2; s++) {
                    int nx = x + (s == 0), ny = y + (s == 1);
                    std::string sig;
                    for (int l = 0; l < 8; l++) sig += WallBetween(a, x, y, nx, ny, l) ? '1' : '0';
                    if (sig == "00000000") continue;
                    auto& k = kinds[sig];
                    if (!k.first++) k.second = std::to_string(x) + "," + std::to_string(y) + (s ? " south" : " east");
                }
        for (auto& [sig, k] : kinds) Log("  walls level %d: %s x%d, e.g. %s", a, sig.c_str(), k.first, k.second.c_str());
    }
    else if (!strcmp(name, "water")) {
        typedef bool(__cdecl * WaterFn)(const int*);
        int t1[3] = {a, b, c}, t2[3] = {b, a, c};
        bool r1 = false, r2 = false;
        Guarded([&] { r1 = ((WaterFn)Va(addr::grid::kWaterOnTile))(t1); });
        Guarded([&] { r2 = ((WaterFn)Va(addr::grid::kWaterOnTile))(t2); });
        Log("water %d,%d,%d: %d, swapped %d; pattern below %d", a, b, c, r1, r2, FloorPatternAt(-1, a, b));
    }
    else if (!strcmp(name, "parts")) {
        // !parts x y level: what stands on a tile, and the game's tiles for each
        for (const PlacedPart& p : PlacedParts()) {
            if (p.tileX != a || p.tileY != b || p.level != c) continue;
            LotObject o;
            ReadLotObject(p.id, &o);
            Log("  part %d at %08x lead %d type %d sub %04x dir %d tile %d,%d \"%s\" (%s)", p.id, (unsigned)o.ptr, p.lead, p.type,
                p.sub, p.direction, p.tileX, p.tileY, o.name.c_str(), ClassName(o.ptr).c_str());
            for (int off = 0; off < 0x400 && d; off += 4) {
                Ptr v = Rd<Ptr>(o.ptr + off);
                std::string cn = ClassName(v);
                if (cn.find("cTSObject") != std::string::npos || cn.find("cEdith") != std::string::npos ||
                    cn.find("cTSPerson") != std::string::npos)
                    Log("    +%03x -> %08x %s", off, (unsigned)v, cn.c_str());
            }
        }
    }
    else if (!strcmp(name, "dump")) {
        // !dump <address in decimal> <dwords>
        for (int i = 0; i < (b ? b : 16); i++) {
            Ptr v = Rd<Ptr>((Ptr)(unsigned)a + i * 4);
            Log("  %08x +%03x: %08x %s", (unsigned)a + i * 4, i * 4, (unsigned)v, ClassName(v).c_str());
        }
    }
    else if (!strcmp(name, "rooms")) {
        for (const LotObject& o : LotObjects())
            if (o.x >= 0) Log("  object %d \"%s\" floor %d room %d flags %x type %d at %.1f,%.1f", o.id, o.catalogName.c_str(), o.level, o.room, o.roomFlags, o.type, o.x, o.y);
    }
    else if (!strcmp(name, "motives")) {
        int sim = a ? a : SelectedSimId();
        for (int i = 0; i < 16; i++) {
            float v = 0;
            if (SimMotive(sim, i, &v)) Log("  motive %d of %d: %.1f", i, sim, v);
        }
    }
    else if (!strcmp(name, "objects")) {
        Log("selected Sim: object %d", SelectedSimId());
        for (const LotObject& o : LotObjects()) {
            std::string f, l;
            if (o.person && SimName(o.id, &f, &l)) Log("  Sim %d: %s %s", o.id, f.c_str(), l.c_str());
        }
        for (const LotObject& o : LotObjects())
            if (o.x >= 0)
                Log("  object %d %s \"%s\" \"%s\" guid %08x $%d at %.1f, %.1f level %d", o.id, o.person ? "Sim" : "",
                    o.name.c_str(), o.catalogName.c_str(), o.guid, o.price, o.x, o.y, o.level);
    }
}

}  // namespace

bool RunCheat(const std::string& line) {
    Ptr cs = CheatSystem();
    if (!cs) {
        Log("cheat system not found");
        return false;
    }
    typedef bool(__thiscall * ExecFn)(Ptr, const char*);
    typedef uint32_t(__thiscall * ReleaseFn)(Ptr);
    auto exec = Slot<ExecFn>(cs, addr::cheat::kSlotExecute);
    bool result = false;
    bool ran = exec && Guarded([&] { result = exec(cs, line.c_str()); });
    if (auto release = Slot<ReleaseFn>(cs, 2)) Guarded([&] { release(cs); });
    Log("cheat \"%s\": %s", line.c_str(), !ran ? "crashed" : result ? "ok" : "refused");
    return ran && result;
}

void PollCheatRequest(const wchar_t* modDir) {
    ULONGLONG now = GetTickCount64();
    if (now - g_lastPoll < 300) return;
    g_lastPoll = now;
    std::wstring path = std::wstring(modDir) + L"\\cheat_request.txt";
    HANDLE f = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
    static int s_logged = 0;
    if (s_logged++ < 1) Log("cheat requests from %ls (%s)", path.c_str(), f == INVALID_HANDLE_VALUE ? "none now" : "found");
    if (f == INVALID_HANDLE_VALUE) return;
    char buf[1024] = {};
    DWORD got = 0;
    ReadFile(f, buf, sizeof buf - 1, &got, nullptr);
    CloseHandle(f);
    DeleteFileW(path.c_str());
    std::string line(buf);
    while (!line.empty() && (line.back() == '\n' || line.back() == '\r')) line.pop_back();
    if (line.empty()) return;
    if (line[0] == '!') DevCommand(line.substr(1));
    else RunCheat(line);
}

}  // namespace game
