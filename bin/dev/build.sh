#! /bin/sh

if [ $# -eq 0 ]; then
    bin/dev/prebuild.sh RPI source #DEBUG by default
else
   #1 : target AVR, X64, RPI
   #2 : source or test
   #3 : DEBUG or RELEASE or CLEAN
    bin/dev/prebuild.sh ${1} source ${2}
fi

cd build
CFLAGS=-fdiagnostics-color CXXFLAGS=-fdiagnostics-color CLICOLOR_FORCE=1 make -j16 -Wno-dev
cd -
