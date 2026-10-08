import sys
import shutil
import subprocess
from pathlib import Path

#todo factorise

def require_assets_bundle(path: Path) -> None:
    if not path.is_file():
        raise FileNotFoundError(f"Required file not found:{path},execute Embedded command to create {path}")

def require_file(path: Path) -> None:
    if not path.is_file():
        raise FileNotFoundError(f"Required file not found:{path},try to redownload release")

def  require_folder(path:Path)-> None:
       if not path.is_dir():
        raise FileNotFoundError(
            f"Required directory not found:\n  {path},try to redownload release"
        )
    
def clean_dist(dir_to: str) -> None:
    dist_path = Path(dir_to) / "dist"
    if dist_path.exists():
        shutil.rmtree(dist_path)

    (dist_path / "server").mkdir(parents=True)
    (dist_path / "linux-x86_64").mkdir()
    (dist_path / "linux-arm64").mkdir()
    (dist_path / "macos-x86_64").mkdir()
    (dist_path / "macos-arm64").mkdir()
    (dist_path / "windows-x64").mkdir()
    (dist_path / "windows-arm64").mkdir()
    (dist_path / "ios-arm64").mkdir()

def bundle_runtime(dir_from: str, dir_to: str) -> None:
    runtime_dir = Path(dir_from)
    dist_dir = Path(dir_to) / "dist"

    server_files = [
        "server.py",
        "Runtime-wasm-wasm32-releaseembedded.html",
        "Runtime-wasm-wasm32-releaseembedded.js",
        "Runtime-wasm-wasm32-releaseembedded.wasm",
    ]

    for filename in server_files:      
        src = runtime_dir/ filename
        require_file(src)     
        shutil.copy2(
            src,
            dist_dir / "server" / filename,
        )

    binaries = {
        "Runtime-linux-arm64-releaseembedded": "linux-arm64",
        "Runtime-linux-x86_64-releaseembedded": "linux-x86_64",
        "Runtime-macosx-arm64-releaseembedded": "macos-arm64",
        "Runtime-macosx-x86_64-releaseembedded": "macos-x86_64",
        "Runtime-windows-arm64-releaseembedded.exe": "windows-arm64",
        "Runtime-windows-x64-releaseembedded.exe": "windows-x64",
        "Runtime-iphoneos-arm64-releaseembedded": "ios-arm64"
    }

    for filename, platform in binaries.items():
        src = runtime_dir /filename
        require_file(src)
        shutil.copy2(
            src,
            dist_dir / platform / filename,
        )


    src = runtime_dir / "android-build"
    require_folder(src)

    shutil.copytree(
        src,
        dist_dir / "android",
        dirs_exist_ok=True,
        copy_function=shutil.copy2,
    )


def find_embedded(dir_to: str) -> Path:
    project_dir = Path(dir_to)

    for file in project_dir.glob("Embedded-*"):
        if file.is_file() and file.stat().st_mode & 0o111:
            return file

    embedded = project_dir / "embedded"

    if embedded.is_file() and embedded.stat().st_mode & 0o111:
        return embedded

    raise FileNotFoundError(
        f"Error: Embedded executable not found in:\n  {project_dir}"
    )


def bundle_assets(dir_to: str) -> None:
    project_dir = Path(dir_to)
    embedded_bin = find_embedded(dir_to)

    subprocess.run(
        [
            str(embedded_bin),
            str("Assets.pak"),
            str("assets"),
            str("scripts"),
            str("std"),
            str("data"),
            str("config"),
        ],
        check=True,
    )


def bundle_assets_pak(dir_to: str) -> None:
    project_dir = Path(dir_to)
    dist_dir = project_dir / "dist"
    assets_pak = project_dir / "Assets.pak"
    require_assets_bundle(assets_pak)
    
    platforms = [
        "server",
        "linux-x86_64",
        "linux-arm64",
        "macos-x86_64",
        "macos-arm64",
        "windows-x64",
        "windows-arm64",
        "ios-arm64",
    ]

    for platform in platforms:
        shutil.copy2(
            assets_pak,
            dist_dir / platform / "Assets.pak",
        )


if __name__ == "__main__":
    script_dir = Path(__file__).resolve().parent
    runtime_dir = script_dir.parent / "runtime"
    project_dir = Path.cwd()

    clean_dist(project_dir)
    bundle_runtime(runtime_dir, project_dir)
    bundle_assets(project_dir)
    bundle_assets_pak(project_dir)