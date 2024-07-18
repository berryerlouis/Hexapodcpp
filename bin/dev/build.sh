#! /bin/sh

if [ $# -eq 0 ]; then
    bin/dev/prebuild.sh source
else
    bin/dev/prebuild.sh source -D${1} #could be DEBUG or RELEASE
fi

cd build
CFLAGS=-fdiagnostics-color CXXFLAGS=-fdiagnostics-color CLICOLOR_FORCE=1 make -j16 -Wno-dev
cd -
