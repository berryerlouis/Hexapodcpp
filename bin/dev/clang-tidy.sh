#!/usr/bin/env bash
#
# run-clang-tidy.sh
# Runs clang-tidy on all files in compile_commands.json
# Usage: ./run-clang-tidy.sh
# Example: ./run-clang-tidy.sh

set -euo pipefail

# Default values
BUILD_DIR="${1:-build/hexapod-X64-DEBUG}"


# Default build directory and checks
CHECKS="${2:--checks=*,
-clang-analyzer-core.NonNullParamChecker,
-clang-analyzer-core.uninitialized.Assign,
-clang-analyzer-optin.cplusplus.UninitializedObject,
-clang-analyzer-optin.cplusplus.VirtualCall,
}"

# Build the project to generate compile_commands.json
cmake -S . -B $BUILD_DIR \
-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
-DTARGET=X64 \
-DCMAKE_BUILD_TYPE=Debug
cmake --build $BUILD_DIR -- -j$(nproc)

# Ensure compile_commands.json exists
if [[ ! -f "$BUILD_DIR/compile_commands.json" ]]; then
    echo "❌ Error: compile_commands.json not found in $BUILD_DIR."
    echo "Run CMake with: cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..."
    exit 1
fi

# Ensure clang-tidy is installed
if ! command -v clang-tidy &>/dev/null; then
    echo "❌ Error: clang-tidy not found. Install it first."
    exit 1
fi


echo "🔍 Running clang-tidy..."
# Create output directory for reports
REPORT_DIR="clang-tidy-report"
mkdir -p "$REPORT_DIR"
REPORT_FILE="$REPORT_DIR/report.txt"

# Run clang-tidy in parallel using run-clang-tidy.py (comes with LLVM)
run-clang-tidy-20 \
    -p "$BUILD_DIR" \
    -checks="$CHECKS" \
    -header-filter='Hexapodcpp/src/*' src \
    -j "$(nproc)" \
    -fix \
    > "$REPORT_FILE" 2>&1

echo "✅ Clang-Tidy analysis complete."
echo "📄 Report saved to: $REPORT_FILE"
