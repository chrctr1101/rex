#!/bin/bash

MOUNT_POINT="/mnt/reserve"

if [ "$1" = "-m" ]; then
    if ! read -r -p "Enter USB device name (like /dev/sda): " line; then
        echo "Error. No input. Exiting..." >&2
        exit 1
    fi
    if [ ! -e "$line" ]; then
        echo "Error. Device $line does not exist. Exiting..." >&2
        exit 1
    fi
    mkdir -p -- "$MOUNT_POINT" || {
        echo "Error. Cannot create $MOUNT_POINT" >&2
        exit 1
    }
    if ! sudo mount -- "$line" "$MOUNT_POINT"; then
        echo "Error. Failed to mount '$line' on $MOUNT_POINT" >&2
        exit 1
    fi

elif [ "$1" = "-u" ]; then
    if ! umount -- "$MOUNT_POINT"; then
        echo "Error. Failed to unmount $MOUNT_POINT" >&2
        exit 1
    fi

else
    echo "Usage: $0 -m|-u" >&2
    exit 1
fi

exit 0