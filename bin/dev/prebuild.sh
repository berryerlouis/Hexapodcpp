#!/bin/sh
set -e  # Exit on error

# Prebuild configuration script for Hexapodcpp
# Arguments:
#   1: TARGET (X64 or RPI)
#   2: MODE (sources or test)
#   3: BUILD_TYPE (DEBUG, RELEASE, or CLEAN)

# Validate arguments
if [ $# -eq 0 ]; then
    printf "No arguments provided, using defaults: DEBUG build\n"
    cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build
    exit 0
fi

# Validate TARGET
if [ "$1" != "X64" ] && [ "$1" != "RPI" ]; then
    printf "Error: Invalid TARGET '$1'. Must be X64 or RPI\n"
    exit 1
fi

# Handle sources mode
if [ "$2" = "sources" ]; then
    BUILD_TYPE="${3:-DEBUG}"
    
    # Validate BUILD_TYPE
    if [ "$BUILD_TYPE" != "DEBUG" ] && [ "$BUILD_TYPE" != "RELEASE" ] && [ "$BUILD_TYPE" != "CLEAN" ]; then
        printf "Error: Invalid BUILD_TYPE '$BUILD_TYPE'. Must be DEBUG, RELEASE, or CLEAN\n"
        exit 1
    fi
    
    if [ "$BUILD_TYPE" = "CLEAN" ]; then
        printf "Cleaning build directory...\n"
        cmake --build . --target clean
    else
        printf "Configuring ${BUILD_TYPE} build for ${1}...\n"
        cmake \
            -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
            -DTARGET="${1}" \
            -DGTEST=0 \
            -S . \
            -B "build/hexapod-${1}-${BUILD_TYPE}" \
            -Wno-dev
    fi
fi

# Handle test mode
if [ "$2" = "test" ]; then
    printf "Configuring test build for ${1}...\n"
    cmake \
        -DCMAKE_BUILD_TYPE=Debug \
        -DGTEST=1 \
        -DTARGET="${1}" \
        -S . \
        -B "build/hexapod-${1}-TEST" \
        -Wno-dev
fi

printf "Configuration completed successfully\n"