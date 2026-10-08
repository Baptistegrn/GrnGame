import os
import subprocess
import sys
from pathlib import Path

OS = {"Linux": "linux", "Windows": "windows", "macOS": "macos"}
ARCH = {"X64": "x64", "ARM64": "arm64"}

if __name__ == "__main__":
    tag = sys.argv[1]
    platform = f"{OS[os.environ['RUNNER_OS']]}-{ARCH[os.environ['RUNNER_ARCH']]}"

    stage_dir = Path(f"staging_{platform}")
    output_dir = Path("release-assets")
    output_dir.mkdir(exist_ok=True)

    if not stage_dir.exists():
        print(f"Dossier introuvable : {stage_dir}")
        sys.exit(1)

    env = os.environ.copy()
    env["GRNGAME_STAGE"] = str(stage_dir.resolve())
    env["GRNGAME_VERSION"] = tag.lstrip("v")

    print("::group::XPack")
    subprocess.run(
        ["xmake", "pack", "-o", str(output_dir.resolve()), "-y"],
        env=env, cwd="pack", check=True,
    )
    print("::endgroup::")

    # delete somes extensions
    for f in output_dir.glob("*.wixpdb"):
        f.unlink()