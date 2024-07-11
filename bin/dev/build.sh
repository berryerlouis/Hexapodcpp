#! /bin/sh

bin/dev/prebuild.sh source

cd build
CFLAGS=-fdiagnostics-color CXXFLAGS=-fdiagnostics-color CLICOLOR_FORCE=1 make -j16 -Wno-dev

cd -
