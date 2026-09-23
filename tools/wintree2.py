# memtool script: the visible cGZWin tree with window procs, button flags and tooltips.
#   python memtool.py -f wintree2.py        (env WINTREE_ROOT=0x... to start elsewhere)
import os

exec(open(os.path.join(ROOT, "tools", "wintree.py")).read().split("wm = find_wm()")[0])

MAXD = int(os.environ.get("WINTREE_DEPTH", "30"))


def walk2(w, d):
    f = u32(w + 0xe8) or 0
    if not f & 1 or d > MAXD:
        return
    c = cls(w) or "?"
    extra = ""
    proc = u32(w + 0x12c)
    pc = cls(proc) if proc and proc > 0x10000 else None
    if pc and "WinProc" in pc:
        extra += " proc=" + pc
    if "Btn" in c or "TextLabel" in c:
        b = w - 4
        extra += " bf=%x tip=%r" % (u32(b + 0xfc) or 0, (std(b + 0x18c) or "")[:60])
    if "TextEdit" in c:
        extra += " text=%r" % (std(w + 0x100) or "")[:60]
    l, t, r, bb = u32s(w + 0xc8, 4)
    print("  " * d + f"{w:#x} {c} id={u32(w + 0x10):#x} [{l},{t},{r},{bb}] {(std(w + 0x64) or '')[:40]!r}{extra}")
    for ch in children(w):
        walk2(ch, d + 1)


root = int(os.environ["WINTREE_ROOT"], 16) if os.environ.get("WINTREE_ROOT") else u32(find_wm() + 0x44)
walk2(root, 0)
