#include "simantics.h"

#include <cstring>
#include <mutex>

#include "../util/log.h"
#include "addresses.h"
#include "hooks.h"
#include "rtti.h"

namespace game {
namespace {

std::mutex g_lock;
Balloon g_balloons[32];
int g_count = 0;

// The thread's variable accessor (the interface at +4, slot 7): (scope, data, 0, 0, 0, 0, 0) -> value.
short ReadVariable(Ptr thread, int scope, int data) {
    typedef short(__thiscall * VarFn)(Ptr, int, int, int, int, int, int, int);
    Ptr iface = thread + 4;
    Ptr vt = Rd<Ptr>(iface);
    if (!InImage(vt)) return 0;
    auto fn = (VarFn)Rd<Ptr>(vt + 0x1c);
    short value = 0;
    Guarded([&] { value = fn(iface, scope, data, 0, 0, 0, 0, 0); });
    return value;
}

// The object's slot 19 (0x4c): Temp n (the effect when flag 8 says so, an icon's GUID in 4/5).
short ReadTemp(Ptr thread, int n) {
    typedef short(__thiscall * TempFn)(Ptr, int);
    Ptr vt = Rd<Ptr>(thread);
    if (!InImage(vt)) return -1;
    auto fn = (TempFn)Rd<Ptr>(vt + 0x4c);
    short value = -1;
    Guarded([&] { value = fn(thread, n); });
    return value;
}

// The groups whose STR# 0x95 names a balloon's sheet, as the handler finds them (0xa0580703 and
// 0xa0581a11): the thread's +0x40 slot 24 is the object folder (cTSObjectFolder), whose slot 14
// gives the cTSObjectSelector of the tree's key (the behavior's slot 38, its +0x20: a GUID). The
// selector's slot 18 (+0x18) holds the object's groups (global +0xc, semi-global +0x14, own +0x1c;
// slot 11 reads the own one) and its slot 70 (+0x10) the semi-global's file reference (slot 11:
// its +4, the group). The game passes (1, 0x2026960b) to slot 11, which ignores them: called here
// without (a getter's plain ret would unbalance the stack). Read live 2026-11-08: Carlos's thought
// gave his own group 0x7fedfe16, the semi-global 0x7f01ec29 (motives-thought-02 at sheet 0). The
// food objects' sheet (counters, stoves...) is only in their semi-global group.
void SheetGroups(Ptr thread, Ptr behavior, uint32_t* own, uint32_t* semi) {
    typedef Ptr(__thiscall * GetFn)(Ptr);
    typedef Ptr(__thiscall * LookupFn)(Ptr, Ptr);
    auto vcall = [](Ptr self, int offset) -> Ptr {
        Ptr vt = self ? Rd<Ptr>(self) : 0;
        if (!InImage(vt)) return 0;
        return ((GetFn)Rd<Ptr>(vt + offset))(self);
    };
    Guarded([&] {
        Ptr folder = vcall(Rd<Ptr>(thread + 0x40), 0x60);
        Ptr key = vcall(behavior, 0x98);
        Ptr vt = folder ? Rd<Ptr>(folder) : 0;
        if (!InImage(vt)) return;
        Ptr selector = ((LookupFn)Rd<Ptr>(vt + 0x38))(folder, key);
        if (!selector) return;
        *own = (uint32_t)vcall(vcall(selector, 0x48), 0x2c);
        *semi = (uint32_t)vcall(vcall(selector, 0x118), 0x2c);
    });
}

// Opcode 0x70 (the balloon): thiscall (thread, stack frame, operands).
typedef int(__fastcall* BalloonFn)(Ptr thread, void* edx, Ptr frame, const uint8_t* operands);
BalloonFn g_balloon = nullptr;
int g_frameDumps = 0;
int __fastcall BalloonDetour(Ptr thread, void* edx, Ptr frame, const uint8_t* operands) {
    if (g_frameDumps < 0) {
        g_frameDumps++;
        std::string line = "frame " + ClassName(frame) + ":";
        for (int i = 0; i < 0x40; i += 4) {
            Ptr v = Rd<Ptr>(frame + i);
            char buf[96];
            sprintf_s(buf, " +%x=%08x(%s)", i, v, ClassName(v).c_str());
            line += buf;
        }
        Log("%s", line.c_str());
        line = "thread " + ClassName(thread) + ":";
        for (int i = 0; i < 0x60; i += 4) {
            Ptr v = Rd<Ptr>(thread + i);
            char buf[96];
            sprintf_s(buf, " +%x=%08x(%s)", i, v, ClassName(v).c_str());
            line += buf;
        }
        Log("%s", line.c_str());
    }
    Balloon b;
    if (Read((Ptr)operands, b.operands, sizeof b.operands)) {
        const uint8_t* o = b.operands;
        Ptr behavior = Rd<Ptr>(frame + 0x20);  // cTSTreeStackElem: the tree (cTSBehavior: +0xc its group)
        if (behavior) b.group = Rd<uint32_t>(behavior + 0xc);
        b.objectId = ReadVariable(thread, o[5], *(uint16_t*)(o + 6));
        // Who runs the tree and its stack object (scope 10, "Stack Object ID"): in a conversation,
        // the two Sims (the balloon is over either).
        b.runner = ReadVariable(thread, 3, 11);
        b.stackObject = ReadVariable(thread, 10, 0);
        b.mode = o[4];
        b.flags = *(uint16_t*)(o + 0xe);
        if (b.flags & 1) b.effect = (b.flags & 8) ? ReadTemp(thread, 0) : *(int16_t*)(o + 8);
        // The icon, in the handler's order: 0x1000 other; 0x4 an object's thumbnail (0x100 the
        // running object, 0x400 a GUID in Temps 4/5, else the object id in the icon variable);
        // 0x20 a sheet cell; 0x10 a Sim (neighbor id).
        if (b.flags & 0x1000) {
        } else if (b.flags & 0x4) {
            if (b.flags & 0x100)
                b.iconThing = ReadVariable(thread, 3, 11);  // My object's data 11: its id
            else if (b.flags & 0x400)
                b.iconGuid = (uint16_t)ReadTemp(thread, 4) | (uint32_t)(uint16_t)ReadTemp(thread, 5) << 16;
            else
                b.iconThing = ReadVariable(thread, o[0x10], *(uint16_t*)(o + 0x11));
        } else if (b.flags & 0x30) {
            b.iconValue = ReadVariable(thread, o[0x10], *(uint16_t*)(o + 0x11));
            if (b.flags & 0x20) {
                b.sheet = o[0x13];
                if (b.mode == 0 || b.mode == 0xf) SheetGroups(thread, behavior, &b.sheetGroup, &b.semiGroup);
            } else {
                b.iconObject = true;
            }
        }
        std::lock_guard<std::mutex> hold(g_lock);
        if (g_count < 32) g_balloons[g_count++] = b;
    }
    return g_balloon(thread, edx, frame, operands);
}

// cTSUIPaintingService slot 6: a balloon's thumbnail of a thing (thiscall: name builder, thing, a, b).
typedef int(__fastcall* ThumbFn)(Ptr self, void* edx, Ptr builder, Ptr thing, int a, int b);
ThumbFn g_thumb = nullptr;
int __fastcall ThumbDetour(Ptr self, void* edx, Ptr builder, Ptr thing, int a, int b) {
    uint32_t id = 0;
    if (thing) {
        typedef uint32_t(__thiscall * IdFn)(Ptr);
        Ptr vt = Rd<Ptr>(thing);
        if (InImage(vt)) Guarded([&] { id = ((IdFn)Rd<Ptr>(vt + 0x70))(thing); });
    }
    LogVerbose("thumbnail of %s %08x (%d, %d)", ClassName(thing).c_str(), id, a, b);
    return g_thumb(self, edx, builder, thing, a, b);
}

}  // namespace

void InitSimAnticsHooks() {
    HookGame("SimAntics::Balloon", addr::simantics::kBalloon, (void*)BalloonDetour, (void**)&g_balloon);
    HookGame("cTSUIPaintingService::BalloonThumbnail", addr::simantics::kBalloonThumbnail, (void*)ThumbDetour, (void**)&g_thumb);
}

int TakeBalloons(Balloon* out, int max) {
    std::lock_guard<std::mutex> hold(g_lock);
    int n = g_count < max ? g_count : max;
    memcpy(out, g_balloons, n * sizeof(Balloon));
    g_count = 0;
    return n;
}

}  // namespace game
