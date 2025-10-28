#!/usr/bin/env bash
set -e

APP_DIR="/opt/cryptum"
BIN_LINK="/usr/local/bin/cryptum"

if [[ -L "$BIN_LINK" || -f "$BIN_LINK" ]]; then
    sudo rm -f "$BIN_LINK"
fi

if [[ -d "$APP_DIR" ]]; then
    sudo rm -rf "$APP_DIR"
fi

echo "Uninstallation done"