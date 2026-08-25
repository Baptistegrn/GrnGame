#!/usr/bin/env bash

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RUNTIME_DIR="$(cd "$SCRIPT_DIR/../runtime" && pwd)"
PROJECT_DIR="$(pwd)"
DIST_DIR="$PROJECT_DIR/dist"

rm -rf "$DIST_DIR"

mkdir -p "$DIST_DIR/server"
mkdir -p "$DIST_DIR/linux-x86_64"
mkdir -p "$DIST_DIR/linux-arm64"
mkdir -p "$DIST_DIR/macos-x86_64"
mkdir -p "$DIST_DIR/macos-arm64"
mkdir -p "$DIST_DIR/windows-x64"
mkdir -p "$DIST_DIR/windows-arm64"

cp "$RUNTIME_DIR/server.py" \
   "$DIST_DIR/server/"

cp "$RUNTIME_DIR/Runtime-wasm-wasm32-releaseembedded.html" \
   "$DIST_DIR/server/"

cp "$RUNTIME_DIR/Runtime-wasm-wasm32-releaseembedded.js" \
   "$DIST_DIR/server/"

cp "$RUNTIME_DIR/Runtime-wasm-wasm32-releaseembedded.wasm" \
   "$DIST_DIR/server/"

cp "$RUNTIME_DIR/Runtime-linux-arm64-releaseembedded" \
   "$DIST_DIR/linux-arm64/"

cp "$RUNTIME_DIR/Runtime-linux-x86_64-releaseembedded" \
   "$DIST_DIR/linux-x86_64/"

cp "$RUNTIME_DIR/Runtime-macosx-arm64-releaseembedded" \
   "$DIST_DIR/macos-arm64/"

cp "$RUNTIME_DIR/Runtime-macosx-x86_64-releaseembedded" \
   "$DIST_DIR/macos-x86_64/"

cp "$RUNTIME_DIR/Runtime-windows-arm64-releaseembedded.exe" \
   "$DIST_DIR/windows-arm64/"

cp "$RUNTIME_DIR/Runtime-windows-x64-releaseembedded.exe" \
   "$DIST_DIR/windows-x64/"



EMBEDDED_BIN=""

for file in "$PROJECT_DIR"/Embedded-*; do
    if [ -f "$file" ] && [ -x "$file" ]; then
        EMBEDDED_BIN="$file"
        break
    fi
done

if [ -z "$EMBEDDED_BIN" ] && [ -x "$PROJECT_DIR/embedded" ]; then
    EMBEDDED_BIN="$PROJECT_DIR/embedded"
fi

if [ -z "$EMBEDDED_BIN" ]; then
    echo "Error: Embedded executable not found in:"
    echo "  $PROJECT_DIR"
    exit 1
fi

"$EMBEDDED_BIN" \
    "$PROJECT_DIR/Assets.pak" \
    "$PROJECT_DIR/assets" \
    "$PROJECT_DIR/scripts" \
    "$PROJECT_DIR/std" \
    "$PROJECT_DIR/data" \
    "$PROJECT_DIR/config"

cp "$PROJECT_DIR/Assets.pak" "$DIST_DIR/server/"
cp "$PROJECT_DIR/Assets.pak" "$DIST_DIR/linux-x86_64/"
cp "$PROJECT_DIR/Assets.pak" "$DIST_DIR/linux-arm64/"
cp "$PROJECT_DIR/Assets.pak" "$DIST_DIR/macos-x86_64/"
cp "$PROJECT_DIR/Assets.pak" "$DIST_DIR/macos-arm64/"
cp "$PROJECT_DIR/Assets.pak" "$DIST_DIR/windows-x64/"
cp "$PROJECT_DIR/Assets.pak" "$DIST_DIR/windows-arm64/"