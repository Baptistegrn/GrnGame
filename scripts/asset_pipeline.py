import sys
import shutil
import os
from pathlib import Path


# Example: dir_from = "tests/pad_event"
def bundle_assets(dir_from: str, dir_to: str) -> None:
    shutil.copytree(
        dir_from,
        dir_to,
        dirs_exist_ok=True,
        copy_function=shutil.copy2,
    )


def bundle_std(dir_to: str) -> None:
    dest_path = os.path.join(dir_to, "std")
    
    shutil.copytree(
        "std",
        dest_path,
        dirs_exist_ok=True,
        copy_function=shutil.copy2,
    )

def bundle_controller_db(dir_to: str) -> None:
    src = Path("grngame/input")
    dst = Path(dir_to) / "data"

    shutil.copytree(
        src,
        dst,
        dirs_exist_ok=True,
        copy_function=shutil.copy2,
        ignore=shutil.ignore_patterns("*.c", "*.cpp", "*.h", "*.hpp"),
    )


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
    bundle_assets(dir_from, dir_to)
    bundle_std(dir_to)
    bundle_controller_db(dir_to)
    bundle_runtime(dir_to)