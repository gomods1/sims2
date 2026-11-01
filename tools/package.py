"""Builds a release of the mod (ported from the Sims 1 mod's tools/package.py).

    python tools/package.py v0.1.0 [--no-build] [--no-installer]

1. tools/build_docs.py --check: a manual translation that drifted from en.md stops the release
2. mod\\build.bat (release files in mod\\out; no deploy, so no dev.txt)
3. mod\\out\\Sims2Access-<tag>.zip, laid out like the game's EP9\\TSBin folder:
       version.dll                    (loader, next to Sims2EP9.exe)
       Sims2Access\\Sims2Access.dll    (core)
       Sims2Access\\SpeechHost.exe + prism.dll
       Sims2Access\\sounds\\*.wav       (the cursors' sound cues)
       Sims2Access\\docs\\<tag>.html    (the manual in every language, tools/build_docs.py)
       Sims2Access\\licenses\\...       (MinHook, Prism and its parts)
   and mod\\out\\Sims2Access-<tag>.map, the core's map for crash offsets of that release.
4. ..\\installer\\build_offline_sims2.bat with that zip, when it exists (the shared installer has
   no Sims 2 entry yet: ROADMAP.md).
"""
import argparse
import re
import shutil
import subprocess
import sys
import zipfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "mod" / "out"
PRISM = Path(r"C:\Users\Nitropc\code\libs\prism\prism-windows-x64")
INSTALLER = ROOT.parent / "installer"

FILES = [  # (source, path in the zip)
    (OUT / "version.dll", "version.dll"),
    (OUT / "Sims2Access.dll", "Sims2Access/Sims2Access.dll"),
    (OUT / "SpeechHost.exe", "Sims2Access/SpeechHost.exe"),
    (OUT / "prism.dll", "Sims2Access/prism.dll"),
    (ROOT / "mod" / "third_party" / "minhook" / "LICENSE.txt", "Sims2Access/licenses/MinHook.txt"),
    (PRISM / "NOTICE", "Sims2Access/licenses/Prism/NOTICE"),
]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("tag", help="release tag, e.g. v0.1.0")
    ap.add_argument("--no-build", action="store_true", help="package mod\\out as it is")
    ap.add_argument("--no-installer", action="store_true", help="only make the zip")
    args = ap.parse_args()
    if not re.fullmatch(r"v\d+\.\d+\.\d+", args.tag):
        sys.exit("tag must look like v1.2.3")

    # The manual must say what the mod does in every language: a translation whose sections,
    # tables, lists or key combinations differ from en.md stops the release.
    subprocess.run([sys.executable, str(ROOT / "tools" / "build_docs.py"), "--check"], check=True)
    if not args.no_build:
        subprocess.run(["cmd", "/c", str(ROOT / "mod" / "build.bat")], check=True)

    files = list(FILES)
    for f in sorted((ROOT / "mod" / "sounds").glob("*.wav")):  # the cursors' sound cues
        files.append((f, "Sims2Access/sounds/" + f.name))
    for f in sorted((OUT / "docs").glob("*.html")):  # the manual, one page per language
        files.append((f, "Sims2Access/docs/" + f.name))
    for f in sorted((PRISM / "LICENSES").rglob("*")):
        if f.is_file():
            files.append((f, "Sims2Access/licenses/Prism/" + f.relative_to(PRISM / "LICENSES").as_posix()))
    missing = [str(src) for src, _ in files if not src.is_file()]
    if missing:
        sys.exit("missing: " + ", ".join(missing))

    zip_path = OUT / f"Sims2Access-{args.tag}.zip"
    with zipfile.ZipFile(zip_path, "w", zipfile.ZIP_DEFLATED) as z:
        for src, name in files:
            z.write(src, name)
    shutil.copyfile(OUT / "Sims2Access.map", OUT / f"Sims2Access-{args.tag}.map")
    print(f"{zip_path} ({zip_path.stat().st_size:,} bytes, {len(files)} files)")

    script = INSTALLER / "build_offline_sims2.bat"
    if args.no_installer:
        return
    if not script.is_file():
        print(f"note: no {script}; the shared installer has no Sims 2 entry yet, only the zip was made")
        return
    subprocess.run(["cmd", "/c", str(script), str(zip_path), args.tag], check=True)


if __name__ == "__main__":
    main()
