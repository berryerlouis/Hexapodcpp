#!/bin/sh

# Arguments:
# 1: target (AVR, X64, RPI)
# 2: source or test
# 3: DEBUG, RELEASE, or CLEAN

# Check if no arguments are provided
if [ $# -eq 0 ]; then
    # Run cmake with DEBUG build type, specifying source and build directories
    cmake -DCMAKE_BUILD_TYPE=DEBUG -S . -B build
else
    # Check if the second argument is "sources"
    if [ $2 = "sources" ]; then
        # If only one argument is provided
        if [ $# -eq 1 ]; then
            # Run cmake with DEBUG build type, specifying target and disabling GTest
            cmake -DCMAKE_BUILD_TYPE=DEBUG -DTARGET="${1}" -DGTEST=0 -S . -B build
        else
            # If the third argument is "CLEAN"
            if [ $3 = "CLEAN" ]; then
                # Clean the build directory
                cmake --build . --target clean
            else
                # Run cmake with the specified build type, target, and disabling GTest
                cmake -DCMAKE_BUILD_TYPE=${3} -DTARGET="${1}" -DGTEST=0 -S . -B build
            fi
        fi
    fi
    # Check if the second argument is "test"
    if [ $2 = "test" ]; then
        # Change directory to unittests
        cd unittests
        # If the third argument is "all", "clean", or "help"
        if [ $3 = "all" ] || [ $3 = "clean" ] || [ $3 = "help" ]; then
            # Run cmake with GTest samples enabled, DEBUG build type, and build all unit tests
            cmake -Dgtest_build_samples=ON -DCMAKE_BUILD_TYPE=Debug -DUT_TO_BUILD="all" -S . -B build
        else
            # Run cmake with GTest samples enabled, DEBUG build type, and build specified unit tests
            cmake -Dgtest_build_samples=ON -DCMAKE_BUILD_TYPE=Debug -DUT_TO_BUILD=${3} -S . -B build
        fi
    fi
fi