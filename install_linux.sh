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

mkdir -p "$SRC_DIR"
cd "$SRC_DIR"

if ! command -v cmake >/dev/null 2>&1; then
  if command -v apt-get >/dev/null 2>&1; then
    $SUDO apt-get update
    $SUDO apt-get install -y cmake build-essential
  elif command -v dnf >/dev/null 2>&1; then
    $SUDO dnf install -y cmake make gcc-c++
  elif command -v yum >/dev/null 2>&1; then
    $SUDO yum install -y cmake make gcc-c++
  elif command -v pacman >/dev/null 2>&1; then
    $SUDO pacman -Sy --noconfirm cmake base-devel
  elif command -v zypper >/dev/null 2>&1; then
    $SUDO zypper --non-interactive install cmake gcc make gcc-c++
  fi
fi

cmake -DCMAKE_BUILD_TYPE=Release ..
MAKE_JOBS=1
if command -v nproc >/dev/null 2>&1; then
  MAKE_JOBS=$(nproc)
fi
make -j"$MAKE_JOBS"

BUILD_BIN="./$APP_NAME"
if [ ! -x "$BUILD_BIN" ]; then
  BUILD_BIN=$(find . -type f -name "$APP_NAME" -perm /111 -print -quit 2>/dev/null || true)
fi
if [ -z "${BUILD_BIN:-}" ]; then
  echo "build failed: binary not found" >&2
  exit 1
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
