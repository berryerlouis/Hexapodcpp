#!/usr/bin/env bash
set -euo pipefail

# Build script for Hexapodcpp
# Usage: bin/dev/build.sh [TARGET] [MODE] [BUILD_TYPE] [install]
#   TARGET:     X64 | RPI                (default: RPI)
#   MODE:       sources | test           (default: sources)
#   BUILD_TYPE: DEBUG | RELEASE | CLEAN  (default: DEBUG, sources mode only)
#               CLEAN removes the DEBUG and RELEASE build directories of TARGET.
#   install:    RPI only, builds and installs WiringPi system-wide (native build on the Pi)

# Color output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

TARGET="${1:-RPI}"
MODE="${2:-sources}"
BUILD_TYPE="${3:-DEBUG}"
INSTALL="${4:-}"

if [ $# -eq 0 ]; then
    printf "${YELLOW}No arguments provided, using defaults: ${TARGET} ${MODE} ${BUILD_TYPE}${NC}\n"
fi

if [ "$MODE" = "sources" ] && [ "$BUILD_TYPE" = "CLEAN" ]; then
    for dir in "build/hexapod-${TARGET}-DEBUG" "build/hexapod-${TARGET}-RELEASE"; do
        if [ -d "$dir" ]; then
            printf "${YELLOW}Removing ${dir}${NC}\n"
            rm -rf "$dir"
        fi
    done
    printf "${GREEN}Clean completed${NC}\n"
    exit 0
fi

if [ "$MODE" = "test" ]; then
    BUILD_DIR="build/hexapod-${TARGET}-TEST"
    BUILD_TARGET="HexapodcppTest"
else
    BUILD_DIR="build/hexapod-${TARGET}-${BUILD_TYPE}"
    BUILD_TARGET="Hexapodcpp"
fi
printf "${GREEN}Build directory: ${BUILD_DIR}${NC}\n"

printf "${GREEN}Running prebuild: ${TARGET} ${MODE} ${BUILD_TYPE}${NC}\n"
bin/dev/prebuild.sh "${TARGET}" "${MODE}" "${BUILD_TYPE}"

if [ "$TARGET" = "RPI" ] && [ "$INSTALL" = "install" ]; then
    printf "${YELLOW}Installing WiringPi...${NC}\n"
    (cd "./${BUILD_DIR}/_deps/wiringpi-src/" && ./build)
    printf "${GREEN}WiringPi installed successfully${NC}\n"
fi

printf "${GREEN}Building ${BUILD_TARGET} with -j$(nproc)...${NC}\n"
if CLICOLOR_FORCE=1 cmake --build "${BUILD_DIR}" --target "${BUILD_TARGET}" --parallel "$(nproc)"; then
    printf "${GREEN}Build completed successfully!${NC}\n"
else
    BUILD_RESULT=$?
    printf "${RED}Build failed with exit code ${BUILD_RESULT}${NC}\n"
    exit ${BUILD_RESULT}
fi
