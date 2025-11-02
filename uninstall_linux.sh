#!/usr/bin/env bash
set -euo pipefail

APP_NAME="cryptum"
APP_DIR="/opt/$APP_NAME"
BIN_LINK="/usr/local/bin/$APP_NAME"

if [ "${EUID:-$(id -u)}" -ne 0 ]; then
  SUDO=sudo
else
  SUDO=
fi

$SUDO rm -f "$BIN_LINK"
$SUDO rm -rf "$APP_DIR"

echo "Uninstallation done"
