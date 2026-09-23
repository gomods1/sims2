"""Read The Sims 2 .package files (DBPF 1.x) and their QFS/RefPack-compressed resources.

  python dbpf.py list <file.package> [type_hex]         type group instance [instance_hi] size
  python dbpf.py types <file.package>                     resource counts per type
  python dbpf.py get <file.package> <type> <group> <inst> <out>   write one resource (decompressed)
  python dbpf.py str <file.package> [lang]                 dump every STR# / CTSS / TTAs (0 = all languages)
  python dbpf.py ui <file.package> <outdir>                write every UI script (type 0) as text

Layout (verified on this build's ui.package, 2026-09-23): header "DBPF", major 1, minor 0/1/2;
index entry count at 0x24, index offset 0x28, index size 0x2c, index minor version at 0x3c
(2 = entries carry a second instance dword). Entries: type, group, instance, [instance_hi],
offset, size. The DIR resource (type 0xE86B1EEF) lists compressed resources and their
uncompressed sizes; a compressed resource starts with u32 compressed size, u16 0x10FB, 3-byte
big-endian uncompressed size, then RefPack.
"""

import os
import struct
import sys

T_DIR = 0xE86B1EEF
T_STR = 0x53545223  # STR#
T_CTSS = 0x43545353
T_TTAS = 0x54544173
T_UI = 0x00000000

# The STR# language byte (FreeSO/SimPE numbering; checked against this build's text where noted).
LANGS = {
    1: "USEnglish", 2: "UKEnglish", 3: "French", 4: "German", 5: "Italian", 6: "Spanish",
    7: "Dutch", 8: "Danish", 9: "Swedish", 10: "Norwegian", 11: "Finnish", 12: "Hebrew",
    13: "Russian", 14: "Portuguese", 15: "Japanese", 16: "Polish", 17: "SimplifiedChinese",
    18: "TraditionalChinese", 19: "Thai", 20: "Korean", 21: "Czech", 22: "Hungarian",
}


def refpack_decompress(data: bytes, out_size: int) -> bytes:
    out = bytearray()
    i = 0
    n = len(data)
    while i < n:
        c0 = data[i]
        if c0 < 0x80:
            c1 = data[i + 1]
            plain = c0 & 3
            copy = ((c0 >> 2) & 7) + 3
            off = ((c0 & 0x60) << 3) + c1 + 1
            i += 2
        elif c0 < 0xC0:
            c1, c2 = data[i + 1], data[i + 2]
            plain = (c1 >> 6) & 3
            copy = (c0 & 0x3F) + 4
            off = ((c1 & 0x3F) << 8) + c2 + 1
            i += 3
        elif c0 < 0xE0:
            c1, c2, c3 = data[i + 1], data[i + 2], data[i + 3]
            plain = c0 & 3
            copy = ((c0 & 0x0C) << 6) + c3 + 5
            off = ((c0 & 0x10) << 12) + (c1 << 8) + c2 + 1
            i += 4
        elif c0 < 0xFC:
            plain = ((c0 & 0x1F) << 2) + 4
            copy = 0
            off = 0
            i += 1
        else:
            plain = c0 & 3
            copy = 0
            off = 0
            i += 1
        out += data[i : i + plain]
        i += plain
        for _ in range(copy):
            out.append(out[-off])
        if c0 >= 0xFC:
            break
    return bytes(out[:out_size]) if out_size else bytes(out)


