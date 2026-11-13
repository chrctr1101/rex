#!/bin/bash
set -euo pipefail

cd -- "$(dirname -- "${BASH_SOURCE[0]}")" || exit 1

BIN_DIR="/usr/local/bin"
NAME="rex"

SUDO=""
[ "$(id -u)" -ne 0 ] && SUDO="sudo"

rm -f client/tools.o libtools.a rex

echo "Compiling tools..."
g++ -c client/tools.cpp -o client/tools.o || exit 1

echo "Archiving..."
ar rcs libtools.a client/tools.o || exit 1


echo "Linking..."
g++ client/main.cpp -L. -ltools -o rex || {
    echo "--- link failed, diagnostics ---" >&2
    ls -l libtools.a client/tools.o >&2 || true
    nm -C libtools.a >&2 || true
    exit 1
}

touch ips || exit 1

target="$(readlink -f -- rex)" || exit 1
chmod +x -- "$target" || exit 1

$SUDO ln -sf -- "$target" "$BIN_DIR/$NAME" || exit 1
echo "Installed: $BIN_DIR/$NAME -> $target"
