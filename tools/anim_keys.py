"""Animation descriptions: the keys of both games' Sim animations, the work chunks for writing
their descriptions, and the merged files the mods load.

  python tools/anim_keys.py collect        every animation name of both games -> extracted/anims/names_s1.tsv, names_s2.tsv
  python tools/anim_keys.py chunks [n]     the keys to describe, n a chunk (150) -> extracted/anims/en_in/NNN.tsv
  python tools/anim_keys.py tchunks [n]    the English descriptions in chunks of n (300) for translating -> extracted/anims/tr_in/NNN.tsv
  python tools/anim_keys.py status         what is done per language
  python tools/anim_keys.py glossin        lines naming things the game has its own word for -> gloss_in/<lang>.tsv
  python tools/anim_keys.py glossapply     gloss_out/<lang>.tsv (the game-term pass's changes) -> the chunks
  python tools/anim_keys.py merge        the done chunks -> mod/anims/<lang>.tsv (and the Sims 1 mod's copy)
  python tools/anim_keys.py key <name>...  the key of names (to compare with the mod's)

A key is an animation's internal name without who does it (its first part: "a2o", "c", "d2a"...),
in lowercase words: "a2o-bed-relax-loop-loop2_anim" -> "bed relax". Loop marks, numbers, variants
(alt0, var1, v2) and rig marks (fk, ik, blend) go, a repeated word once. The mods compute the same
key at runtime (sims2 nav/anim_names.cpp, the Sims 1 mod's copy): change both together.
The work files are game material (names): extracted/ is never committed. The merged descriptions
(mod/anims/<lang>.tsv: key TAB text) are the mod's own writing; "-" means "say nothing" (blinking).
"""

import glob
import mmap
import os
import re
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(HERE)
WORK = os.path.join(REPO, "extracted", "anims")
OUT = os.path.join(REPO, "mod", "anims")
SIMS1_OUT = os.path.join(os.path.dirname(REPO), "sims", "mod", "anims")
S2_ROOT = r"S:\steam\steamapps\common\The Sims 2 Legacy Collection"
S1_ROOT = r"S:\steam\steamapps\common\The Sims Legacy Collection"
LANGS = ["en", "es", "fr", "de", "it", "nl", "da", "sv", "no", "fi", "pt", "pl", "cs", "hu", "ru", "ja", "ko",
         "zh-hans", "zh-hant", "th"]

WHO = re.compile(r"^\d*[a-z]{1,2}(\d*2[a-z]{1,2})?$")
DROP = re.compile(r"^(loop\d*|alt\d*|var\d*|v\d+|fk|ik|blend)$")


def words(part: str):
    """Splits on anything not a letter or digit, on digit/letter changes, and camelCase
    ("IKTransJig" -> ik trans jig, "loop250" -> loop 250)."""
    out, cur = [], ""
    kind = lambda c: "d" if c.isdigit() else "u" if "A" <= c <= "Z" else "l" if "a" <= c <= "z" else ""
    for i, c in enumerate(part):
        k = kind(c)
        if not k:
            if cur:
                out.append(cur)
            cur = ""
            continue
        if cur:
            p = kind(part[i - 1])
            nxt = kind(part[i + 1]) if i + 1 < len(part) else ""
            if (k == "d") != (p == "d") or (k == "u" and p == "l") or (k == "u" and p == "u" and nxt == "l"):
                out.append(cur)
                cur = ""
        cur += c
    if cur:
        out.append(cur)
    return [w.lower() for w in out]


def key(raw: str) -> str:
    if raw.lower().endswith("_anim"):
        raw = raw[:-5]
    parts = raw.split("-")
    if len(parts) > 1 and WHO.match(parts[0].lower()):
        parts = parts[1:]
    out = []
    for p in parts:
        for w in words(p):
            if w.isdigit() or DROP.match(w) or (out and out[-1] == w):
                continue
            out.append(w)
    return " ".join(out)


def who(raw: str) -> str:
    parts = raw.split("-")
    return parts[0].lower() if len(parts) > 1 and WHO.match(parts[0].lower()) else ""


