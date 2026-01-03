#!/usr/bin/env bash
set -euo pipefail

# Format ./src
if [ -d "./src" ]; then
  find ./src -type f \( -iname '*.h' -o -iname '*.cpp' \) -print0 \
    | xargs -0 clang-format -style=file -i
fi

# Format ./unittests
if [ -d "./unittests" ]; then
  find ./unittests -type f \( -iname '*.h' -o -iname '*.cpp' \) -print0 \
    | xargs -0 clang-format -style=file -i
fi