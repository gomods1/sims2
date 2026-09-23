"""List the uses of a SimAntics primitive in the game's BHAVs (Sims 2 packages).

  python tools/bhav_ops.py <opcode hex> [package ...]    e.g. python tools/bhav_ops.py 70

Without packages, reads every objects.package of the install (base game and expansions). Prints
one line per instruction: package, group, BHAV name, instruction index, the 16 operand bytes.
"""
import glob
import os
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dbpf

T_BHAV = 0x42484156
GAME = r"S:\steam\steamapps\common\The Sims 2 Legacy Collection"


def instructions(b: bytes):
    """(name, [(opcode, operands16)]) of a Sims 2 BHAV resource."""
    name = b[:64].split(b"\0", 1)[0].decode("latin-1")
    sig, count = struct.unpack_from("<HH", b, 64)
    if sig < 0x8000 or sig > 0x8009:
        return name, []
    head = 64 + (12 if sig <= 0x8002 else 12 if sig <= 0x8006 else 12)
    # 0x8007+: opcode u16, true u16, false u16, operands 16, node version u8 (23 bytes)
    size = 23 if sig >= 0x8007 else 22 if sig >= 0x8003 else 12
    left = len(b) - head
    if count and left // count != size:  # header length differs: fit it from the end
        head = len(b) - count * size
    out = []
    for i in range(count):
        p = head + i * size
        op = struct.unpack_from("<H", b, p)[0]
        ops = b[p + (6 if size >= 22 else 4): p + (23 if size >= 22 else 12)]
        out.append((op, ops))
    return name, out


def main(argv):
    want = int(argv[0], 16)
    paths = argv[1:] or sorted(glob.glob(os.path.join(GAME, "*", "TSData", "Res", "Objects", "objects.package")))
    for path in paths:
        pkg = dbpf.Package(path)
        label = os.path.relpath(path, GAME).split(os.sep)[0]
        for e in pkg.of_type(T_BHAV):
            try:
                name, ins = instructions(pkg.read(e))
            except Exception:
                continue
            for i, (op, ops) in enumerate(ins):
                if op == want:
                    print(f"{label}\t{e[1]:08x}\t{name}\t{i}\t{ops.hex(' ')}")


if __name__ == "__main__":
    sys.stdout.reconfigure(encoding="utf-8")
    main(sys.argv[1:])
