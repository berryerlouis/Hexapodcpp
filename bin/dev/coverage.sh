#!/bin/sh
set -e  # Exit on error

cmake -S . -B ./build/hexapod-X64-COVERAGE \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DGTEST=1 \
  -DTARGET=X64 \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="--coverage" \
  -DCMAKE_C_FLAGS="--coverage"

cmake --build ./build/hexapod-X64-COVERAGE/unittests -- -j$(nproc)

ctest --test-dir ./build/hexapod-X64-COVERAGE/unittests

lcov  --ignore-errors gcov,mismatch,inconsistent,unused --capture --directory ./build/hexapod-X64-COVERAGE/unittests --output-file  coverage.info       

lcov --remove coverage.info '/usr/' 'build/hexapod-X64-COVERAGE/*' 'unittests/*' -o coverage.info

genhtml coverage.info --output-directory coverage-report --ignore-errors mismatch