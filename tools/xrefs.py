"""Direct call/jump sites (E8/E9 rel32) to a function, with the functions that contain them.

  python xrefs.py <va> [va ...]
"""
import bisect, csv, os, struct, sys
import pefile
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DUMP = os.path.join(ROOT, "dumps", "Sims2EP9_1.18.0.187_24041f83_dump.exe")
pe = pefile.PE(DUMP, fast_load=True)
base = pe.OPTIONAL_HEADER.ImageBase
t = [s for s in pe.sections if s.Name.startswith(b".text")][0]
d = t.get_data()
o = base + t.VirtualAddress
funcs = sorted((int(r["va"], 16), r["name"]) for r in csv.DictReader(open(os.path.join(ROOT, "re", "functions.tsv")), delimiter="\t"))
starts = [f[0] for f in funcs]
def containing(a):
    k = bisect.bisect_right(starts, a) - 1
    return funcs[k] if k >= 0 else (0, "?")
for arg in sys.argv[1:]:
    target = int(arg, 16)
    k = bisect.bisect_right(starts, target) - 1
    if k >= 0 and starts[k] != target:
        print(f"note: {target:#x} is inside {funcs[k][1]} at {starts[k]:#x}")
    found = 0
    for i in range(len(d) - 5):
        if d[i] in (0xE8, 0xE9):
            rel = struct.unpack_from("<i", d, i + 1)[0]
            if o + i + 5 + rel == target:
                f = containing(o + i)
                print(f"{target:#x} <- {'call' if d[i] == 0xE8 else 'jmp '} at {o + i:#x} in {f[1]} ({f[0]:#x})")
                found += 1
    print(f"{found} sites")
