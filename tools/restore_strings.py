"""Bring string ids (with every translation) back from the Sims 1 mod's tables.

  python tools/restore_strings.py <group comment> Id [Id ...]

Appends the ids, in the order given, under a new group comment at the end of
mod/core/util/strings_table.h (before the last entry's continuation ends), and adds each
translation to mod/core/lang/<language>.cpp through tools/mod_strings.py. The Sims 1 tables live
in re/sims1_core_ref (a copy of the Sims 1 mod's core). Languages the Sims 1 mod did not have
(Czech, Hungarian, Russian) get nothing: those strings are spoken in English until translated.
"""
import importlib.util
import json
import os
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
REF = os.path.join(ROOT, "re", "sims1_core_ref")


def load_mod_strings(table, lang_dir):
    spec = importlib.util.spec_from_file_location("ms_" + str(abs(hash(table))), os.path.join(ROOT, "tools", "mod_strings.py"))
    m = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(m)
    m.TABLE = table
    m.LANG_DIR = lang_dir
    return m


def main(argv):
    group, ids = argv[0], argv[1:]
    ref = load_mod_strings(os.path.join(REF, "util", "strings_table.h"), os.path.join(REF, "lang"))
    cur = load_mod_strings(os.path.join(ROOT, "mod", "core", "util", "strings_table.h"), os.path.join(ROOT, "mod", "core", "lang"))
    english = {e["id"]: e["english"] for e in ref.read_english()}
    have = {e["id"] for e in cur.read_english()}
    missing = [i for i in ids if i not in english]
    if missing:
        raise SystemExit(f"not in the Sims 1 table: {missing}")
    new = [i for i in ids if i not in have]
    # Table: a group comment and the entries, before the final entry.
    path = cur.TABLE
    lines = open(path, encoding="utf-8").read().rstrip("\n").split("\n")
    last = lines.pop()  # the final entry has no continuation backslash
    add = [("    /* " + group + " */").ljust(119) + "\\"]
    for i in new:
        text = english[i].replace("\\", "\\\\").replace('"', '\\"').replace("\n", "\\n")
        add.append(f'    X({i}, "{text}")'.ljust(119) + "\\")
    lines = lines + add + [last]
    open(path, "w", encoding="utf-8", newline="\n").write("\n".join(lines) + "\n")
    # Translations.
    for lang in cur.LANGUAGES:
        old = ref.read_translation(lang) if os.path.exists(os.path.join(REF, "lang", lang + ".cpp")) else {}
        now = cur.read_translation(lang)
        for i in new:
            if i in old:
                now[i] = old[i]
        with tempfile.NamedTemporaryFile("w", suffix=".json", delete=False, encoding="utf-8") as f:
            json.dump(now, f, ensure_ascii=False)
        subprocess.run([sys.executable, os.path.join(ROOT, "tools", "mod_strings.py"), "write", lang, f.name],
                       capture_output=True)
        os.unlink(f.name)
    print(f"restored {len(new)} ids: {', '.join(new)}")


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
