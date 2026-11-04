#!/usr/bin/env bash
set -euo pipefail

# Unit test script for Hexapodcpp
# Usage: bin/dev/test.sh [all|clean|help|<TestSuiteName>]
#   (none):          build and run all tests (shuffled)
#   all:             build and run all tests, write build/hexapod-X64-TEST/test_results.xml
#   clean|help:      forward to the build system
#   <TestSuiteName>: build and run only that suite (gtest filter "<TestSuiteName>.*")
# Exits with a non-zero status if the build or any test fails.

BUILD_DIR="build/hexapod-X64-TEST"
TEST_BIN="${BUILD_DIR}/unittests/HexapodcppTest"
RESULTS_XML="${BUILD_DIR}/test_results.xml"
ARG="${1:-}"

# Clean partial googletest downloads left by an interrupted fetch
rm -rf "${BUILD_DIR}"/_deps/ex-googletest-populate*

bin/dev/prebuild.sh X64 test

case "$ARG" in
    clean|help)
        cmake --build "${BUILD_DIR}" --target "${ARG}"
        exit 0
        ;;
esac

cmake --build "${BUILD_DIR}" --target HexapodcppTest --parallel "$(nproc)"

case "$ARG" in
    "")
        "${TEST_BIN}" --gtest_color=yes --gtest_shuffle
        ;;
    all)
        "${TEST_BIN}" --gtest_color=yes --gtest_shuffle --gtest_output="xml:${RESULTS_XML}"
        ;;
    *)
        "${TEST_BIN}" --gtest_color=yes --gtest_filter="${ARG}.*" --gtest_output="xml:${RESULTS_XML}"
        ;;
esac
