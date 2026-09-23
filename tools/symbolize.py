"""Function, file and line of RVAs in the mod core (CRASH lines in Sims2Access.log), from its PDB.

  python symbolize.py <dll> 0x35a21 [0x... ...]
  python symbolize.py --log      symbolizes every CRASH line of the game's Sims2Access.log
                                 against the DLL copy it names (Sims2Access\\run\\...)

Uses Windows' dbghelp.dll through ctypes. The PDB must match the DLL: the run\\ copies are exact
copies of the deployed Sims2Access.dll, whose PDB is deployed next to it.
"""

import ctypes
import ctypes.wintypes as wt
import os
import re
import sys

GAME_MOD_DIR = r"S:\steam\steamapps\common\The Sims 2 Legacy Collection\EP9\TSBin\Sims2Access"
dbghelp = ctypes.WinDLL("dbghelp")


class SYMBOL_INFO(ctypes.Structure):
    _fields_ = [("SizeOfStruct", wt.ULONG), ("TypeIndex", wt.ULONG), ("Reserved", ctypes.c_uint64 * 2),
                ("Index", wt.ULONG), ("Size", wt.ULONG), ("ModBase", ctypes.c_uint64), ("Flags", wt.ULONG),
                ("Value", ctypes.c_uint64), ("Address", ctypes.c_uint64), ("Register", wt.ULONG),
                ("Scope", wt.ULONG), ("Tag", wt.ULONG), ("NameLen", wt.ULONG), ("MaxNameLen", wt.ULONG),
                ("Name", ctypes.c_char * 512)]


class IMAGEHLP_LINE64(ctypes.Structure):
    _fields_ = [("SizeOfStruct", wt.DWORD), ("Key", ctypes.c_void_p), ("LineNumber", wt.DWORD),
                ("FileName", ctypes.c_char_p), ("Address", ctypes.c_uint64)]


def symbolize(dll: str, rvas: list[int], pdb_dir: str | None = None) -> list[str]:
    proc = wt.HANDLE(0x5A11)
    dbghelp.SymSetOptions(0x2 | 0x10)  # SYMOPT_UNDNAME | SYMOPT_LOAD_LINES
    search = (pdb_dir or os.path.dirname(dll)).encode()
    dbghelp.SymInitialize(proc, search, False)
    base = 0x10000000
    dbghelp.SymLoadModuleEx.restype = ctypes.c_uint64
    dbghelp.SymLoadModuleEx.argtypes = [wt.HANDLE, wt.HANDLE, ctypes.c_char_p, ctypes.c_char_p,
                                        ctypes.c_uint64, wt.DWORD, ctypes.c_void_p, wt.DWORD]
    loaded = dbghelp.SymLoadModuleEx(proc, None, dll.encode(), None, base, 0, None, 0)
    out = []
    for rva in rvas:
        addr = ctypes.c_uint64(base + rva)
        sym = SYMBOL_INFO()
        sym.SizeOfStruct = 88
        sym.MaxNameLen = 500
        disp = ctypes.c_uint64()
        name = "?"
        if dbghelp.SymFromAddr(proc, addr, ctypes.byref(disp), ctypes.byref(sym)):
            name = f"{sym.Name.decode()}+0x{disp.value:x}"
        line = IMAGEHLP_LINE64()
        line.SizeOfStruct = ctypes.sizeof(IMAGEHLP_LINE64)
        d32 = wt.DWORD()
        where = ""
        if dbghelp.SymGetLineFromAddr64(proc, addr, ctypes.byref(d32), ctypes.byref(line)):
            where = f" {os.path.basename(line.FileName.decode())}:{line.LineNumber}"
        out.append(f"{rva:#x} {name}{where}" + ("" if loaded else " (module not loaded)"))
    dbghelp.SymCleanup(proc)
    return out


def main(argv: list[str]) -> int:
    if argv and argv[0] == "--log":
        log = open(os.path.join(GAME_MOD_DIR, "Sims2Access.log"), encoding="utf-8", errors="replace").read()
        for m in re.finditer(r"CRASH in .*?\((Sims2Access_\w+\.dll)\+0x([0-9a-f]+)\)", log):
            dll = os.path.join(GAME_MOD_DIR, "run", m.group(1))
            if not os.path.exists(dll):
                dll = os.path.join(GAME_MOD_DIR, "Sims2Access.dll")
            print(m.group(0)[:60], "->", symbolize(dll, [int(m.group(2), 16)], GAME_MOD_DIR)[0])
        return 0
    if len(argv) < 2:
        print(__doc__)
        return 1
    for line in symbolize(argv[0], [int(a, 16) for a in argv[1:]]):
        print(line)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
