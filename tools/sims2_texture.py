"""Save a Sims 2 texture (TXTR, with its LIFO mips) from a package as PNG (development only: the
images are the game's and stay in shots/, never shipped).

  python tools/sims2_texture.py <package> <name substring> <out.png> [package with the LIFO mips ...]
"""
import re
import struct
import sys
import os
import io

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dbpf
from PIL import Image

T_TXTR = 0x1C4A276C
T_LIFO = 0xED534136


def dds(w, h, fourcc, data):
    head = struct.pack("<4sIIIIIII44sII4sIIIIIIIIII", b"DDS ", 124, 0x81007, h, w, len(data), 0, 1, b"\0" * 44,
                       32, 4, fourcc, 0, 0, 0, 0, 0, 0x1000, 0, 0, 0, 0)
    return head + data


def main(argv):
    pkg = dbpf.Package(argv[0])
    want = argv[1].lower().encode()
    lifos = {}
    for extra in [pkg] + [dbpf.Package(x) for x in argv[3:]]:
        for e in extra.entries:
            if e[0] == T_LIFO:
                d = extra.read(e)
                m = re.search(rb"[\w\-~]+_lifo", d[:300])
                if m and want in m.group(0).lower():
                    lifos[m.group(0).lower()] = d
    for e in pkg.entries:
        if e[0] != T_TXTR:
            continue
        d = pkg.read(e)
        m = re.search(rb"[\w\-~]+_txtr", d[:300])
        if not m or want not in m.group(0).lower():
            continue
        i = m.end()
        w, h, fmt, mips = struct.unpack_from("<4I", d, i)
        i += 28
        ln = d[i]; i += 1 + ln
        cnt = struct.unpack_from("<I", d, i)[0]; i += 4
        big = None
        bw, bh = w, h
        for k in range(cnt):
            t = d[i]; i += 1
            lw, lh = max(1, w >> (cnt - 1 - k)), max(1, h >> (cnt - 1 - k))
            if t == 0:
                sz = struct.unpack_from("<I", d, i)[0]; i += 4
                big = d[i:i + sz]; i += sz
                bw, bh = lw, lh
            else:
                ln = d[i]; i += 1
                ref = d[i:i + ln].lower(); i += ln
                lf = lifos.get(ref) or next((v for k, v in lifos.items() if k.endswith(ref) or ref.endswith(k)), None)
                if lf:
                    j = lf.index(b"_lifo") + 5
                    lw, lh, z, sz = struct.unpack_from("<4I", lf, j)
                    big = lf[j + 16:j + 16 + sz]
                    bw, bh = lw, lh
                else:
                    print("missing", ref)
        four = {4: b"DXT1", 5: b"DXT3", 6: b"DXT5"}.get(fmt, b"DXT5")
        img = Image.open(io.BytesIO(dds(bw, bh, four, big)))
        img.save(argv[2])
        print(m.group(0).decode(), w, h, fmt, "saved", bw, bh, "->", argv[2])
        return


if __name__ == "__main__":
    main(sys.argv[1:])
