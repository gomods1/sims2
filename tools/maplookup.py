"""Name the function of the mod core at an RVA (from a CRASH line in the log): python maplookup.py 0x35a21"""
import re, sys
MAP = r"C:\Users\Nitropc\code\mods\sims2\mod\out\Sims2Access.map"
text = open(MAP, encoding="latin1").read()
base = int(re.search(r"Preferred load address is ([0-9a-f]+)", text).group(1), 16)
syms = []
for m in re.finditer(r"^\s*[0-9a-f]{4}:[0-9a-f]{8}\s+(\S+)\s+([0-9a-f]{8})", text, re.M):
    syms.append((int(m.group(2), 16) - base, m.group(1)))
syms.sort()
for arg in sys.argv[1:]:
    rva = int(arg, 16)
    best = max((s for s in syms if s[0] <= rva), default=None)
    print(hex(rva), best[1] if best else "?", hex(rva - best[0]) if best else "")
