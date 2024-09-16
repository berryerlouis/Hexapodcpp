#! /bin/sh

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

bin/dev/prebuild.sh test ${1}

cd unittests/build

if [ $# -eq 0 ]; then
    make -j16 ${1}
    ./HexapodcppTest --gtest_color=yes -j16 --rerun-failed --output-on-failure --gtest_shuffle
else
    if [ $1 = "all" ] || [ $1 = "clean" ] || [ $1 = "help" ]; then
        make -j16 ${1}
        if [ $1 = "all" ]; then
            ./HexapodcppTest --gtest_color=yes -j16 --rerun-failed --output-on-failure --gtest_shuffle
        fi
    else 
        make -j16
        ./HexapodcppTest --gtest_color=yes -j16 --rerun-failed --output-on-failure --gtest_shuffle
    fi
fi

cd -