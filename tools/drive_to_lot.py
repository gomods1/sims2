"""Drive the game from the neighborhood chooser into a lot, by keyboard through the mod.

  python drive_to_lot.py [neighborhood=Cala Belladonna] [family=Contreras]

Waits for the chooser, opens the neighborhood, accepts the welcome dialogs, opens the family's lot
from the mod's lot list and enters it. Reads Sims2Access.log to know where it is.
"""
import os
import subprocess
import sys
import time

HERE = os.path.dirname(os.path.abspath(__file__))
LOG = r"S:\steam\steamapps\common\The Sims 2 Legacy Collection\EP9\TSBin\Sims2Access\Sims2Access.log"


def spoken():
    with open(LOG, encoding="utf-8", errors="replace") as f:
        return [l.split("Spoke: ", 1)[1].strip() for l in f.read().splitlines() if "Spoke: " in l]


def keys(*k):
    subprocess.run([sys.executable, os.path.join(HERE, "keys.py"), *k])


def nav_to(key, text, limit=60, contains=False):
    """Presses key until the spoken item starts with text (after a group summary, if any)."""
    for _ in range(limit):
        keys(key)
        time.sleep(0.5)
        last = spoken()[-1] if spoken() else ""
        item = last.split(". ", 1)[1] if " elementos. " in last else last
        if item.startswith(text) or last.startswith(text) or (contains and text in last):
            print("found:", last)
            return True
    print("not found; last:", spoken()[-1] if spoken() else "")
    return False


def wait_for(text, seconds=240):
    start = len(spoken())
    deadline = time.time() + seconds
    while time.time() < deadline:
        lines = spoken()
        for l in lines[max(0, start - 1):]:
            if text in l:
                return l
        time.sleep(1)
    raise SystemExit(f"timed out waiting for {text!r}")


def main(argv):
    hood = argv[0] if argv else "Cala Belladonna"
    family = argv[1] if len(argv) > 1 else "Contreras"
    if not any("Elige un barrio" in l for l in spoken()[-12:]):
        wait_for("Elige un barrio")
    time.sleep(1)
    if "Elige un barrio" not in spoken()[-1]:
        nav_to("tab", "Elige un barrio", 6)
    keys("home")
    time.sleep(0.6)
    if hood not in spoken()[-1] and not nav_to("down", hood, 8):
        raise SystemExit("neighborhood not found")
    keys("enter")
    wait_for("Jugar", 30) if False else time.sleep(2)
    if not nav_to("down", "Jugar", 12):
        raise SystemExit("no Jugar button")
    keys("enter")
    # Welcome and expansion dialogs, then the neighborhood panel with the lot list.
    deadline = time.time() + 240
    while time.time() < deadline:
        time.sleep(3)
        last = spoken()[-1] if spoken() else ""
        if "lots," in last or "solares," in last:
            break
        if any(w in last for w in ("Aceptar", "Listo", "Cerrar", "Continuar")):
            if "Listo" not in last and "Siguiente" in last:
                keys("end")
            keys("enter")
        elif "Siguiente, botón" in last:
            keys("end")
            time.sleep(0.5)
            keys("enter")
    keys("home")
    time.sleep(0.6)
    if family not in spoken()[-1] and not nav_to("down", family, 60, contains=True):
        raise SystemExit("family's lot not found")
    keys("enter")
    time.sleep(2)
    if not nav_to("down", "Entrar", 12):
        raise SystemExit("no Entrar en solar")
    keys("enter")
    print("entering the lot")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
