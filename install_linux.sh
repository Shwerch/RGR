#!/usr/bin/env bash
set -euo pipefail

SRC_DIR="linux-build-release"
APP_NAME="cryptum"
APP_DIR="/opt/$APP_NAME"
BIN_LINK="/usr/local/bin/$APP_NAME"

if [ "${EUID:-$(id -u)}" -ne 0 ]; then
  SUDO=sudo
else
  SUDO=
fi

if ! command -v cmake >/dev/null 2>&1; then
  echo "Cmake not found" >&2
  exit 1
elif ! command -v make >/dev/null 2>&1; then
  echo "Make not found" >&2
  exit 1
fi

mkdir -p "$SRC_DIR"
cd "$SRC_DIR"
cmake -DCMAKE_BUILD_TYPE=Release ..
make

BUILD_BIN="./$APP_NAME"
if [ ! -x "$BUILD_BIN" ]; then
  BUILD_BIN=$(find . -type f -name "$APP_NAME" -perm /111 -print -quit 2>/dev/null || true)
fi

$SUDO mkdir -p "$APP_DIR"
$SUDO cp "$BUILD_BIN" "$APP_DIR/"

LIBS=$(printf "%s\n" lib*.so* 2>/dev/null || true)
if [ -n "$LIBS" ]; then
  for f in $LIBS; do
    if [ -f "$f" ]; then
      $SUDO cp "$f" "$APP_DIR/" || true
    fi
  done
fi

$SUDO chmod 755 "$APP_DIR/$APP_NAME"

if [ -e "$BIN_LINK" ] || [ -L "$BIN_LINK" ]; then
  $SUDO rm -f "$BIN_LINK"
fi

$SUDO bash -c "cat > '$BIN_LINK' <<'EOF'
#!/usr/bin/env bash
DIR='$APP_DIR'
export LD_LIBRARY_PATH=\"\$DIR:\${LD_LIBRARY_PATH:-}\"
exec \"\$DIR/$APP_NAME\" \"\$@\"
EOF"
$SUDO chmod 755 "$BIN_LINK"

echo "Installation done"
