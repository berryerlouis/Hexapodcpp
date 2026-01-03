#!/usr/bin/env bash
set -euo pipefail

# Color output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color


echo ""
printf  "${GREEN}Deploy to Raspberry Pi:${NC}\n"
echo "  Stopping running process on hexabot..."
ssh hexabot "sudo systemctl stop hexabot.service 2>/dev/null; sudo pkill -9 gdbserver 2>/dev/null; sudo killall -9 Hexapodcpp 2>/dev/null; sleep 0.5; true"
echo "  Copying binary..."
scp Hexapodcpp hexabot:/home/hexabot/
printf ho " ${GREEN}Deployment complete!${NC}\n"
echo ""

