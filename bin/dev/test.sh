#! /bin/sh

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

bin/dev/prebuild.sh test

if [ $# -eq 0 ]; then
    cd unittests/build
    CFLAGS=-fdiagnostics-color CXXFLAGS=-fdiagnostics-color CLICOLOR_FORCE=1 make -j16 -Wno-dev
    if command; then
        ctest -j16 --rerun-failed --output-on-failure
    else
        echo "${RED}Error${NC}: Make failed!"
    fi
else
    if [ $1 = "list" ]; then
        echo "${GREEN}Success${NC}: List of Unit test ${GREEN}${1}${NC}:"
        UTfiles=$(find unittests/build/ -name 'UT_*' -type f ! -name "*.*")
        for UTfile in $UTfiles; do
            if [ -f "$UTfile" ]; then
                echo ${GREEN}$(basename ${UTfile})${NC}
            fi
        done
    else
        echo "Seach for Unit Test: $1"
        file=$(find unittests/build/ -name "$1")
        if [ $file = ""]; then
            echo "${RED}Error${NC}: Unit test file ${RED}${1}${NC} not found!"
        else
            dir=$(dirname $file)
            echo "${GREEN}Success${NC}: Launch Unit test file ${GREEN}${1} ${NC}."
            cd $dir
            make -j16
            cd -
            eval "$file"
        fi
    fi
fi
