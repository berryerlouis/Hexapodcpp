#!/bin/sh
set -e  # Exit on error

cmake -S . -B ./build/hexapod-X64-coverage \
  -DGTEST=1 \
  -DTARGET=X64 \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="--coverage" \
  -DCMAKE_C_FLAGS="--coverage"

cmake --build ./build/hexapod-X64-coverage/unittests -- -j$(nproc)

ctest --test-dir ./build/hexapod-X64-coverage/unittests

lcov --capture --directory ./build/hexapod-X64-coverage/unittests --output-file  coverage.info \
          --ignore-errors mismatch \
          --ignore-errors gcov \
          --ignore-errors gcov

lcov --remove coverage.info "./build/hexapod-X64-coverage/*/build/*" -o coverage.info

genhtml coverage.info --output-directory coverage-report