#! /bin/sh

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

bin/dev/prebuild.sh test

if [ $# -eq 0 ]; then
    cd unittests/build
    make -j16 ${1}
    ./HexapodcppTest --gtest_color=yes -j16 --rerun-failed --output-on-failure --gtest_shuffle
    cd ../..
else
    if [ $1 = "all" ] || [ $1 = "clean" ] || [ $1 = "help" ]; then
        cd unittests/build
        make -j16 ${1}
        if [ $1 = "all" ]; then
            ./HexapodcppTest --gtest_color=yes -j16 --rerun-failed --output-on-failure --gtest_shuffle
        fi
        cd ../..
    else 
        cd unittests/build
        make -j16
        ./HexapodcppTest --gtest_filter="${1}*" --gtest_color=yes -j16 --rerun-failed --output-on-failure --gtest_shuffle
        cd ../..
    fi
fi
