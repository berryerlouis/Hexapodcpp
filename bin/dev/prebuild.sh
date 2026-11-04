#!/usr/bin/env bash
set -euo pipefail

# Prebuild (CMake configure) script for Hexapodcpp
# Usage: bin/dev/prebuild.sh [TARGET] [MODE] [BUILD_TYPE]
#   TARGET:     X64 | RPI              (default: RPI)
#   MODE:       sources | test         (default: sources)
#   BUILD_TYPE: DEBUG | RELEASE        (default: DEBUG, sources mode only)
#
# Build directories:
#   sources -> build/hexapod-<TARGET>-<BUILD_TYPE>
#   test    -> build/hexapod-<TARGET>-TEST

TARGET="${1:-RPI}"
MODE="${2:-sources}"
BUILD_TYPE="${3:-DEBUG}"

if [ "$TARGET" != "X64" ] && [ "$TARGET" != "RPI" ]; then
    printf "Error: Invalid TARGET '%s'. Must be X64 or RPI\n" "$TARGET"
    exit 1
fi

case "$MODE" in
    sources)
        case "$BUILD_TYPE" in
            DEBUG) CMAKE_BUILD_TYPE="Debug" ;;
            RELEASE) CMAKE_BUILD_TYPE="Release" ;;
            *)
                printf "Error: Invalid BUILD_TYPE '%s'. Must be DEBUG or RELEASE\n" "$BUILD_TYPE"
                exit 1
                ;;
        esac
        BUILD_DIR="build/hexapod-${TARGET}-${BUILD_TYPE}"
        GTEST=OFF
        ;;
    test)
        CMAKE_BUILD_TYPE="Debug"
        BUILD_DIR="build/hexapod-${TARGET}-TEST"
        GTEST=ON
        ;;
    *)
        printf "Error: Invalid MODE '%s'. Must be sources or test\n" "$MODE"
        exit 1
        ;;
esac

printf "Configuring %s %s build for %s in %s...\n" "$MODE" "$CMAKE_BUILD_TYPE" "$TARGET" "$BUILD_DIR"
cmake \
    -DCMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}" \
    -DTARGET="${TARGET}" \
    -DGTEST="${GTEST}" \
    -DCMAKE_COLOR_DIAGNOSTICS=ON \
    -S . \
    -B "${BUILD_DIR}" \
    -Wno-dev

printf "Configuration completed successfully\n"
