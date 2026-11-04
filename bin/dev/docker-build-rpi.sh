#!/bin/bash
# Cross-compile Hexapodcpp for Raspberry Pi Zero 2 W using Docker
# Usage: ./bin/dev/docker-build-rpi.sh [debug|release] [clean]
#   debug|release: Build type (default: release)
#   clean: Remove previous build artifacts
# Environment:
#   TOOLCHAIN_IMAGE           Docker image tag to use (default: hexapodcpp:rpi-toolchain)
#   HEXAPOD_SKIP_IMAGE_BUILD  Set to 1 to reuse an existing TOOLCHAIN_IMAGE (e.g. built by CI)

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
TOOLCHAIN_IMAGE="${TOOLCHAIN_IMAGE:-hexapodcpp:rpi-toolchain}"
SKIP_IMAGE_BUILD="${HEXAPOD_SKIP_IMAGE_BUILD:-0}"
OUTPUT_BINARY="Hexapodcpp"

# Parse arguments
BUILD_TYPE="RELEASE"
CLEAN_BUILD=false

for arg in "$@"; do
    case $arg in
        debug|DEBUG|Debug)
            BUILD_TYPE="DEBUG"
            ;;
        release|RELEASE|Release)
            BUILD_TYPE="RELEASE"
            ;;
        clean|CLEAN)
            CLEAN_BUILD=true
            ;;
        *)
            echo "Unknown argument: $arg" >&2
            exit 1
            ;;
    esac
done

if [ "$BUILD_TYPE" = "DEBUG" ]; then
    CMAKE_BUILD_TYPE="Debug"
else
    CMAKE_BUILD_TYPE="Release"
fi

BUILD_DIR="build/hexapod-RPI-$BUILD_TYPE"
cd "$PROJECT_ROOT"

echo "============================================="
echo "Hexapodcpp - Raspberry Pi Cross-Compilation"
echo "Build Type: $BUILD_TYPE"
echo "============================================="

# Clean previous artifacts if requested
if [ "$CLEAN_BUILD" = true ]; then
    echo "Cleaning previous build artifacts..."
    rm -f "$OUTPUT_BINARY"
    rm -rf "$BUILD_DIR"
    if [ "$SKIP_IMAGE_BUILD" != "1" ]; then
        docker rmi "$TOOLCHAIN_IMAGE" 2>/dev/null || true
    fi
    echo "Clean complete."
fi

# Build toolchain image (only base dependencies, cached)
if [ "$SKIP_IMAGE_BUILD" = "1" ]; then
    echo ""
    echo "Using existing toolchain image: $TOOLCHAIN_IMAGE"
else
    echo ""
    echo "Building toolchain Docker image (cached)..."
    docker build -f Dockerfile.rpi-zero2 --target toolchain \
        -t "$TOOLCHAIN_IMAGE" .
fi

# Run incremental build using volume mount
echo ""
echo "Running incremental build with volume mount..."
docker run --rm \
    -v "$PROJECT_ROOT:/workspace" \
    -w /workspace \
    -e CLICOLOR_FORCE=1 \
    -e TERM=xterm-256color \
    "$TOOLCHAIN_IMAGE" \
    bash -c "set -e && \
        if [ ! -f $BUILD_DIR/CMakeCache.txt ]; then \
        cmake -Wno-dev -S . -B $BUILD_DIR \
            -DCMAKE_TOOLCHAIN_FILE=/workspace/cmake/toolchain-rpi-zero-2-w.cmake \
            -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE \
            -DTARGET=RPI \
            -DOPENSSL_USE_STATIC_LIBS=OFF \
            -DCMAKE_COLOR_DIAGNOSTICS=ON; \
        fi && \
        cmake --build $BUILD_DIR --target Hexapodcpp --parallel \$(nproc)"

# Copy binary to project root
echo ""
echo "Copying binary to project root..."
cp "$BUILD_DIR/src/Hexapodcpp" "./$OUTPUT_BINARY"

# Verify binary
echo ""
echo "Binary information:"
file "$OUTPUT_BINARY"
ls -lh "$OUTPUT_BINARY"

echo ""
echo "============================================="
echo "Build complete! Binary: ./$OUTPUT_BINARY"
echo "============================================="