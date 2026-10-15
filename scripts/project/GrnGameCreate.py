import sys
import shutil
from pathlib import Path

def  require_folder(path:Path)-> None:
       if not path.is_dir():
        raise FileNotFoundError(
            f"Required directory not found:\n  {path},try to redownload release"
        )

def bundle_project(name: str, dir_to: str) -> None:
    script_dir = Path(__file__).resolve().parent
    src = script_dir.parent / "project_model"

    require_folder(src)
    
    dst = Path(dir_to) / name
    shutil.copytree(
        src,
        dst,
        dirs_exist_ok=True,
        copy_function=shutil.copy2,
    )

if __name__ == "__main__":
    name = sys.argv[1]
    dir_to = sys.argv[2]
    bundle_project(name, dir_to)