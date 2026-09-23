#include "hooks.h"

#include <cstdio>
#include <cstring>
#include <string>

#include "../util/log.h"
#include "MinHook.h"
#include "addresses.h"
#include "buy.h"
#include "neighborhood.h"
#include "gz.h"
#include "rtti.h"
#include "simantics.h"

namespace game {
namespace {

bool g_minhookReady = false;
std::vector<Ptr> g_modals;

// ---- cGZWinMgrW32::DoModalWin ---------------------------------------------------------------
// The detour only records the window and jumps on: a dialog's loop runs inside the call, and a
// frame of this DLL left on the stack there would crash when a rebuilt core is hot-reloaded.
// Closed modals are dropped by ModalStack() against the manager's nesting count.
void* g_doModal = nullptr;

// Return addresses on the stack that point into the game (a rough call chain for the log).
std::string GameReturnAddresses(const void* stack, int words, int max) {
    std::string out;
    int found = 0;
    for (int i = 0; i < words && found < max; i++) {
        Ptr v = Rd<Ptr>((Ptr)stack + i * 4);
        if (!InImage(v)) continue;
        // A return address follows a call: E8 rel32 (5 bytes back) or FF /2 (2..6 bytes back).
        uint8_t b5 = Rd<uint8_t>(v - 5), b2 = Rd<uint8_t>(v - 2), b3 = Rd<uint8_t>(v - 3), b6 = Rd<uint8_t>(v - 6);
        if (b5 != 0xE8 && b2 != 0xFF && b3 != 0xFF && b6 != 0xFF) continue;
        char buf[16];
        sprintf_s(buf, " %08x", (unsigned)(v - Va(0xa0000000) + 0xa0000000));
        out += buf;
        found++;
    }
    return out;
}

void __stdcall RecordModal(Ptr win, Ptr caller) {
    g_modals.push_back(win);
    int marker = 0;
    Log("modal window %p (%s) opens, stack:%s", (void*)win, ClassName(win).c_str(),
        GameReturnAddresses(&marker, 400, 12).c_str());
    (void)caller;
}

__declspec(naked) void DoModalDetour() {
    __asm {
        push ecx
        push dword ptr [esp + 4]
        push dword ptr [esp + 0xc]
        call RecordModal
        pop ecx
        jmp dword ptr [g_doModal]
    }
}

// ---- GetKeyState / GetAsyncKeyState ----------------------------------------------------------
// The game reads Ctrl, Shift and Alt from the keyboard state, which posted test keys
// (tools/keys.py) do not change. While the mod has seen such a posted modifier go down, the
// game is told it is held too. Real keys are untouched.
typedef SHORT(WINAPI* KeyStateFn)(int);
KeyStateFn g_getAsyncKeyState = nullptr;
KeyStateFn g_getKeyState = nullptr;
bool g_syntheticHeld[256] = {};

bool SyntheticHeld(int vk) {
    if (vk < 0 || vk > 255) return false;
    if (g_syntheticHeld[vk]) return true;
    switch (vk) {
    case VK_LCONTROL: case VK_RCONTROL: return g_syntheticHeld[VK_CONTROL];
    case VK_LSHIFT: case VK_RSHIFT: return g_syntheticHeld[VK_SHIFT];
    case VK_LMENU: case VK_RMENU: return g_syntheticHeld[VK_MENU];
    }
    return false;
}

SHORT WINAPI GetAsyncKeyStateDetour(int vk) {
    SHORT r = g_getAsyncKeyState(vk);
    return SyntheticHeld(vk) ? (SHORT)(r | 0x8000) : r;
}

SHORT WINAPI GetKeyStateDetour(int vk) {
    SHORT r = g_getKeyState(vk);
    return SyntheticHeld(vk) ? (SHORT)(r | 0xff80) : r;
}

void HookApi(const char* name, void* detour, KeyStateFn* original) {
    void* target = (void*)GetProcAddress(GetModuleHandleA("user32.dll"), name);
    MH_STATUS st = target ? MH_CreateHook(target, detour, (void**)original) : MH_ERROR_FUNCTION_NOT_FOUND;
    if (st == MH_OK) st = MH_EnableHook(target);
    Log("hook %s: %s", name, MH_StatusToString(st));
}

}  // namespace

bool HookGame(const char* name, Ptr preferredVa, void* detour, void** original) {
    if (!g_minhookReady) return false;
    if (!FunctionOk(preferredVa)) {
        Log("hook %s skipped: signature mismatch", name);
        return false;
    }
    void* target = (void*)Va(preferredVa);
    MH_STATUS st = MH_CreateHook(target, detour, original);
    if (st == MH_OK) st = MH_EnableHook(target);
    Log("hook %s at %p: %s", name, target, MH_StatusToString(st));
    return st == MH_OK;
}

void InitHooks() {
    MH_STATUS st = MH_Initialize();
    g_minhookReady = st == MH_OK || st == MH_ERROR_ALREADY_INITIALIZED;
    if (!g_minhookReady) {
        Log("MinHook init failed: %s", MH_StatusToString(st));
        return;
    }
    HookGame("cGZWinMgrW32::DoModalWin", 0xa0972f48, (void*)DoModalDetour, &g_doModal);
    InitBuyHooks();
    InitNeighborhoodHooks();
    InitSimAnticsHooks();
    HookApi("GetAsyncKeyState", (void*)GetAsyncKeyStateDetour, &g_getAsyncKeyState);
    HookApi("GetKeyState", (void*)GetKeyStateDetour, &g_getKeyState);
}

bool ModifierHeld(int vk) {
    return SyntheticHeld(vk) || ((g_getAsyncKeyState ? g_getAsyncKeyState(vk) : GetAsyncKeyState(vk)) & 0x8000) != 0;
}

void SetSyntheticModifier(int vk, bool down) {
    if (vk >= 0 && vk < 256) g_syntheticHeld[vk] = down;
}

void ShutdownHooks() {
    if (!g_minhookReady) return;
    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(MH_ALL_HOOKS);
    MH_Uninitialize();
    g_minhookReady = false;
    g_modals.clear();
    g_getAsyncKeyState = nullptr;
    g_getKeyState = nullptr;
    memset(g_syntheticHeld, 0, sizeof g_syntheticHeld);
}

// A dialog can open right after another's loop returned (Aceptar on one welcome dialog opens
// the next), so closed ones are recognised by their window: gone or hidden. Past that, more
// entries than the manager's nesting count are the oldest.
const std::vector<Ptr>& ModalStack() {
    for (size_t i = g_modals.size(); i-- > 0;)
        if (!IsKindOf(g_modals[i], "cGZWin") || !Visible(g_modals[i])) g_modals.erase(g_modals.begin() + i);
    int open = ModalCount();
    while ((int)g_modals.size() > open && open >= 0) g_modals.erase(g_modals.begin());
    return g_modals;
}

short RealGetAsyncKeyState(int vk) { return g_getAsyncKeyState ? g_getAsyncKeyState(vk) : GetAsyncKeyState(vk); }

}  // namespace game
