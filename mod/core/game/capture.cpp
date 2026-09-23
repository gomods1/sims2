#include "capture.h"

#include <d3d9.h>

#include <string>

#include "../util/log.h"
#include "MinHook.h"

#pragma comment(lib, "d3d9.lib")

namespace game {
namespace {

typedef HRESULT(STDMETHODCALLTYPE* PresentFn)(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
PresentFn g_present = nullptr;
void* g_presentTarget = nullptr;
std::wstring g_requestFile;   // Sims2Access\shot_request.txt: the path to write, one line
std::wstring g_pending;       // a capture waiting for the next Present
ULONGLONG g_lastPoll = 0;
unsigned g_presents = 0;

// Writes a 32-bit top-down BMP (tools/game.py converts it to PNG).
bool SaveBmp(const std::wstring& path, const void* bits, int pitch, int w, int h) {
    HANDLE f = CreateFileW(path.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
    if (f == INVALID_HANDLE_VALUE) return false;
    BITMAPFILEHEADER fh = {};
    BITMAPINFOHEADER ih = {};
    ih.biSize = sizeof ih;
    ih.biWidth = w;
    ih.biHeight = -h;
    ih.biPlanes = 1;
    ih.biBitCount = 32;
    fh.bfType = 0x4d42;
    fh.bfOffBits = sizeof fh + sizeof ih;
    fh.bfSize = fh.bfOffBits + (DWORD)w * h * 4;
    DWORD written = 0;
    WriteFile(f, &fh, sizeof fh, &written, nullptr);
    WriteFile(f, &ih, sizeof ih, &written, nullptr);
    for (int y = 0; y < h; y++) WriteFile(f, (const char*)bits + (size_t)y * pitch, w * 4, &written, nullptr);
    CloseHandle(f);
    return true;
}

void Capture(IDirect3DDevice9* dev) {
    std::wstring path = g_pending;
    g_pending.clear();
    IDirect3DSurface9* back = nullptr;
    IDirect3DSurface9* copy = nullptr;
    D3DSURFACE_DESC desc = {};
    HRESULT hr = dev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &back);
    if (SUCCEEDED(hr)) hr = back->GetDesc(&desc);
    if (SUCCEEDED(hr))
        hr = dev->CreateOffscreenPlainSurface(desc.Width, desc.Height, desc.Format, D3DPOOL_SYSTEMMEM, &copy, nullptr);
    if (SUCCEEDED(hr)) hr = dev->GetRenderTargetData(back, copy);
    D3DLOCKED_RECT lr = {};
    if (SUCCEEDED(hr)) hr = copy->LockRect(&lr, nullptr, D3DLOCK_READONLY);
    bool ok = false;
    if (SUCCEEDED(hr)) {
        ok = SaveBmp(path, lr.pBits, lr.Pitch, desc.Width, desc.Height);
        copy->UnlockRect();
    }
    if (copy) copy->Release();
    if (back) back->Release();
    Log("capture %ls: %s (hr 0x%08lx, %ux%u, format %d)", path.c_str(), ok ? "written" : "failed", hr,
        desc.Width, desc.Height, (int)desc.Format);
}

HRESULT STDMETHODCALLTYPE PresentDetour(IDirect3DDevice9* dev, const RECT* a, const RECT* b, HWND w, const RGNDATA* r) {
    g_presents++;
    if (!g_pending.empty()) Capture(dev);
    return g_present(dev, a, b, w, r);
}

// The device's vtable, from a throwaway device on a hidden window.
void* FindPresent() {
    IDirect3D9* d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (!d3d) return nullptr;
    HWND wnd = CreateWindowExA(0, "STATIC", "", WS_POPUP, 0, 0, 16, 16, nullptr, nullptr, nullptr, nullptr);
    D3DPRESENT_PARAMETERS pp = {};
    pp.Windowed = TRUE;
    pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    pp.hDeviceWindow = wnd;
    pp.BackBufferFormat = D3DFMT_UNKNOWN;
    IDirect3DDevice9* dev = nullptr;
    void* present = nullptr;
    HRESULT hr = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, wnd,
                                   D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_FPU_PRESERVE, &pp, &dev);
    if (SUCCEEDED(hr) && dev) {
        present = (*(void***)dev)[17];
        dev->Release();
    } else {
        Log("capture: dummy device failed (0x%08lx)", hr);
    }
    d3d->Release();
    DestroyWindow(wnd);
    return present;
}

}  // namespace

void InitCapture(const wchar_t* modDir) {
    g_requestFile = std::wstring(modDir) + L"\\shot_request.txt";
    g_presentTarget = FindPresent();
    if (!g_presentTarget) return;
    MH_STATUS st = MH_CreateHook(g_presentTarget, (void*)PresentDetour, (void**)&g_present);
    if (st == MH_OK) st = MH_EnableHook(g_presentTarget);
    Log("hook IDirect3DDevice9::Present at %p: %s", g_presentTarget, MH_StatusToString(st));
}

void PollCapture() {
    ULONGLONG now = GetTickCount64();
    if (now - g_lastPoll < 300 || g_requestFile.empty()) return;
    g_lastPoll = now;
    HANDLE f = CreateFileW(g_requestFile.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
    if (f == INVALID_HANDLE_VALUE) return;
    char buf[520] = {};
    DWORD got = 0;
    ReadFile(f, buf, sizeof buf - 1, &got, nullptr);
    CloseHandle(f);
    DeleteFileW(g_requestFile.c_str());
    std::string path(buf);
    while (!path.empty() && (path.back() == '\n' || path.back() == '\r' || path.back() == ' ')) path.pop_back();
    int n = MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, nullptr, 0);
    std::wstring wpath(n > 0 ? n - 1 : 0, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, wpath.data(), n);
    g_pending = wpath;
    Log("capture requested: %s (presents so far %u)", path.c_str(), g_presents);
}

}  // namespace game
