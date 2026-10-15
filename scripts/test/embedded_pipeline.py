import sys
import shutil
import subprocess
from pathlib import Path


def find_embedded(dir_build_embedded: str) -> str:
    """Return the first Embedded- binary."""
    for path in Path(dir_build_embedded).iterdir():
        if path.is_file() and path.name.startswith("Embedded-"):
            return str(path)

    raise FileNotFoundError(
        f"No Embedded-* file found in {dir_build_embedded}"
    )


def stage_resources(dir_racine: str, dir_from: str) -> None:
    root = Path(dir_racine)
    destination = Path(dir_from)

    shutil.copytree(
        root / "std",
        destination / "std",
        copy_function=shutil.copy2,
    )

    data_dir = destination / "data"
    data_dir.mkdir()

    shutil.copy2(
        root / "grngame" / "input" / "gamecontrollerdb.txt",
        data_dir / "gamecontrollerdb.txt",
    )

    shutil.copy2(
        root / "scripts" / "metadata" / "read_metadata.py",
        destination / "read_metadata.py",
    )


def cleanup(dir_from: str) -> None:
    directory = Path(dir_from)

    shutil.rmtree(directory / "std", ignore_errors=True)
    shutil.rmtree(directory / "data", ignore_errors=True)
    (directory / "read_metadata.py").unlink(missing_ok=True)


def build_pak(embedded: str, dir_from: str) -> str:
    command = [
        str(Path(embedded).resolve()),
        "Assets.pak",
        "assets",
        "scripts",
        "std",
        "config",
        "data",
    ]

    subprocess.run(command, cwd=dir_from, check=True)

    return str(Path(dir_from) / "Assets.pak")


def bundle_runtime(dir_to: str) -> None:
    source = Path("build/Runtime")
    destination = Path(dir_to)

    shutil.copytree(
        source,
        destination,
        dirs_exist_ok=True,
        copy_function=shutil.copy2,
        ignore=shutil.ignore_patterns("*.exp", "*.lib"),
    )


def bundle_metadata(dir_from: str, dir_to: str) -> None:
    source = Path(dir_from) / "metadata.json"
    destination = Path(dir_to) / "metadata.json"

    shutil.copy2(source, destination)


if __name__ == "__main__":
    dir_from = sys.argv[1]
    dir_to = sys.argv[2]
    dir_racine = sys.argv[3]
    dir_build_embedded = sys.argv[4]

    embedded = find_embedded(dir_build_embedded)

    stage_resources(dir_racine, dir_from)
    bundle_runtime(dir_to)
    bundle_metadata(dir_from, dir_to)

    try:
        pak = build_pak(embedded, dir_from)
    finally:
        cleanup(dir_from)

    destination = Path(dir_to)
    destination.mkdir(parents=True, exist_ok=True)

    shutil.move(
        pak,
        str(destination / Path(pak).name),
    )