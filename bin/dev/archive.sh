#!/bin/sh

set -e

# Create archive directory
mkdir -p archive

# Archive X64 test results (from test run)
if [ -f "build/hexapod-X64-TEST/test_results.xml" ]; then
    cp build/hexapod-X64-TEST/test_results.xml archive/
fi

# Archive RPI binary if it exists (from release build)
if [ -f "build/hexapod-RPI-RELEASE/src/Hexapodcpp" ]; then
    cp build/hexapod-RPI-RELEASE/src/Hexapodcpp archive/
fi

# List archived files
printf "${GREEN}Archived files:${NC}\n"
ls -lh archive/

