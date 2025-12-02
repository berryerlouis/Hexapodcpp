#!/bin/sh
set -e  # Exit on error

# Build script for Hexapodcpp
# Arguments:
#   1: TARGET (X64 or RPI)
#   2: MODE (sources or test)
#   3: BUILD_TYPE (DEBUG, RELEASE, or CLEAN)
#   4: INSTALL (optional, "install" to build WiringPi for RPI)

# Color output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Determine build directory
if [ $# -eq 0 ]; then
    BUILD_DIR="build"
    printf "${YELLOW}No arguments provided, using default build directory: ${BUILD_DIR}${NC}\n"
else
    if [ "$2" = "sources" ]; then
        BUILD_TYPE="${3:-DEBUG}"
        BUILD_DIR="build/hexapod-${1}-${BUILD_TYPE}"
    elif [ "$2" = "test" ]; then
        BUILD_DIR="build/hexapod-${1}-TEST"
    else
        BUILD_DIR="build"
    fi
    printf "${GREEN}Build directory: ${BUILD_DIR}${NC}\n"
fi

# Run prebuild configuration
if [ $# -eq 0 ]; then
    printf "${YELLOW}Running prebuild with defaults (RPI sources)${NC}\n"
    bin/dev/prebuild.sh RPI sources
else
    printf "${GREEN}Running prebuild: ${1} ${2} ${3}${NC}\n"
    bin/dev/prebuild.sh "${1}" "${2}" "${3}"
    
    # Install WiringPi if requested
    if [ "$1" = "RPI" ] && [ "$4" = "install" ]; then
        printf "${YELLOW}Installing WiringPi...${NC}\n"
        cd "./${BUILD_DIR}/_deps/wiringpi-src/" || exit 1
        ./build
        cd - > /dev/null || exit 1
        printf "${GREEN}WiringPi installed successfully${NC}\n"
    fi
fi

# Build the project
printf "${GREEN}Building project with -j16...${NC}\n"
cd "${BUILD_DIR}" || exit 1
CFLAGS=-fdiagnostics-color CXXFLAGS=-fdiagnostics-color CLICOLOR_FORCE=1 make -j16 -Wno-dev
BUILD_RESULT=$?
cd - > /dev/null || exit 1

if [ ${BUILD_RESULT} -eq 0 ]; then
    printf "${GREEN}Build completed successfully!${NC}\n"
else
    printf "${RED}Build failed with exit code ${BUILD_RESULT}${NC}\n"
    exit ${BUILD_RESULT}
fi