class Package:
    def __init__(self, path: str):
        self.path = path
        self.data = open(path, "rb").read()
        d = self.data
        if d[:4] != b"DBPF":
            raise ValueError(f"{path}: not DBPF")
        count, off, _size = struct.unpack_from("<III", d, 0x24)
        self.index_minor = struct.unpack_from("<I", d, 0x3C)[0]
        esz = 24 if self.index_minor == 2 else 20
        self.entries = []
        for k in range(count):
            p = off + k * esz
            if esz == 24:
                t, g, inst, ihi, o, s = struct.unpack_from("<6I", d, p)
            else:
                t, g, inst, o, s = struct.unpack_from("<5I", d, p)
                ihi = 0
            self.entries.append((t, g, inst, ihi, o, s))
        self.compressed = {}
        for t, g, inst, ihi, o, s in self.entries:
            if t == T_DIR:
                step = 20 if self.index_minor == 2 else 16
                for q in range(o, o + s - step + 1, step):
                    if step == 20:
                        ct, cg, ci, chi, usz = struct.unpack_from("<5I", d, q)
                    else:
                        ct, cg, ci, usz = struct.unpack_from("<4I", d, q)
                        chi = 0
                    self.compressed[(ct, cg, ci, chi)] = usz

    def read(self, e) -> bytes:
        t, g, inst, ihi, o, s = e
        raw = self.data[o : o + s]
        key = (t, g, inst, ihi)
        if key in self.compressed and len(raw) > 9 and raw[4:6] == b"\x10\xfb":
            usz = (raw[6] << 16) | (raw[7] << 8) | raw[8]
            return refpack_decompress(raw[9:], usz)
        return raw

    def of_type(self, t: int):
        return [e for e in self.entries if e[0] == t]


def parse_str(b: bytes):
    """STR#/CTSS/TTAs: 64-byte name, u16 format 0xFFFD, u16 count, then per entry:
    u8 language, value\\0, description\\0 (UTF-8). Returns (name, [(lang, value, desc)])."""
    name = b[:64].split(b"\0")[0].decode("utf-8", "replace")
    fmt, count = struct.unpack_from("<HH", b, 64)
    p = 68
    out = []
    if fmt != 0xFFFD:
        return name, out
    for _ in range(count):
        if p >= len(b):
            break
        lang = b[p]
        p += 1
        e = b.index(b"\0", p)
        val = b[p:e].decode("utf-8", "replace")
        p = e + 1
        e = b.index(b"\0", p)
        desc = b[p:e].decode("utf-8", "replace")
        p = e + 1
        out.append((lang, val, desc))
    return name, out


def main(argv):
    if len(argv) < 2:
        print(__doc__)
        return 1
    cmd, path = argv[0], argv[1]
    pk = Package(path)
    if cmd == "list":
        want = int(argv[2], 16) if len(argv) > 2 else None
        for e in pk.entries:
            if want is None or e[0] == want:
                print(f"{e[0]:08x} {e[1]:08x} {e[2]:08x} {e[3]:08x} {e[5]}")
    elif cmd == "types":
        from collections import Counter
        for t, n in Counter(e[0] for e in pk.entries).most_common():
            print(f"{t:08x} {n}")
    elif cmd == "get":
        t, g, i = (int(x, 16) for x in argv[2:5])
        for e in pk.entries:
            if e[0] == t and e[1] == g and e[2] == i:
                open(argv[5], "wb").write(pk.read(e))
                print("wrote", argv[5])
                break
    elif cmd == "str":
        lang = int(argv[2]) if len(argv) > 2 else 1
        for e in pk.entries:
            if e[0] in (T_STR, T_CTSS, T_TTAS):
                name, items = parse_str(pk.read(e))
                print(f"== {e[0]:08x} {e[1]:08x} {e[2]:08x} {name}")
                idx = {}
                for lg, v, d in items:
                    if lang == 0 or lg == lang:
                        k = idx.get(lg, 0)
                        idx[lg] = k + 1
                        print(f"  [{lg}:{k}] {v!r}" + (f"  # {d}" if d else ""))
                    else:
                        idx[lg] = idx.get(lg, 0) + 1
    elif cmd == "ui":
        os.makedirs(argv[2], exist_ok=True)
        for e in pk.of_type(T_UI):
            b = pk.read(e)
            open(os.path.join(argv[2], f"{e[1]:08x}_{e[2]:08x}.uiScript"), "wb").write(b)
        print(len(pk.of_type(T_UI)), "ui scripts")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
