#include "mouse.h"

#include "../util/log.h"
#include "MinHook.h"

namespace game {
namespace {

HWND g_hwnd = nullptr;
bool g_active = false;
POINT g_client = {};  // virtual position, client coordinates

typedef BOOL(WINAPI* GetCursorPosFn)(LPPOINT);
typedef BOOL(WINAPI* SetCursorPosFn)(int, int);
GetCursorPosFn g_getCursorPos = nullptr;
SetCursorPosFn g_setCursorPos = nullptr;

BOOL WINAPI GetCursorPosDetour(LPPOINT pt) {
    if (g_active && pt && g_hwnd) {
        POINT p = g_client;
        ClientToScreen(g_hwnd, &p);
        *pt = p;
        return TRUE;
    }
    return g_getCursorPos(pt);
}

// The neighborhood's tools take mouse input only while the Windows cursor shows (the cursor
// manager asks GetCursorInfo); the player's cursor may be hidden in another window.
typedef BOOL(WINAPI* GetCursorInfoFn)(PCURSORINFO);
GetCursorInfoFn g_getCursorInfo = nullptr;
BOOL WINAPI GetCursorInfoDetour(PCURSORINFO info) {
    BOOL ok = g_getCursorInfo(info);
    if (ok && g_active && g_hwnd && info) {
        POINT p = g_client;
        ClientToScreen(g_hwnd, &p);
        info->flags = CURSOR_SHOWING;
        info->ptScreenPos = p;
    }
    return ok;
}

BOOL WINAPI SetCursorPosDetour(int x, int y) {
    if (g_active && g_hwnd) {
        POINT p = {x, y};
        ScreenToClient(g_hwnd, &p);
        g_client = p;
        return TRUE;
    }
    return g_setCursorPos(x, y);
}

LPARAM Coords(int x, int y) { return MAKELPARAM((WORD)(short)x, (WORD)(short)y); }

void HookApi(const char* name, void* detour, void** original) {
    void* target = (void*)GetProcAddress(GetModuleHandleA("user32.dll"), name);
    MH_STATUS st = target ? MH_CreateHook(target, detour, original) : MH_ERROR_FUNCTION_NOT_FOUND;
    if (st == MH_OK) st = MH_EnableHook(target);
    Log("hook %s: %s", name, MH_StatusToString(st));
}

}  // namespace

void InitMouse(HWND gameWindow) {
    g_hwnd = gameWindow;
    HookApi("GetCursorPos", (void*)GetCursorPosDetour, (void**)&g_getCursorPos);
    HookApi("SetCursorPos", (void*)SetCursorPosDetour, (void**)&g_setCursorPos);
    HookApi("GetCursorInfo", (void*)GetCursorInfoDetour, (void**)&g_getCursorInfo);
}

bool MoveRealPointer(int screenX, int screenY) {
    if (g_active) return false;  // the mod's own clicks own the pointer the game sees
    return g_setCursorPos ? g_setCursorPos(screenX, screenY) != 0 : SetCursorPos(screenX, screenY) != 0;
}

void MouseMove(int x, int y) {
    g_active = true;
    g_client = {x, y};
    PostMessageA(g_hwnd, WM_MOUSEMOVE, 0, Coords(x, y));
}

void MouseClick(int x, int y, bool right) {
    MouseMove(x, y);
    PostMessageA(g_hwnd, right ? WM_RBUTTONDOWN : WM_LBUTTONDOWN, right ? MK_RBUTTON : MK_LBUTTON, Coords(x, y));
    PostMessageA(g_hwnd, right ? WM_RBUTTONUP : WM_LBUTTONUP, 0, Coords(x, y));
}

void MouseRelease() { g_active = false; }

namespace {
// A drag in progress, one message per tick (the game reads the cursor between its frames).
struct Drag {
    int step = 0;  // 0 none
    POINT from = {}, to = {};
} g_drag;
constexpr int kDragMoves = 5;
}  // namespace

void MouseDrag(int x0, int y0, int x1, int y1) {
    g_drag.step = 1;
    g_drag.from = {x0, y0};
    g_drag.to = {x1, y1};
}

bool MouseDragging() { return g_drag.step != 0; }

void MouseTick() {
    if (!g_drag.step || !g_hwnd) return;
    int step = g_drag.step++;
    POINT a = g_drag.from, b = g_drag.to;
    if (step == 1) {
        MouseMove(a.x, a.y);
    } else if (step == 2) {
        PostMessageA(g_hwnd, WM_LBUTTONDOWN, MK_LBUTTON, Coords(a.x, a.y));
    } else if (step <= 2 + kDragMoves) {
        int k = step - 2;  // 1..kDragMoves: along the way to the end
        int x = a.x + (b.x - a.x) * k / kDragMoves, y = a.y + (b.y - a.y) * k / kDragMoves;
        g_client = {x, y};
        PostMessageA(g_hwnd, WM_MOUSEMOVE, MK_LBUTTON, Coords(x, y));
    } else {
        PostMessageA(g_hwnd, WM_LBUTTONUP, 0, Coords(b.x, b.y));
        g_drag.step = 0;
    }
}

bool MouseActive() { return g_active; }

HWND GameWindow() { return g_hwnd; }

}  // namespace game
