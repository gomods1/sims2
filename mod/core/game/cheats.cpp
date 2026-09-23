#include "cheats.h"

#include <windows.h>

#include "../util/log.h"
#include "addresses.h"
#include "memory.h"
#include "mouse.h"
#include "objects.h"
#include "neighborhood.h"

#include <cstdio>
#include <cstring>

namespace game {
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
    int a = 0, b = 0, c = 0;
    int n = sscanf_s(line.c_str(), "%63s %d %d %d", name, (unsigned)sizeof name, &a, &b, &c);
    Log("dev command %s (%d args)", name, n - 1);
    if (!strcmp(name, "lotinfo")) ShowLotInfo(a, b, c);
    else if (!strcmp(name, "click")) MouseClick(a, b, c != 0);
    else if (!strcmp(name, "move")) MouseMove(a, b);
    else if (!strcmp(name, "release")) MouseRelease();
    else if (!strcmp(name, "pie")) ShowInteractions(a);
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