def is_sim(raw: str) -> bool:
    """Run by a Sim or a pet: not an object's own ("o", "o2a")."""
    w = who(raw)
    return bool(w) and not re.match(r"^\d*o", w)


# --- collect -------------------------------------------------------------------------------

def _unpack(data, limit):
    out = bytearray()
    i, n = 0, len(data)
    while i < n and len(out) < limit:
        c0 = data[i]
        if c0 < 0x80:
            c1 = data[i + 1]; plain = c0 & 3; copy = ((c0 >> 2) & 7) + 3; off = ((c0 & 0x60) << 3) + c1 + 1; i += 2
        elif c0 < 0xC0:
            c1, c2 = data[i + 1], data[i + 2]; plain = (c1 >> 6) & 3; copy = (c0 & 0x3F) + 4; off = ((c1 & 0x3F) << 8) + c2 + 1; i += 3
        elif c0 < 0xE0:
            c1, c2, c3 = data[i + 1], data[i + 2], data[i + 3]; plain = c0 & 3; copy = ((c0 & 0x0C) << 6) + c3 + 5
            off = ((c0 & 0x10) << 12) + (c1 << 8) + c2 + 1; i += 4
        elif c0 < 0xFC:
            plain = ((c0 & 0x1F) << 2) + 4; copy = 0; off = 0; i += 1
        else:
            plain = c0 & 3; copy = 0; off = 0; i += 1
        out += data[i:i + plain]
        i += plain
        for _ in range(copy):
            out.append(out[-off])
        if c0 >= 0xFC:
            break
    return bytes(out)


def collect_s2():
    """The name of every ANIM resource (0xFB00791E) of every pack."""
    names = {}
    paths = glob.glob(S2_ROOT + r"\*\TSData\Res*\Sims3D\*.package") + glob.glob(S2_ROOT + r"\*\TSData\Res*\3D\*.package")
    for path in sorted(paths):
        with open(path, "rb") as f:
            d = mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ)
            count, off, _ = struct.unpack_from("<III", d, 0x24)
            esz = 24 if struct.unpack_from("<I", d, 0x3C)[0] == 2 else 20
            for k in range(count):
                p = off + k * esz
                t, o, s = (struct.unpack_from("<I", d, p)[0], *struct.unpack_from("<II", d, p + esz - 8))
                if t != 0xFB00791E:
                    continue
                raw = d[o:o + min(s, 2000)]
                body = _unpack(raw[9:], 600) if raw[4:6] == b"\x10\xfb" else raw
                m = re.search(rb"([A-Za-z0-9][\w\-\. ]{2,120}?)_anim", body)
                if m:
                    names.setdefault(m.group(1).decode("latin1"), os.path.relpath(path, S2_ROOT).split(os.sep)[0])
    return names


def collect_s1():
    """Animation names as they appear in the game's archives (Animation.far, the packs' .far)."""
    pat = re.compile(rb"(?<![\w-])((?:\d?[a-z]{1,2}2[a-z]{1,2}|[a-z])-[A-Za-z0-9][\w\-]{2,80})")
    names = {}
    for dp, _dn, fn in os.walk(S1_ROOT):
        if "UserData" in dp or "SimsAccess" in dp:
            continue
        for f in fn:
            if not f.lower().endswith((".far", ".bcf", ".cmx", ".iff")):
                continue
            p = os.path.join(dp, f)
            try:
                d = open(p, "rb").read()
            except OSError:
                continue
            for m in pat.finditer(d):
                n = s1_clean(m.group(1).decode("latin1"))
                if n:
                    names.setdefault(n, os.path.relpath(p, S1_ROOT).split(os.sep)[0])
    return names


