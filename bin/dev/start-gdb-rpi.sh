#!/usr/bin/env bash
set -euo pipefail

# Color output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo ""
printf "${GREEN}Start GDB on Raspberry Pi:${NC}\n"
ssh hexabot command -v gdbserver >/dev/null || pkill gdbserver 2>/dev/null || true; sudo gdbserver :2345 /home/hexabot/Hexapodcpp
printf "${GREEN}GDB started!${NC}\n"
echo ""
