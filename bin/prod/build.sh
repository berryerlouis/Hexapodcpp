#!/usr/bin/env bash

set -euo pipefail

# Color output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo #######################
printf "${GREEN}Clang format...${NC}\n"
bin/dev/clang-format-all.sh
printf "${GREEN}Clang format.${NC}\n"
echo #######################

echo #######################
printf "${GREEN}Do Unit tests...${NC}\n"
bin/dev/test.sh all
printf "${GREEN}Unit tests done.${NC}\n"
echo #######################

echo #######################
printf "${GREEN}Building X64 sources RELEASE...${NC}\n"
bin/dev/build.sh X64 sources RELEASE
printf "${GREEN}Build X64 done.${NC}\n"
echo #######################

echo #######################
printf "${GREEN}Building RPI Cross compilation sources RELEASE...${NC}\n"
bin/dev/docker-build-rpi.sh RELEASE
printf "${GREEN}Build RPI Cross compilation done.${NC}\n"
echo #######################

echo #######################
printf "${GREEN}Updating UML...${NC}\n"
docker run --rm -v $PWD:/ws -w /ws plantuml/plantuml -tsvg images/architecture.puml
printf "${GREEN}Update UML done.${NC}\n"
echo #######################