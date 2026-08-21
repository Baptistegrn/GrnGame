rm -rf dist/
mkdir -p dist/server

PLATFORMS=("linux-x64" "linux-arm64" "macos-x64" "macos-arm64" "windows-x64" "windows-arm64")


for plat in "${PLATFORMS[@]}"; do
    mkdir -p "dist/$plat"
    cp runtime/*"$plat"* "dist/$plat/" 2>/dev/null || true
done


EMBEDDED_BIN=$(find . -maxdepth 1 -name "Embedded-*" -type f -executable | head -n 1)
if [ -z "$EMBEDDED_BIN" ]; then
    EMBEDDED_BIN="./embedded"
fi

$EMBEDDED_BIN Assets.pak assets scripts std data config

for plat in "${PLATFORMS[@]}"; do
    cp Assets.pak "dist/$plat/"
done

cp runtime/server.py dist/server/
cp Assets.pak dist/server/

cp runtime/*.js runtime/*.wasm runtime/*.html dist/server/ 2>/dev/null || true

