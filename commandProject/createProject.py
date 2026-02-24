#!/usr/bin/env python3
import os
import sys
import shutil
import argparse

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(SCRIPT_DIR, ".."))

SRC_LUA = os.path.join(ROOT, "fichiers_projets")
BIN_DIR = os.path.join(ROOT, "bin")

parser = argparse.ArgumentParser(
    description="Creation of windows/linux/macos grngame projects"
)
parser.add_argument("name", help="Name of project")
parser.add_argument(
    "path",
    nargs="?",
    default=".",
    help="destination folder ,default : ."
)
parser.add_argument(
    "--level",
    default="debug",
    choices=["debug", "release"],
    help="Build lvl ,default : debug"
)

args = parser.parse_args()

project_root = os.path.abspath(os.path.join(args.path, args.name))

def mkdir(p):
    os.makedirs(p, exist_ok=True)

def copy(src, dst):
    if not os.path.exists(src):
        print(f"[!] Missing file : {src}")
        return
    shutil.copy(src, dst)
    print(f"[+] {src} -> {dst}")

mkdir(project_root)
mkdir(os.path.join(project_root, "src"))

# lua

copy(os.path.join(SRC_LUA, "main.lua"),
     os.path.join(project_root, "src"))

# autocompletion pour editeurs (VS Code, IntelliJ, etc.)
copy(os.path.join(SRC_LUA, "grngame_api.lua"),
     os.path.join(project_root, "src"))

copy(os.path.join(SRC_LUA, "encryptFiles.py"),
     os.path.join(project_root, "src"))

#luajit
#copy(os.path.join(SRC_LUA, "engine.lua"),
     #os.path.join(project_root, "windows"))

# Windows
win_bin = os.path.join(BIN_DIR, args.level)

copy(os.path.join(win_bin, "GrnGameAppWindows.exe"),
     project_root)

# Linux
copy(os.path.join(win_bin, "GrnGameAppLinux"),
     project_root)

# macOS
copy(os.path.join(win_bin, "GrnGameAppMacOs"),
     project_root)

print("\n\nProject create :", project_root)
