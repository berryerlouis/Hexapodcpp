#!/usr/bin/env bash
set -euo pipefail

# Color output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

PI_HOST="hexabot"
TARGET_BIN="/home/hexabot/Hexapodcpp"
PORT=2345

printf "${GREEN}Start GDB on Raspberry Pi:${NC}\n"

ssh "$PI_HOST" bash -c "'
    pkill gdbserver 2>/dev/null || true
    echo \"Launching gdbserver on port $PORT...\"
    nohup gdbserver :$PORT $TARGET_BIN >/tmp/gdbserver.log 2>&1 &
    disown
'"
#!/usr/bin/env bash
set -euo pipefail

PI_HOST="hexabot"
TARGET_BIN="/home/hexabot/Hexapodcpp"
PORT=2345

echo "Starting gdbserver on Raspberry Pi..."

ssh "$PI_HOST" bash -c "'
    # Kill any previous gdbserver
    pkill gdbserver 2>/dev/null || true

    # Start gdbserver fully detached
    sudo nohup gdbserver :$PORT $TARGET_BIN \
        >/tmp/gdbserver.log 2>&1 < /dev/null &
'"
printf "${GREEN}GDB server started.\n"
