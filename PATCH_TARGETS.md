# Hook and call targets (Sims2EP9.exe 1.18.0.187)

Every function the mod hooks or calls, with its convention and evidence. Signatures checked at
startup are in `mod/core/game/memory.cpp`.

## Hooked

| VA | What | Convention | Use | Evidence |
|---|---|---|---|---|
| 0xa0972f48 | `cGZWinMgrW32::DoModalWin(win)` | thiscall, 1 arg | logs modal windows and the call chain (naked detour, no frame of the mod on the stack during the dialog's loop) | disasm; live |
| user32 `GetKeyState`, `GetAsyncKeyState` | key state | stdcall | the test tool's posted Ctrl/Shift/Alt read as held | live (Ctrl+Shift+C opened the console) |
| user32 `GetCursorPos`, `SetCursorPos` | cursor | stdcall | the virtual mouse: the game sees the mod's position while it clicks; SetCursorPos calls move only the virtual cursor | live (pie menu opened by a virtual click) |
| d3d9 `IDirect3DDevice9::Present` (vtable slot 17) | frame end | stdcall | dev screenshots of the back buffer | live |
| 0xa078d6c3 | `cTSUserToolBase` slot 31: ground pick (sx, sy, float out[3], flag) -> bool | thiscall, 4 args | answers with the mod's spot while it places (every ground tool) | Frida: the move handler 0xa078eb62 calls it with the cursor, then stores the spot |
| 0xa067f31a | `cTSUserToolObject` slot 86: set fits (bool) | thiscall, 1 arg | records the tool's verdict on the held object's spot | Frida: 1 / 0 as the object turns green / red |
| 0xa078ec6d | `cTSUserToolBase` slot 48: the cursor left the view | thiscall, 0 args (a thunk to slot 60) | skipped while the mod places (the real cursor is elsewhere) | Frida: it cleared +0x50 every time |
| 0xa078d789 | `cTSUserToolBase` slot 29: object pick (sx, sy, 1, a, b) -> scene node interface | thiscall, 5 args | answers with a lot object's node while the mod clicks to pick it up | Frida: returns the node under the cursor (Sims, furniture) |

## Called

| VA / slot | What | Convention | Evidence |
|---|---|---|---|
| framework slot 5 (0xa01428e2) | GetSystemService(id, iid, out) | thiscall | returns the window manager and the cheat system |
| cheat system slot 7 (0xa0cbe460) | Execute(line) | thiscall(const char*) -> bool | the console's call; `help -all` output |
| cGZWin slots 161, 155, 157 | mouse enter, left down, left up | thiscall | presses buttons (Continuar, Jugar, Siguiente...) |
| 0xa07b7b27 | `cTSWinProcLotInfoDialog::Show(lotId, familyId, mode)` on the proc's +12 interface | thiscall | shows the lot panel as a click on the house |
| 0xa01be3b0 | game `operator new(size)` | cdecl | message allocation (as the loadLot cheat) |
| 0xa01c033d | `cRZMessage2Standard` constructor | thiscall | message 0xC0 opens interaction menus |
| message server slot 4 | Post(msg, 0) | thiscall | idem |
| cTSObject slots 92, 33, 36 | name, location, floor | thiscall | Lua bindings use them |
| globals slot 24, folder slot 14, entry slot 48, string slot 5 | catalog name by GUID | thiscall | Lua GetCatalogName |
| object module slot 39 | selected Sim | thiscall | pie builder uses it |
| framework GetSystemService(0xe92db716, 1) | the main toolbar | thiscall | +0x74 UI mode, +0x90 tool handler |
| user tool slots 42 (3, pos) then 45 (pos) | a mouse move, synchronously | thiscall | the mod judges thousands of spots this way (0.1 ms each) |
| 0xa067be89, 0xa067cb69 | object tool: holding?, held object | thiscall () | put down / sold detection |
