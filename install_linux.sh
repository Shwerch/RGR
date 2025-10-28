#!/usr/bin/env bash
set -e

SRC_DIR="cmake-build-release"

APP_DIR="/opt/cryptum"
BIN_LINK="/usr/local/bin/cryptum"

sudo mkdir -p "$APP_DIR"
sudo cp ${SRC_DIR}/cryptum ${SRC_DIR}/lib*.so "$APP_DIR/"
sudo chmod 755 "$APP_DIR/cryptum"

sudo ln -sf "$APP_DIR/cryptum" "$BIN_LINK"

echo "Installation done"