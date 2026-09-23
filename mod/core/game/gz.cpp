#include "gz.h"

#include "../util/log.h"
#include "addresses.h"
#include "mouse.h"
#include "rtti.h"

namespace game {
namespace {

Ptr g_wm = 0;

template <class Fn>
Fn Slot(Ptr obj, int slot) {
    Ptr vt = Rd<Ptr>(obj);
    return InImage(vt) ? (Fn)Rd<Ptr>(vt + slot * 4) : nullptr;
}

Ptr LookUpWinMgr() {
    Ptr fw = Va(addr::kFramework);
    typedef bool(__thiscall * GetServiceFn)(Ptr, uint32_t, uint32_t, void**);
    auto get = Slot<GetServiceFn>(fw, addr::kFrameworkGetSystemService);
    if (!get || !InImage((Ptr)get)) return 0;
    void* out = nullptr;
    bool ok = false;
    if (!Guarded([&] { ok = get(fw, addr::kWinMgrServiceId, addr::kWinMgrIid, &out); }) || !ok || !out) return 0;
    Ptr wm = (Ptr)out;
    typedef uint32_t(__thiscall * ReleaseFn)(Ptr);
    if (auto release = Slot<ReleaseFn>(wm, 2)) Guarded([&] { release(wm); });  // the service lives on
    return wm;
}

}  // namespace

Ptr WinMgr() {
    if (!g_wm) {
        g_wm = LookUpWinMgr();
        if (g_wm) Log("window manager %p (%s)", (void*)g_wm, ClassName(g_wm).c_str());
    }
    return g_wm;
}

Ptr MainWindow() { return WinMgr() ? Rd<Ptr>(WinMgr() + addr::wm::kMainWindow) : 0; }
Ptr FocusWindow() { return WinMgr() ? Rd<Ptr>(WinMgr() + addr::wm::kFocus) : 0; }
Ptr CaptureWindow() { return WinMgr() ? Rd<Ptr>(WinMgr() + addr::wm::kCapture) : 0; }
Ptr ModalWindow() { return WinMgr() ? Rd<Ptr>(WinMgr() + addr::wm::kModal) : 0; }
int ModalCount() { return WinMgr() ? Rd<int>(WinMgr() + addr::wm::kModalCount) : 0; }

std::vector<Ptr> Children(Ptr w) {
    std::vector<Ptr> out;
    Ptr container = Rd<Ptr>(w + addr::win::kChildren);
    if (!container) return out;
    Ptr head = Rd<Ptr>(container);
    if (!head) return out;
    for (Ptr n = Rd<Ptr>(head); n && n != head && out.size() < 4000; n = Rd<Ptr>(n)) {
        Ptr child = Rd<Ptr>(n + 8);
        if (child) out.push_back(child);
    }
    return out;
}

Ptr Parent(Ptr w) { return Rd<Ptr>(w + addr::win::kParent); }
uint32_t WinId(Ptr w) { return Rd<uint32_t>(w + addr::win::kId); }

RECT Area(Ptr w) {
    RECT r = {};
    Read(w + addr::win::kArea, &r, sizeof r);
    return r;
}

RECT ScreenRect(Ptr w) {
    RECT r = Area(w);
    Ptr main = MainWindow();
    int depth = 0;
    for (Ptr p = Parent(w); p && p != main && depth < 64; p = Parent(p), depth++) {
        RECT pr = Area(p);
        OffsetRect(&r, pr.left, pr.top);
    }
    return r;
}

bool Visible(Ptr w) { return (Rd<uint32_t>(w + addr::win::kFlags) & addr::win::kFlagVisible) != 0; }
bool Enabled(Ptr w) { return (Rd<uint32_t>(w + addr::win::kFlags) & addr::win::kFlagEnabled) != 0; }

bool ShownOnScreen(Ptr w) {
    Ptr main = MainWindow();
    for (int depth = 0; w && depth < 64; w = Parent(w), depth++) {
        if (!Visible(w)) return false;
        if (w == main) return true;
    }
    return false;
}

std::string Caption(Ptr w) { return ReadStdString(w + addr::win::kCaption + 4); }

Ptr CompleteObject(Ptr w) {
    Ptr vt = Rd<Ptr>(w);
    if (!InImage(vt)) return w;
    Ptr col = Rd<Ptr>(vt - 4);
    if (!InImage(col)) return w;
    return w - Rd<uint32_t>(col + 4);
}

bool IsButton(Ptr w) { return IsKindOf(w, "cGZWinBtn"); }

std::string Tooltip(Ptr w) {
    if (!IsButton(w)) return "";
    return ReadStdString(CompleteObject(w) + addr::btn::kTip + 4);
}

bool ButtonOn(Ptr w) {
    return IsButton(w) && (Rd<uint32_t>(CompleteObject(w) + addr::btn::kFlags) & addr::btn::kFlagOn) != 0;
}

bool ButtonToggles(Ptr w) {
    return IsButton(w) && (Rd<uint32_t>(CompleteObject(w) + addr::btn::kFlags) & addr::btn::kFlagToggle) != 0;
}

bool CaptionShown(Ptr w) {
    return IsButton(w) && Rd<uint32_t>(CompleteObject(w) + addr::btn::kCaptionExtent) != 0;
}

std::string ProcClass(Ptr w) {
    Ptr proc = Rd<Ptr>(w + addr::win::kProc);
    std::string c = ClassName(proc);
    return c.find("WinProc") != std::string::npos ? c : std::string();
}

Ptr Proc(Ptr w) {
    Ptr proc = Rd<Ptr>(w + addr::win::kProc);
    return ClassName(proc).find("WinProc") != std::string::npos ? proc : 0;
}

bool MeterPercent(Ptr w, int* percent) {
    Ptr proc = Proc(w);
    if (!IsKindOf(proc, "cTSWinProcGradientFillBar")) return false;
    int v = Rd<int>(proc + addr::fillbar::kValue), lo = Rd<int>(proc + addr::fillbar::kMin),
        hi = Rd<int>(proc + addr::fillbar::kMax);
    if (hi <= lo) return false;
    if (v < lo) v = lo;
    if (v > hi) v = hi;
    *percent = (int)((long long)(v - lo) * 100 / (hi - lo));
    return true;
}

bool IsText(Ptr w) { return IsKindOf(w, "cGZWinText"); }
bool IsTextEdit(Ptr w) { return IsKindOf(w, "cGZWinTextEdit"); }

bool TextEditEditable(Ptr w) {
    return IsTextEdit(w) && (Rd<uint32_t>(w + addr::edit::kFlags) & addr::edit::kFlagEditable) != 0;
}

std::string TextEditText(Ptr w) { return IsTextEdit(w) ? ReadStdString(w + addr::edit::kText + 4) : ""; }

bool PressAt(Ptr w, int x, int y) {
    typedef bool(__thiscall * MouseFn)(Ptr, int, int, uint32_t);
    typedef bool(__thiscall * EnterFn)(Ptr, uint32_t, uint32_t);
    auto enter = Slot<EnterFn>(w, addr::win::kSlotMouseEnter);
    auto down = Slot<MouseFn>(w, addr::win::kSlotMouseDownL);
    auto up = Slot<MouseFn>(w, addr::win::kSlotMouseUpL);
    if (!enter || !down || !up) return false;
    bool ok = Guarded([&] { enter(w, 0, 0); });
    ok = ok && Guarded([&] { down(w, x, y, MK_LBUTTON); });
    ok = ok && Guarded([&] { up(w, x, y, 0); });
    if (!ok) Log("press of %s %p crashed", ClassName(w).c_str(), (void*)w);
    return ok;
}

POINT UiToClient(int x, int y) {
    RECT ui = Area(MainWindow());
    RECT client = {};
    GetClientRect(GameWindow(), &client);
    int uw = ui.right - ui.left, uh = ui.bottom - ui.top;
    if (uw <= 0 || uh <= 0 || client.right <= 0) return {x, y};
    return {(int)((long long)x * client.right / uw), (int)((long long)y * client.bottom / uh)};
}

bool ClickWindow(Ptr w) {
    RECT r = ScreenRect(w);
    POINT p = UiToClient((r.left + r.right) / 2, (r.top + r.bottom) / 2);
    MouseClick(p.x, p.y);
    return true;
}

bool Press(Ptr w) {
    RECT r = Area(w);
    return PressAt(w, (r.right - r.left) / 2, (r.bottom - r.top) / 2);
}

}  // namespace game
