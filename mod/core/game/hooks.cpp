#include "hooks.h"

#include "../util/log.h"
#include "MinHook.h"
#include "addresses.h"
#include "gz.h"
#include "rtti.h"

namespace game {
namespace {

bool g_minhookReady = false;
std::vector<Ptr> g_modals;

// ---- cGZWinMgrW32::DoModalWin ---------------------------------------------------------------
// The detour only records the window and jumps on: a dialog's loop runs inside the call, and a
// frame of this DLL left on the stack there would crash when a rebuilt core is hot-reloaded.
// Closed modals are dropped by ModalStack() against the manager's nesting count.
void* g_doModal = nullptr;

void __stdcall RecordModal(Ptr win) {
    g_modals.push_back(win);
    Log("modal window %p (%s) opens", (void*)win, ClassName(win).c_str());
}

__declspec(naked) void DoModalDetour() {
    __asm {
        push ecx
        push dword ptr [esp + 8]
        call RecordModal
        pop ecx
        jmp dword ptr [g_doModal]
    }
}

template <class T>
bool TryHook(const char* name, Ptr preferredVa, void* detour, T* original) {
    if (!FunctionOk(preferredVa)) {
        Log("hook %s skipped: signature mismatch", name);
        return false;
    }
    void* target = (void*)Va(preferredVa);
    MH_STATUS st = MH_CreateHook(target, detour, (void**)original);
    if (st == MH_OK) st = MH_EnableHook(target);
    Log("hook %s at %p: %s", name, target, MH_StatusToString(st));
    return st == MH_OK;
}

}  // namespace

void InitHooks() {
    MH_STATUS st = MH_Initialize();
    g_minhookReady = st == MH_OK || st == MH_ERROR_ALREADY_INITIALIZED;
    if (!g_minhookReady) {
        Log("MinHook init failed: %s", MH_StatusToString(st));
        return;
    }
    TryHook("cGZWinMgrW32::DoModalWin", 0xa0972f48, (void*)DoModalDetour, &g_doModal);
}

void ShutdownHooks() {
    if (!g_minhookReady) return;
    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(MH_ALL_HOOKS);
    MH_Uninitialize();
    g_minhookReady = false;
    g_modals.clear();
}

const std::vector<Ptr>& ModalStack() {
    int open = ModalCount();
    while ((int)g_modals.size() > open && open >= 0) g_modals.pop_back();
    return g_modals;
}

short RealGetAsyncKeyState(int vk) { return GetAsyncKeyState(vk); }

}  // namespace game
