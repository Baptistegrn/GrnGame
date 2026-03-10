import sys
import os
import shutil

def bundle_assets(dir_from: str, dir_to: str) -> None:
    shutil.copytree(dir_from, dir_to, dirs_exist_ok=True, 
                    copy_function=shutil.copy2)

if __name__ == '__main__':
    bundle_assets(sys.argv[1], sys.argv[2])