#!/usr/bin/env bash
#
# run-clang-tidy.sh
# Runs clang-tidy on all files in compile_commands.json
# Usage:
#   ./run-clang-tidy.sh [BUILD_DIR] [CHECKS]

set -euo pipefail

# Default build directory and checks
BUILD_DIR="${1:-build/hexapod-X64-DEBUG}"

# Default checks: start from clang-tidy defaults and add a few extra analyzers
CHECKS="${2:-clang-analyzer-*,cppcoreguidelines-*,modernize-*,performance-*,readability-*,\
-modernize-use-trailing-return-type,\
-modernize-use-auto,\
-modernize-concat-nested-namespaces,\
-modernize-deprecated-headers,\
-modernize-return-braced-init-list,\
-modernize-use-nullptr,\
-cppcoreguidelines-pro-type-vararg,\
-cppcoreguidelines-pro-bounds-pointer-arithmetic,\
-cppcoreguidelines-avoid-magic-numbers,\
-cppcoreguidelines-macro-usage,\
-cppcoreguidelines-avoid-magic-numbers,\
-cppcoreguidelines-pro-bounds-array-to-pointer-decay,\
-cppcoreguidelines-avoid-do-while,\
-cppcoreguidelines-avoid-non-const-global-variables,\
-cppcoreguidelines-pro-type-reinterpret-cast,\
-cppcoreguidelines-pro-type-member-init,\
-cppcoreguidelines-avoid-c-arrays,\
-cppcoreguidelines-pro-bounds-constant-array-index,\
-cppcoreguidelines-narrowing-conversions,\
-cppcoreguidelines-init-variables,\
-modernize-avoid-c-arrays,\
-performance-unnecessary-value-param,\
-readability-implicit-bool-conversion,\
-readability-magic-numbers,\
-cppcoreguidelines-pro-type-const-cast,\
-modernize-redundant-void-arg,\
-readability-else-after-return,\
-readability-redundant-member-init,\
-cppcoreguidelines-noexcept-move-operations,\
-performance-noexcept-move-constructor,\
-readability-suspicious-call-argument,\
-readability-function-cognitive-complexity,\
-readability-uppercase-literal-suffix,\
-readability-identifier-length,\
-readability-isolate-declaration,\
-performance-for-range-copy,\
-readability-simplify-boolean-expr,\
-clang-analyzer-core.NonNullParamChecker,\
-clang-analyzer-optin.cplusplus.VirtualCall,\
-clang-analyzer-core.uninitialized.Assign,\
-readability-inconsistent-declaration-parameter-name,\
-readability-avoid-return-with-void-value,\
-readability-isolate-declaration,\
-modernize-pass-by-value,\
-cppcoreguidelines-c-copy-assignment-signature,\
-modernize-use-emplace,\
-performance-move-const-arg,\
-readability-convert-member-functions-to-static,\
-modernize-use-equals-default,\
-performance-move-constructor-init,\
-readability-redundant-casting,\
-readability-redundant-string-init,\
-readability-container-size-empty,\
-readability-braces-around-statements,\
-cppcoreguidelines-owning-memory,\
-modernize-redundant-void-arg}"

# Configure the project to generate compile_commands.json (no build required)
cmake -S . -B "$BUILD_DIR" \
  -DTARGET=X64 \
  -DCMAKE_BUILD_TYPE=Debug \
  -Wno-dev

# Ensure compile_commands.json exists
if [[ ! -f "$BUILD_DIR/compile_commands.json" ]]; then
  echo "Error: compile_commands.json not found in $BUILD_DIR."
  exit 1
fi

# Tool names can be overridden, e.g. CLANG_TIDY=clang-tidy-20 RUN_CLANG_TIDY=run-clang-tidy-20
CLANG_TIDY="${CLANG_TIDY:-clang-tidy}"
if ! command -v "$CLANG_TIDY" >/dev/null 2>&1; then
  echo "Error: $CLANG_TIDY not found in PATH. Install it first."
  exit 1
fi

# Prefer run-clang-tidy if available, otherwise fall back to clang-tidy -p
RUN_CLANG_TIDY="${RUN_CLANG_TIDY:-}"
if [[ -z "$RUN_CLANG_TIDY" ]]; then
  if command -v run-clang-tidy >/dev/null 2>&1; then
    RUN_CLANG_TIDY="run-clang-tidy"
  elif command -v run-clang-tidy.py >/dev/null 2>&1; then
    RUN_CLANG_TIDY="run-clang-tidy.py"
  fi
fi

echo "Running clang-tidy..."
REPORT_DIR="clang-tidy-report"
mkdir -p "$REPORT_DIR"
REPORT_FILE="$REPORT_DIR/report.txt"

# Regexes on absolute paths: only analyze project sources/headers, not fetched dependencies
PROJECT_SRC="$(pwd)/src/"
HEADER_FILTER="^${PROJECT_SRC}.*"

if [[ -n "$RUN_CLANG_TIDY" ]]; then
  "$RUN_CLANG_TIDY" \
    -clang-tidy-binary "$(command -v "$CLANG_TIDY")" \
    -p "$BUILD_DIR" \
    -checks="$CHECKS" \
    -header-filter="$HEADER_FILTER" \
    -j "$(nproc)" \
    "^${PROJECT_SRC}" \
    >"$REPORT_FILE" 2>&1
else
  # Fallback: plain clang-tidy on project sources compiled for X64
  find src -name '*.cpp' ! -name '*RPI.cpp' -print0 \
    | xargs -0 "$CLANG_TIDY" \
      -p "$BUILD_DIR" \
      -checks="$CHECKS" \
      -header-filter="$HEADER_FILTER" \
    >"$REPORT_FILE" 2>&1
fi

echo "Clang-Tidy analysis complete."
echo "Report saved to: $REPORT_FILE"