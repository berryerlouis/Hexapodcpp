#! /bin/sh

if [ $# -eq 0 ]; then
    bin/dev/prebuild.sh source #DEBUG by default
else
    bin/dev/prebuild.sh source -D${1} #could be DEBUG or RELEASE or CLEAN
fi

cd build
CFLAGS=-fdiagnostics-color CXXFLAGS=-fdiagnostics-color CLICOLOR_FORCE=1 make -j16 -Wno-dev
cd -
