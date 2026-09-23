"""Show every slot of a class's vtable with a short disassembly (first N instructions).

  python vtdump.py <class> [this_offset=0] [n=6] [first-last]
"""
import csv, os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import capstone
from disasm import read

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
cls, off = sys.argv[1], sys.argv[2] if len(sys.argv) > 2 else "0"
n = int(sys.argv[3]) if len(sys.argv) > 3 else 6
rng = sys.argv[4].split("-") if len(sys.argv) > 4 else None
md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
rows = [r for r in csv.DictReader(open(os.path.join(ROOT, "re", "vtables.tsv"), encoding="utf-8"), delimiter="\t")
        if r["class"] == cls and r.get("this_offset", "0") == off]
for r in rows:
    s = int(r["slot"])
    if rng and not (int(rng[0]) <= s <= int(rng[1])):
        continue
    a = int(r["target_va"], 16)
    ins = []
    for i in md.disasm(read(a, 0x80), a):
        ins.append(f"{i.mnemonic} {i.op_str}".strip())
        if len(ins) >= n or i.mnemonic in ("ret", "jmp"):
            break
    print(f"{s:3d} {a:08x}  " + " ; ".join(ins))
