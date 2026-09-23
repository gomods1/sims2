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
