#!/bin/sh
set -e  # Exit on error

cppcheck \
    --enable=all \
    --inconclusive \
    --force \
    --inline-suppr \
    --language=c++ \
    --xml \
    --xml-version=2 \
    src 2> cppcheck-report.xml