def s1_clean(n):
    """The archives' names run into each other ("...-loop0xskill-a2a-...", "artistic2a2o-...") and
    binary data looks like names ("q-esf"): cut the joins, keep Sim animations with real words."""
    n = n.split("xskill")[0]
    m = re.search(r"(?<=[a-z0-9])\d?[ackd]2[aockd]-", n[4:])
    if m:
        n = n[:4 + m.start()]
    n = n.rstrip("-_0123456789") or n
    w, k = who(n), key(n)
    if not re.fullmatch(r"\d?[ackd](2[aockd])?", w) or not k:
        return ""
    t = k.split()
    if max(len(x) for x in t) < 4 or sum(len(x) < 3 for x in t) * 2 > len(t):
        return ""
    return n


def write_names(path, names):
    with open(path, "w", encoding="utf-8") as f:
        for n in sorted(names):
            f.write(f"{n}\t{names[n]}\n")


def read_names(game):
    out = {}
    with open(os.path.join(WORK, f"names_{game}.tsv"), encoding="utf-8") as f:
        for line in f:
            n, src = line.rstrip("\n").split("\t")
            out[n] = src
    return out


# --- chunks --------------------------------------------------------------------------------

def all_keys():
    """key -> (games, example names), Sim animations of both games."""
    keys = {}
    for game in ("s2", "s1"):
        for n, src in read_names(game).items():
            if not is_sim(n):
                continue
            k = key(n)
            if not k or k.isdigit() or len(k) < 3:
                continue
            e = keys.setdefault(k, [set(), []])
            e[0].add(game)
            if len(e[1]) < 3:
                e[1].append(n)
    return keys


