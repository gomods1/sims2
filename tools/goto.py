"""Move the mod's focus to an item and optionally press it, reading what the mod speaks.

  python goto.py <group text> <item text> [--enter] [--tabs 14] [--moves 40]

Presses Tab until the spoken group line contains <group text> (a group's summary starts with its
name; a group of one item is announced by the item alone, which matches <item text> directly),
then Home and Right until the spoken item's name contains <item text>. Groups sharing a name are
tried in turn. Presses Enter only when the item was found; exits 1 otherwise (nothing pressed).
Matching is case-insensitive. Reads Sims2Access.log like nav_to.py.
"""
import os
import subprocess
import sys
import time

LOG = r"S:\steam\steamapps\common\The Sims 2 Legacy Collection\EP9\TSBin\Sims2Access\Sims2Access.log"
HERE = os.path.dirname(os.path.abspath(__file__))


def spoken() -> list:
    with open(LOG, encoding="utf-8", errors="replace") as f:
        return [l.split("Spoke:", 1)[1].strip() for l in f if "Spoke:" in l]


def key(name: str):
    subprocess.run([sys.executable, os.path.join(HERE, "keys.py"), name])
    time.sleep(0.45)


WHOLE = False  # match the whole item line (the text looked for has a comma)


def name_of(line: str) -> str:
    """The item's name in a spoken line (after a group summary "group, N items. ", if any)."""
    parts = line.split(". ", 1)
    if len(parts) == 2 and "," in parts[0]:
        line = parts[1]
    return line.lower() if WHOLE else line.split(", ", 1)[0].lower()


def press_if(enter: bool, line: str) -> bool:
    print("found:", line)
    if enter:
        key("enter")
    return True


def find_item(item: str, moves: int, enter: bool) -> bool:
    key("home")
    last = None
    for _ in range(moves):
        line = spoken()[-1] if spoken() else ""
        if item in name_of(line):
            return press_if(enter, line)
        n = len(spoken())
        key("right")
        new = spoken()[n:]
        if not new or new[-1] == last:
            return False  # the end of the group repeats its last item
        last = new[-1]
    return False


def main(argv):
    args = [a for a in argv if not a.startswith("--")]
    global WHOLE
    group, item = args[0].lower(), args[1].lower()
    WHOLE = "," in item
    enter = "--enter" in argv
    tabs = int(argv[argv.index("--tabs") + 1]) if "--tabs" in argv else 14
    moves = int(argv[argv.index("--moves") + 1]) if "--moves" in argv else 40
    for _ in range(tabs):
        n = len(spoken())
        key("tab")
        new = spoken()[n:]
        if not new:
            continue
        line = new[-1]
        if item in name_of(line) and "," not in line.split(". ", 1)[0][:0]:
            return 0 if press_if(enter, line) else 1
        if any(l.lower().startswith(group) for l in new) and find_item(item, moves, enter):
            return 0
    print("not found:", group, "/", item)
    return 1


if __name__ == "__main__":
    sys.stdout.reconfigure(encoding="utf-8")
    sys.exit(main(sys.argv[1:]))
