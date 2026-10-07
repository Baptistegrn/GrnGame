import shutil
import sys
from pathlib import Path

ARTIFACTS_DIR = Path("artifacts")
OUTPUT_DIR = Path("release-assets")

PLATFORMS = [
    "linux-x64",
    "linux-arm64",
    "macos-x64",
    "macos-arm64",
    "windows-x64",
    "windows-arm64",
]

RUNTIME_PLATFORMS = PLATFORMS + ["wasm"]
ANDROID_ABIS = ["arm64-v8a", "x86_64"]

# ignored files 
IGNORED_FILES = shutil.ignore_patterns("*.lib", "*.exp", "*.pdb", "*.a", "*.ilk")


def copy_folder(src: Path, dst: Path, ignore=None) -> None:
    shutil.copytree(src, dst, dirs_exist_ok=True, copy_function=shutil.copy2, ignore=ignore)


def copy_file(src: Path, dst_folder: Path) -> None: 
    dst_folder.mkdir(parents=True, exist_ok=True)
    shutil.copy2(src, dst_folder)


def warn(message: str) -> None:
    print(f"::warning::{message}")


def bundle_project_model(stage_dir: Path) -> None:
    model_dir = stage_dir / "project_model"

    for name in ("assets", "data", "config"):
        (model_dir / name).mkdir(parents=True, exist_ok=True)

    copy_file(Path("ressources/config.json"), model_dir / "config")
    copy_file(Path("grngame/input/gamecontrollerdb.txt"), model_dir / "data")
    copy_file(Path("scripts/server.py"), model_dir)
    copy_folder(Path("std"), model_dir / "std")


def bundle_setup_scripts(platform: str, stage_dir: Path) -> None:
    if "windows" in platform:
        scripts_src = Path("ressources/batch")
        setup_name = "setup.bat"
    else:
        scripts_src = Path("ressources/bash")
        setup_name = "setup.sh"

    scripts_dst = stage_dir / "scripts"

    copy_file(scripts_src / setup_name, stage_dir)

    for script in scripts_src.iterdir():
        if script.is_file() and script.name != setup_name:
            copy_file(script, scripts_dst)

    scripts_dst.mkdir(parents=True, exist_ok=True)


def bundle_desktop_binary(platform: str, stage_dir: Path) -> None:
    # only runtime not embedded
    src = ARTIFACTS_DIR / f"{platform}-release-embedfalse"

    ignored_extensions = {".lib", ".exp", ".pdb", ".a", ".ilk"}

    for file in src.rglob("*"):
        if file.is_file() and file.suffix not in ignored_extensions:
            copy_file(file, stage_dir / "project_model")


def bundle_runtimes(stage_dir: Path) -> None:
    #bundle final runtime (embedded)
    runtime_dir = stage_dir / "runtime"
    runtime_dir.mkdir(parents=True, exist_ok=True)

    for platform in RUNTIME_PLATFORMS:
        artifact = ARTIFACTS_DIR / f"{platform}-release-embedtrue"

        if not artifact.is_dir():
            warn(f"Artefact manquant : {artifact}")
            continue

        # desktop :Runtime/  wasm : .
        src = artifact / "Runtime"
        if not src.is_dir():
            src = artifact

        copy_folder(src, runtime_dir, ignore=IGNORED_FILES)


def bundle_android(stage_dir: Path) -> None:
    android_src = Path("android-build")
    android_dst = stage_dir / "runtime" / "android-build"

    copy_folder(android_src, android_dst)

    for abi in ANDROID_ABIS:
        lib = ARTIFACTS_DIR / f"android-{abi}-release" / "libGrnGame.so"

        if not lib.is_file():
            continue

        copy_file(lib, android_dst / "app" / "jni" / "src" / abi)


def make_zip(platform: str, stage_dir: Path, tag: str) -> None:
    OUTPUT_DIR.mkdir(exist_ok=True)
    archive = OUTPUT_DIR / f"{platform}-{tag}"
    shutil.make_archive(str(archive), "zip", root_dir=stage_dir)


def package(platform: str, tag: str) -> None:
    print(f"::group::Packaging {platform}")

    stage_dir = Path(f"staging_{platform}")
    stage_dir.mkdir(exist_ok=True)

    bundle_project_model(stage_dir)
    bundle_setup_scripts(platform, stage_dir)
    bundle_desktop_binary(platform, stage_dir)
    bundle_runtimes(stage_dir)
    bundle_android(stage_dir)

    make_zip(platform, stage_dir, tag)
    shutil.rmtree(stage_dir)

    print("::endgroup::")


if __name__ == "__main__":
    tag = sys.argv[1]

    for platform in PLATFORMS:
        package(platform, tag)