def chunks(size=150):
    keys = all_keys()
    d = os.path.join(WORK, "en_in")
    os.makedirs(d, exist_ok=True)
    ordered = sorted(keys)
    for c in range(0, len(ordered), size):
        with open(os.path.join(d, f"{c // size:03d}.tsv"), "w", encoding="utf-8") as f:
            for k in ordered[c:c + size]:
                games, ex = keys[k]
                f.write(f"{k}\t{'+'.join(sorted(games))}\t{' | '.join(ex)}\n")
    print(len(keys), "keys in", (len(ordered) + size - 1) // size, "chunks of", size)


def read_tsv(path):
    out = {}
    if not os.path.exists(path):
        return out
    with open(path, encoding="utf-8") as f:
        for line in f:
            line = line.rstrip("\n").rstrip("\r")
            if "\t" in line:
                k, t = line.split("\t", 1)
                out[k.strip()] = t.strip()
    return out


def english():
    out = {}
    for p in sorted(glob.glob(os.path.join(WORK, "en_out", "*.tsv"))):
        out.update(read_tsv(p))
    return out


def tchunks(size=300):
    en = english()
    d = os.path.join(WORK, "tr_in")
    os.makedirs(d, exist_ok=True)
    ordered = [k for k in sorted(en) if en[k] != "-"]
    for c in range(0, len(ordered), size):
        with open(os.path.join(d, f"{c // size:03d}.tsv"), "w", encoding="utf-8") as f:
            for k in ordered[c:c + size]:
                f.write(f"{k}\t{en[k]}\n")
    print(len(ordered), "descriptions in", (len(ordered) + size - 1) // size, "chunks")


def status():
    n_in = len(glob.glob(os.path.join(WORK, "en_in", "*.tsv")))
    n_en = len(glob.glob(os.path.join(WORK, "en_out", "*.tsv")))
    print(f"en: {n_en}/{n_in} chunks")
    n_tr = len(glob.glob(os.path.join(WORK, "tr_in", "*.tsv")))
    for lang in LANGS[1:]:
        paths = sorted(glob.glob(os.path.join(WORK, "tr_out", lang, "*.tsv")))
        bad = []
        for p in paths:
            # A chunk is good when it has every input key, in order, each with a text.
            want = list(read_tsv(os.path.join(WORK, "tr_in", os.path.basename(p))))
            with open(p, encoding="utf-8") as f:
                rows = [l.split("\t", 1) for l in f.read().splitlines() if l]
            if [r[0] for r in rows] != want or any(len(r) < 2 or not r[1].strip() for r in rows):
                bad.append(os.path.basename(p)[:-4])
        note = f" (broken: {' '.join(bad)})" if bad else ""
        print(f"{lang}: {len(paths)}/{n_tr} chunks{note}")


GLOSS_RE = re.compile(r"woohoo|reaper|plantsim|plant sim|servo|social bunny|crumplebottom|bigfoot"
                      r"|genie|elixir|water wiggle", re.I)


def gloss_in():
    """The lines naming a thing the game has its own word for (WORK/glossary.tsv), per language:
    key TAB English TAB translation, for the game-term pass (WORK/gloss_task.md)."""
    en = {}
    for p in sorted(glob.glob(os.path.join(WORK, "tr_in", "*.tsv"))):
        for k, e in read_tsv(p).items():
            if GLOSS_RE.search(e):
                en[k] = e
    os.makedirs(os.path.join(WORK, "gloss_in"), exist_ok=True)
    for lang in LANGS[1:]:
        tr = {}
        for p in glob.glob(os.path.join(WORK, "tr_out", lang, "*.tsv")):
            tr.update(read_tsv(p))
        with open(os.path.join(WORK, "gloss_in", f"{lang}.tsv"), "w", encoding="utf-8", newline="\n") as f:
            for k, e in en.items():
                f.write(f"{k}\t{e}\t{tr.get(k, '')}\n")
    print(len(en), "lines per language")


def gloss_apply():
    """Writes the game-term pass's changed lines (WORK/gloss_out/<lang>.tsv) into the chunks."""
    for p in sorted(glob.glob(os.path.join(WORK, "gloss_out", "*.tsv"))):
        lang = os.path.basename(p)[:-4]
        fix = read_tsv(p)
        done = 0
        for c in glob.glob(os.path.join(WORK, "tr_out", lang, "*.tsv")):
            with open(c, encoding="utf-8") as f:
                rows = [l.split("\t", 1) for l in f.read().splitlines() if l]
            hit = [r for r in rows if r[0] in fix and fix[r[0]] and fix[r[0]] != r[1]]
            if not hit:
                continue
            for r in hit:
                r[1] = fix[r[0]]
            done += len(hit)
            with open(c, "w", encoding="utf-8", newline="\n") as f:
                f.writelines(f"{k}\t{t}\n" for k, t in rows)
        print(lang, done, "of", len(fix), "changed")


def merge():
    en = english()
    os.makedirs(OUT, exist_ok=True)
    langs = {"en": en}
    for lang in LANGS[1:]:
        t = {}
        for p in sorted(glob.glob(os.path.join(WORK, "tr_out", lang, "*.tsv"))):
            t.update(read_tsv(p))
        for k, v in en.items():
            if v == "-":
                t[k] = "-"
        langs[lang] = t
    for lang, t in langs.items():
        for out in (OUT, SIMS1_OUT):
            os.makedirs(out, exist_ok=True)
            with open(os.path.join(out, f"{lang}.tsv"), "w", encoding="utf-8", newline="\n") as f:
                for k in sorted(t):
                    if k in en and t[k]:
                        f.write(f"{k}\t{t[k]}\n")
        print(lang, len([k for k in t if k in en]), "of", len(en))


def main(argv):
    cmd = argv[0] if argv else ""
    if cmd == "collect":
        os.makedirs(WORK, exist_ok=True)
        s2 = collect_s2()
        write_names(os.path.join(WORK, "names_s2.tsv"), s2)
        s1 = collect_s1()
        write_names(os.path.join(WORK, "names_s1.tsv"), s1)
        print("sims 2:", len(s2), "sims 1:", len(s1))
    elif cmd == "chunks":
        chunks(int(argv[1]) if len(argv) > 1 else 150)
    elif cmd == "tchunks":
        tchunks(int(argv[1]) if len(argv) > 1 else 300)
    elif cmd == "status":
        status()
    elif cmd == "merge":
        merge()
    elif cmd == "glossin":
        gloss_in()
    elif cmd == "glossapply":
        gloss_apply()
    elif cmd == "key":
        for n in argv[1:]:
            print(f"{n}\t{key(n)}")
    else:
        print(__doc__)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
