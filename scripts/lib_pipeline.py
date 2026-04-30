import sys
import os
import shutil

def bundle_lib(src: str, dst: str) -> None:
    if os.path.isdir(src):
        shutil.copytree(src, dst, dirs_exist_ok=True, copy_function=shutil.copy2)
        return

    os.makedirs(dst, exist_ok=True)
    shutil.copy2(src, os.path.join(dst, os.path.basename(src)))

if __name__ == '__main__':
    bundle_lib(sys.argv[1], sys.argv[2])