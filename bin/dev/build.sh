#!/bin/sh

# Arguments:
# 1: target (X64, RPI)
# 2: source or test
# 3: DEBUG, RELEASE, or CLEAN
# 4: RPI install wiring PI (optional)

# Determine build directory based on arguments
if [ $# -eq 0 ]; then
    BUILD_DIR="build"
else
    if [ "$2" = "sources" ]; then
        BUILD_TYPE="${3:-DEBUG}"
        BUILD_DIR="build/hexapod-${1}-${BUILD_TYPE}"
    elif [ "$2" = "test" ]; then
        BUILD_DIR="build/hexapod-${1}-TEST"
    else
        BUILD_DIR="build"
    fi
fi

# Check if no arguments are provided
if [ $# -eq 0 ]; then
    # Run prebuild script with default parameters (RPI and source)
    bin/dev/prebuild.sh RPI sources #DEBUG by default
else

    # Run prebuild script with provided arguments
    bin/dev/prebuild.sh ${1} ${2} ${3}

    # If target is RPI and the fourth argument is "install"
    if [ "$1" = "RPI" -a "$4" = "install" ]; then
        # Change directory to wiringpi source
        cd ./${BUILD_DIR}/_deps/wiringpi-src/
        # Execute the build script
        ./build
        # Return to the previous directory
        cd -
    fi
fi

# Change to the build directory
cd ${BUILD_DIR}
# Set compiler flags for colored diagnostics and force color output
CFLAGS=-fdiagnostics-color CXXFLAGS=-fdiagnostics-color CLICOLOR_FORCE=1 make -j16 -Wno-dev
# Return to the previous directory
cd -