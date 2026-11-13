#!/bin/bash

if [ -f /etc/os-release ]; then
    . /etc/os-release
    DISTRO_ID="${ID,,}"
    DISTRO_NAME="${PRETTY_NAME:-$NAME}"
else
    echo "Error: cannot detect distribution (/etc/os-release not found)." >&2
    exit 1
fi

echo "Detected distribution: $DISTRO_NAME"
echo "Starting update..."

if [ "$DISTRO_ID" = "ubuntu" ] || [ "$DISTRO_ID" = "debian" ]; then
    sudo apt update && sudo apt upgrade -y

elif [ "$DISTRO_ID" = "fedora" ]; then
    sudo dnf upgrade --refresh -y

elif [ "$DISTRO_ID" = "arch" ] || [ "$DISTRO_ID" = "manjaro" ]; then
    sudo pacman -Syu --noconfirm

elif [ "$DISTRO_ID" = "void" ]; then
    sudo xbps-install -Su

elif [[ "${ID_LIKE,,}" == *"debian"* ]]; then
    sudo apt update && sudo apt upgrade -y

elif [[ "${ID_LIKE,,}" == *"fedora"* ]]; then
    sudo dnf upgrade --refresh -y

else
    echo "Error: automatic update for '$DISTRO_NAME' is not supported." >&2
    exit 1
fi

echo "Update complete."