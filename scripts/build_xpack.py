import subprocess
import os
import sys
from pathlib import Path

if __name__ == "__main__":
    tag = sys.argv[1]
    stage_dir = Path(f"staging_{platform}")
    output_dir = Path("release-assets")
    output_dir.mkdir(exist_ok=True)

    if not stage_dir.exists():
        sys.exit(1)

    env = os.environ.copy()
    env["GRNGAME_STAGE"] = str(stage_dir)
    env["GRNGAME_VERSION"] = tag

    print("::group::XPack")
    subprocess.run(
        ["xmake", "pack", "-o", str(output_dir), "-y"],
        env=env, check=True,
    )
    print("::endgroup::")