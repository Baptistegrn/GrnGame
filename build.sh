#!/bin/bash
set -e
cd "$(dirname "$0")"
rm -rf .xmake
rm -rf build
rm -rf dist
xmake f -c -y -p linux -m release --cxxflags="-std=c++17" -v
xmake