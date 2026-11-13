#!/bin/bash

base="/mnt/reserve"
name="RESERVE[$(date +%d.%m.%Y)].tar.gz"

sudo ~/.tools/mount.sh -m || exit 1

tar -czf "$name" ~/important || exit 2
sudo mv ~/"$name" /mnt/reserve || exit 3

sudo ~/.tools/mount.sh -u || true
echo "OK: $base/$name"