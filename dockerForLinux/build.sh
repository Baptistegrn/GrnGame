#!/bin/bash
set -e
cd "$(dirname "$0")"
xmake c -y
xmake f -c -y -p linux -m release \
    --debug_mode=yes \
    --malloc_mode=yes \
    --cxxflags="-std=c++17" \
    -v
xmake