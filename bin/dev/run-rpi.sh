#!/usr/bin/env bash
set -euo pipefail

# Color output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo ""
printf "${GREEN}Run Raspberry Pi:${NC}\n"
ssh hexabot sudo "systemctl start hexabot.service"
printf "${GREEN}Service started!${NC}\n"
echo ""
