#!/bin/bash
set -euo pipefail

BUILD_DIR="../build"

echo "Cleaning build directory..."
rm -rf "$BUILD_DIR"

echo "Creating build directory..."
mkdir -p "$BUILD_DIR"

echo "Configuring project..."
cmake -S ../cmake -B "$BUILD_DIR"

echo "Building..."
cmake --build "$BUILD_DIR" -j$(nproc)

echo "Build successful"