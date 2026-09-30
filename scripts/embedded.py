import sys
import shutil
import subprocess
from pathlib import Path


def find_embedded(dir_build_embedded: str) -> str:
    """Return the first Embedded-* binary."""
    for p in Path(dir_build_embedded).iterdir():
        if p.is_file() and p.name.startswith("Embedded-"):
            return str(p)

    raise FileNotFoundError(
        f"No Embedded-* file found in {dir_build_embedded}"
    )


def stage_resources(dir_racine: str, dir_from: str) -> None:
    shutil.copytree(
        Path(dir_racine) / "std",
        Path(dir_from) / "std",
        copy_function=shutil.copy2,
    )

    data = Path(dir_from) / "data"
    data.mkdir()

    shutil.copy2(
        Path(dir_racine) / "grngame" / "input" / "gamecontrollerdb.txt",
        data / "gamecontrollerdb.txt",
    )


def cleanup(dir_from: str) -> None:
    dir_from = Path(dir_from)

    shutil.rmtree(dir_from / "std", ignore_errors=True)
    shutil.rmtree(dir_from / "data", ignore_errors=True)


def build_pak(embedded: str, dir_from: str) -> str:
    cmd = [
        str(Path(embedded).resolve()),
        "Assets.pak",
        "assets",
        "scripts",
        "std",
        "config",
        "data",
    ]

    subprocess.run(cmd, cwd=dir_from, check=True)
    return str(Path(dir_from) / "Assets.pak")

def bundle_runtime(dir_to: str) -> None:
    src = Path("build/Runtime")
    dst = Path(dir_to)

    shutil.copytree(
        src,
        dst,
        dirs_exist_ok=True,
        copy_function=shutil.copy2,
        ignore=shutil.ignore_patterns("*.exp", "*.lib"),
    )

if __name__ == "__main__":
    dir_from = sys.argv[1]
    dir_to = sys.argv[2]

    dir_racine = sys.argv[3]
    dir_build_embedded = sys.argv[4]

    embedded = find_embedded(dir_build_embedded)
    stage_resources(dir_racine, dir_from)
    bundle_runtime(dir_to)
    try:
        pak = build_pak(embedded, dir_from)
    finally:
        cleanup(dir_from)

    Path(dir_to).mkdir(parents=True, exist_ok=True)
    shutil.move(pak, str(Path(dir_to) / Path(pak).name))
