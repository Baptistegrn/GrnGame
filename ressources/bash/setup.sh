#!/usr/bin/env bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIPTS_DIR="$SCRIPT_DIR/scripts"

export PATH="$SCRIPTS_DIR:$PATH"

echo "Added to PATH:"
echo "  $SCRIPTS_DIR"
echo

echo "Available commands:"

for script in "$SCRIPTS_DIR"/*; do
    [ -e "$script" ] || continue
    echo "  $(basename "$script")"
done