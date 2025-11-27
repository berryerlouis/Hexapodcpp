#!/bin/bash
# Cross-compile Hexapodcpp for Raspberry Pi Zero 2 W using Docker
# Usage: ./bin/dev/docker-build-rpi.sh [debug|release] [clean]
#   debug|release: Build type (default: release)
#   clean: Remove previous build artifacts

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
TOOLCHAIN_IMAGE="hexapodcpp:rpi-toolchain"
OUTPUT_BINARY="Hexapodcpp"

# Parse arguments
BUILD_TYPE="RELEASE"
CLEAN_BUILD=false

for arg in "$@"; do
    case $arg in
        debug|DEBUG)
            BUILD_TYPE="DEBUG"
            ;;
        release|RELEASE)
            BUILD_TYPE="RELEASE"
            ;;
        clean|CLEAN)
            CLEAN_BUILD=true
            ;;
    esac
done

BUILD_DIR="build/hexapod-RPI-$(echo $BUILD_TYPE)"

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
    docker rmi "$TOOLCHAIN_IMAGE" 2>/dev/null || true
    echo "Clean complete."
fi

# Build toolchain image (only base dependencies, cached)
echo ""
echo "Building toolchain Docker image (cached)..."
docker build -f Dockerfile.rpi-zero2 --target toolchain \
    --build-arg INSTALL_CORP_CERTS=true \
    -t "$TOOLCHAIN_IMAGE" .

# Run incremental build using volume mount
echo ""
echo "Running incremental build with volume mount..."
docker run --rm \
    -v "$PROJECT_ROOT:/workspace" \
    -w /workspace \
    -e CLICOLOR_FORCE=1 \
    -e TERM=xterm-256color \
    "$TOOLCHAIN_IMAGE" \
    bash -c "export CFLAGS=-fdiagnostics-color && export CXXFLAGS=-fdiagnostics-color && \
        if [ ! -f $BUILD_DIR/CMakeCache.txt ]; then \
            cmake -Wno-dev -S . -B $BUILD_DIR \
                -DCMAKE_TOOLCHAIN_FILE=/workspace/cmake/toolchain-rpi-zero-2-w.cmake \
                -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
                -DTARGET=RPI \
                -DOPENSSL_USE_STATIC_LIBS=OFF \
                -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
                -DCMAKE_COLOR_DIAGNOSTICS=ON; \
        fi && \
        CLICOLOR_FORCE=1 cmake --build $BUILD_DIR --target Hexapodcpp -- -j\$(nproc)"

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
echo ""
echo "Deploy to Raspberry Pi:"
echo "  Stopping running process on hexabot..."
ssh hexabot "sudo systemctl stop hexabot.service 2>/dev/null; sudo pkill -9 gdbserver 2>/dev/null; sudo killall -9 Hexapodcpp 2>/dev/null; sleep 0.5; true"
echo "  Copying binary..."
scp $OUTPUT_BINARY hexabot:/home/hexabot/
echo "  Deployment complete!"
echo ""
