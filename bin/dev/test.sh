#!/bin/sh

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

# Clean partial googletest downloads on permission errors
if [ -d "build/hexapod-X64-TEST/_deps/ex-googletest-populate"* ]; then
    echo "Cleaning partial GoogleTest extraction..."
    rm -rf build/hexapod-X64-TEST/_deps/ex-googletest-populate*
fi

bin/dev/prebuild.sh X64 test ${1}

cd build/hexapod-X64-TEST/unittests

if [ $# -eq 0 ]; then
    make -j16
    ./HexapodcppTest --gtest_color=yes --rerun-failed --output-on-failure --gtest_shuffle
else
    if [ $1 = "all" ]; then
        make -j16
        ./HexapodcppTest --gtest_color=yes --rerun-failed --output-on-failure --gtest_shuffle --gtest_output="xml:../test_results.xml"
    elif [ $1 = "clean" ] || [ $1 = "help" ]; then
        make -j16 ${1}
    else
        # Run specific test using gtest filter
        make -j16
        ./HexapodcppTest --gtest_color=yes --gtest_filter="${1}.*" --output-on-failure --gtest_output="xml:../test_results.xml"
    fi
fi

cd -

cd -