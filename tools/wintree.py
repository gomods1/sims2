# memtool script: print the live cGZWin tree of The Sims 2 from the window manager's main window.
#   python memtool.py -f wintree.py
#   env WINTREE_HIDDEN=0 hides invisible windows, WINTREE_DEPTH limits depth.
# Offsets (STRUCTS.md "GZ windows"): parent +0x58, child list [+0x54] (std::list of cIGZWin*),
# id +0x10, caption cRZString +0x60 (std::string at +0x64), area l,t,r,b +0xc8, flags +0xe8
# (1 = visible, 2 = enabled), window proc +0x12c (varies by class).
import csv
import os

MAX_DEPTH = int(os.environ.get("WINTREE_DEPTH", "30"))
SHOW_HIDDEN = os.environ.get("WINTREE_HIDDEN", "1") == "1"

_wm_vt = None
for r in csv.DictReader(open(os.path.join(ROOT, "re", "classes.tsv"), encoding="utf-8"), delimiter="\t"):
    if r["class"] == "cGZWinMgrW32" and r["this_offset"] == "0":
        _wm_vt = int(r["vtable_va"], 16)


def find_wm():
    # Stale copies of the vtable pointer lie around in freed memory: the live manager is the one
    # whose main window (+0x44) is a window.
    for h in rpc.scan_u32(va(_wm_vt)):
        wm = int(h, 16)
        main = u32(wm + 0x44)
        if main and (cls(main) or "").startswith("cGZWin"):
            return wm
    return None


def children(w):
    cont = u32(w + 0x54)
    if not cont:
        return []
    head = u32(cont)
    out = []
    n = u32(head) if head else None
    while n and n != head and len(out) < 2000:
        out.append(u32(n + 8))
        n = u32(n)
    return out


def utf8(a):
    s = std(a)
    return s


def describe(w):
    c = cls(w) or "?"
    l, t, r, b = u32s(w + 0xc8, 4)
    flags = u32(w + 0xe8)
    wid = u32(w + 0x10)
    cap = std(w + 0x64) or ""
    return f"{c} id={wid:#x} [{l},{t},{r},{b}] f={flags:#x} {cap!r}"


def walk(w, depth, parent_visible):
    flags = u32(w + 0xe8) or 0
    vis = parent_visible and bool(flags & 1)
    if vis or SHOW_HIDDEN:
        print("  " * depth + ("" if vis else "(hidden) ") + f"{w:#x} " + describe(w))
    if depth < MAX_DEPTH and (vis or SHOW_HIDDEN):
        for ch in children(w):
            if ch:
                walk(ch, depth + 1, vis)


wm = find_wm()
print("wm", hex(wm), "main", hex(u32(wm + 0x44)), "+0x40", cls(u32(wm + 0x40)), "+0x38", cls(u32(wm + 0x38)),
      "+0x34", cls(u32(wm + 0x34)))
walk(u32(wm + 0x44), 0, True)
