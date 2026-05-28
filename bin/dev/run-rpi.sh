#!/usr/bin/env bash
set -euo pipefail

# Color output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo ""
printf "${GREEN}Run Raspberry Pi:${NC}\n"
# Ensure no other process holds pigpio (daemon or stale pid) before launching.
ssh hexabot "sudo systemctl stop pigpiod 2>/dev/null; sudo killall -9 pigpiod 2>/dev/null; sudo rm -f /var/run/pigpio.pid; true"
ssh hexabot sudo "systemctl start hexabot.service"
printf "${GREEN}Service started!${NC}\n"
echo ""
