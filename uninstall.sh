#!/bin/bash
set -euo pipefail

LINK="/usr/local/bin/rex"

SUDO=""
[ "$(id -u)" -ne 0 ] && SUDO="sudo"

[ -L "$LINK" ] || { echo "Error: '$LINK' is not a symlink" >&2; exit 1; }
$SUDO rm -f -- "$LINK" || exit 1
echo "Removed: $LINK"

cd ~/rex
rm ips libtools.a client/tools.o
echo "Deleted staff files"

rm rex
echo "Deleted binary file"

echo "rex is deleted"
