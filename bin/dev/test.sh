#! /bin/sh

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

bin/dev/prebuild.sh test

buildAll() {
    cd unittests/build
    CFLAGS=-fdiagnostics-color CXXFLAGS=-fdiagnostics-color CLICOLOR_FORCE=1 make -j16
    if command; then
        ./HexapodcppTest -j16 --rerun-failed --output-on-failure --gtest_shuffle
    else
        echo "${RED}Error${NC}: Make failed!"
    fi
    cd ../..
}

listAll() {
    echo "$List of current Unit test:"
    unittests/build/HexapodcppTest --gtest_list_tests
}

if [ $# -eq 0 ]; then
    buildAll
else
    if [ $1 = "list" ]; then
        listAll
    else
        echo "Seach for Unit Test ${GREEN}${1}${NC}:"
        cd unittests/build
        make -j16
        ./HexapodcppTest --gtest_filter="${1}*" --gtest_color=yes -j16 --rerun-failed --output-on-failure --gtest_shuffle

        cd ../..
    fi
fi
