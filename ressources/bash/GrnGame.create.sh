#!/bin/bash

if [ -z "$1" ] || [ -z "$2" ]; then
    echo "Usage: $(basename "$0") <name> <destination>"
    exit 1
fi

SCRIPT_DIR="$(dirname "$(readlink -f "$0")")"

cp -r "$SCRIPT_DIR/../project_model" "$2"
mv "$2/project_model" "$2/$1"