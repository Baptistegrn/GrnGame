import shutil
import sys
from pathlib import Path

ARTIFACTS_DIR = Path("artifacts")
OUTPUT_DIR = Path("release-assets")

#releases : only for dev 
PLATFORMS = [
    "linux-x64",
    "linux-arm64",
    "macos-x64",
    "macos-arm64",
    "windows-x64",
    "windows-arm64",
]



# desktop artefacts : runtime,embedded binary 
DESKTOP_ARTIFACTS = {
    "linux-x64": "linux-x64-release-embedfalse",
    "linux-arm64": "linux-arm64-release-embedfalse",
    "macos-x64": "macos-x64-release-embedfalse",
    "macos-arm64": "macos-arm64-release-embedfalse",
    "windows-x64": "windows-x64-release-embedfalse",
    "windows-arm64": "windows-arm64-release-embedfalse",
}

# runtimes embedded going to move to runtime/
RUNTIME_ARTIFACTS = [
    "linux-x64-release-embedtrue",
    "linux-arm64-release-embedtrue",
    "macos-x64-release-embedtrue",
    "macos-arm64-release-embedtrue",
    "windows-x64-release-embedtrue",
    "windows-arm64-release-embedtrue",
    "wasm-release-embedtrue",
    "ios-arm64-release-embedtrue",
]

# android : x2 .so files 
ANDROID_ARTIFACTS = {
    "arm64-v8a": "android-arm64-v8a-release",
    "x86_64": "android-x86_64-release",
}

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
    runtime_dir = stage_dir / "runtime"
    for name in ("assets", "data", "config"):
        (model_dir / name).mkdir(parents=True, exist_ok=True)

    copy_file(Path("ressources/config.json"), model_dir / "config")
    copy_file(Path("grngame/input/gamecontrollerdb.txt"), model_dir / "data")
    copy_file(Path("scripts/server.py"), runtime_dir)
    copy_folder(Path("std"), model_dir / "std")


def bundle_setup_scripts(stage_dir: Path) -> None:
    for name in ("GrnGameCreate.py", "GrnGameDist.py"):
        copy_file(Path("scripts") / name, stage_dir / "scripts")


def bundle_desktop_binary(platform: str, stage_dir: Path) -> None:
    # runtime and embedded binary in the same folder
    src = ARTIFACTS_DIR / DESKTOP_ARTIFACTS[platform]
    ignored_extensions = {".lib", ".exp", ".pdb", ".a", ".ilk"}
    for file in src.rglob("*"):
        if file.is_file() and file.suffix not in ignored_extensions:
            copy_file(file, stage_dir / "project_model")


def bundle_runtimes(stage_dir: Path) -> None:
    # bundle final runtime (embedded)
    runtime_dir = stage_dir / "runtime"
    runtime_dir.mkdir(parents=True, exist_ok=True)

    for artifact_name in RUNTIME_ARTIFACTS:
        # ex: artifacts/ios-arm64-release-embedtrue/
        artifact = ARTIFACTS_DIR / artifact_name

        if not artifact.is_dir():
            continue

        # desktop : Runtime/   wasm / ios : .
        src = artifact / "Runtime"
        if not src.is_dir():
            src = artifact

        copy_folder(src, runtime_dir, ignore=IGNORED_FILES)


def bundle_android(stage_dir: Path) -> None:
    android_src = Path("android-build")
    android_dst = stage_dir / "runtime" / "android-build"

    copy_folder(android_src, android_dst)

    for abi, artifact_name in ANDROID_ARTIFACTS.items():
        # ex: artifacts/android-arm64-v8a-release/libGrnGame.so
        lib = ARTIFACTS_DIR / artifact_name / "libGrnGame.so"

        if not lib.is_file():
            warn(f"Lib android manquante : {lib}")
            continue

        # every abi have its own folder : app/jni/src/<abi>/libGrnGame.so
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
    bundle_setup_scripts( stage_dir)
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