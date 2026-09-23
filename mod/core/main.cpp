// Sims2Access.dll: the mod core, loaded by the version.dll proxy (mod/loader). Every entry point
// runs on the game's main thread (the window procedure and a 30 ms timer). This file only wires
// the modules together:
//   util/     log, text conversion, the game's language, the mod's own strings
//   lang/     the mod's strings in every language the game ships
//   speech/   speech through SpeechHost.exe
//   game/     the only code that knows addresses and offsets (game/addresses.h)
//   nav/      keyboard navigation over the game's widgets
//   input/    which keys the mod claims
#include <windows.h>

#include <string>

#include "../loader/loader_api.h"
#include "game/capture.h"
#include "game/cheats.h"
#include "game/gz.h"
#include "game/hooks.h"
#include "game/memory.h"
#include "game/mouse.h"
#include "game/rtti.h"
#include "input/keyboard.h"
#include "nav/navigator.h"
#include "speech/speech.h"
#include "util/language.h"
#include "util/log.h"
#include "util/settings.h"
#include "util/strings.h"

using game::Ptr;

namespace {

bool g_active = false;       // false on an unsupported build: speech only, no hooks, no keys
bool g_dev = false;
std::wstring g_modDir;

bool ModFileExists(const SaLoaderApi* api, const wchar_t* name) {
    std::wstring path = std::wstring(api->mod_dir) + L"\\" + name;
    return GetFileAttributesW(path.c_str()) != INVALID_FILE_ATTRIBUTES;
}

// Development: tools/launch.py writes the user's foreground window to return_focus.txt before
// starting the game. When the game's window takes the foreground at startup, the core hands it
// back (the foreground process may pass it on; an outside process is refused).
HWND g_gameWindow = nullptr;
HWND g_returnFocus = nullptr;
ULONGLONG g_returnFocusUntil = 0;

void ReadReturnFocus(const SaLoaderApi* api) {
    std::wstring path = std::wstring(api->mod_dir) + L"\\return_focus.txt";
    HANDLE f = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
    if (f == INVALID_HANDLE_VALUE) return;
    char text[32] = {};
    DWORD got = 0;
    ReadFile(f, text, sizeof text - 1, &got, nullptr);
    CloseHandle(f);
    DeleteFileW(path.c_str());
    g_returnFocus = (HWND)(uintptr_t)strtoul(text, nullptr, 0);
    g_returnFocusUntil = GetTickCount64() + 60000;
    Log("development: will hand the foreground back to window %p", (void*)g_returnFocus);
}

void ReturnFocus() {
    if (!g_returnFocus) return;
    if (GetTickCount64() > g_returnFocusUntil || !IsWindow(g_returnFocus)) {
        g_returnFocus = nullptr;
        return;
    }
    if (GetForegroundWindow() != g_gameWindow) return;
    BOOL ok = SetForegroundWindow(g_returnFocus);
    Log("development: handed the foreground back to %p (%d)", (void*)g_returnFocus, ok);
    g_returnFocus = nullptr;
}

// A crash inside the mod's own code is logged with its place (an RVA in this DLL, for the PDB)
// and swallowed, so one bad read does not end the mod for the session.
HMODULE g_self = nullptr;
int g_crashes = 0;

int CrashFilter(EXCEPTION_POINTERS* e, const char* where) {
    if (g_crashes++ < 20) {
        Ptr at = (Ptr)e->ExceptionRecord->ExceptionAddress;
        MEMORY_BASIC_INFORMATION mbi = {};
        VirtualQuery((void*)at, &mbi, sizeof mbi);
        Ptr base = (Ptr)mbi.AllocationBase;
        char module[MAX_PATH] = "?";
        GetModuleFileNameA((HMODULE)base, module, MAX_PATH);
        const char* name = strrchr(module, '\\');
        Log("CRASH in %s: exception 0x%08lx at %p (%s+0x%x), reading %p", where,
            e->ExceptionRecord->ExceptionCode, (void*)at, name ? name + 1 : module, (unsigned)(at - base),
            e->ExceptionRecord->NumberParameters > 1 ? (void*)e->ExceptionRecord->ExceptionInformation[1] : nullptr);
    }
    return EXCEPTION_EXECUTE_HANDLER;
}

void GuardedTick() {
    __try {
        nav::Tick();
    } __except (CrashFilter(GetExceptionInformation(), "tick")) {
    }
}

}  // namespace

extern "C" __declspec(dllexport) int __cdecl SA_Init(const SaLoaderApi* api) {
    if (!api || api->version != SA_LOADER_API_VERSION) return 0;
    bool dev = ModFileExists(api, L"dev.txt");
    g_dev = dev;
    g_modDir = api->mod_dir;
    util::InitLog(api, dev);
    Log("Sims2Access core init (built %s %s)", __DATE__, __TIME__);
    util::InitSettings(api->mod_dir);
    g_gameWindow = api->hwnd;
    if (dev) ReadReturnFocus(api);
    util::InitLanguage();  // from the registry: even an unsupported build speaks the player's language
    speech::Start(api->mod_dir);
    if (!api->build_ok) {
        Log("unsupported Sims2EP9.exe build %s: installing nothing", api->exe_hash);
        speech::Say(S(Str::UnsupportedBuild));
        return 1;
    }
    game::InitMemory(api->game);
    game::InitHooks();
    game::InitMouse(api->hwnd);
    if (dev) game::InitCapture(api->mod_dir);
    nav::Reset();
    g_active = true;
    if (!api->reloaded) speech::Say(S(Str::ModLoaded));
    return 1;
}

extern "C" __declspec(dllexport) void __cdecl SA_Shutdown() {
    Log("Sims2Access core shutdown");
    if (g_active) {
        game::ShutdownHooks();
        game::ClearRttiCache();
    }
    g_active = false;
    speech::Stop();
}

extern "C" __declspec(dllexport) int __cdecl SA_OnMessage(HWND, UINT msg, WPARAM wp, LPARAM lp, LRESULT* result) {
    if (!g_active) return 0;
    bool consumed = false;
    switch (msg) {
    case WM_KEYDOWN: consumed = input::OnKeyDown(wp, lp); break;
    case WM_KEYUP: consumed = input::OnKeyUp(wp, lp); break;
    case WM_CHAR: consumed = input::OnChar(wp); break;
    case WM_KILLFOCUS:
    case WM_ACTIVATEAPP: input::OnFocusLost(); break;
    }
    if (consumed) *result = 0;
    return consumed ? 1 : 0;
}

extern "C" __declspec(dllexport) void __cdecl SA_OnTick() {
    ReturnFocus();
    if (!g_active) return;
    game::PollCapture();
    if (g_dev) game::PollCheatRequest(g_modDir.c_str());
    GuardedTick();
}
