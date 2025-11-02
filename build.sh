#!/bin/bash

echo "================================"
echo "3D Game Engine - Build Script"
echo "================================"
echo ""

# Check if CMake is installed
if ! command -v cmake &> /dev/null; then
    echo "ERROR: CMake is not installed"
    echo "Please install CMake using your package manager"
    exit 1
fi

# Update submodules
echo "Updating git submodules..."
git submodule update --init --recursive
if [ $? -ne 0 ]; then
    echo "ERROR: Failed to update git submodules"
    exit 1
fi

# Create build directory
mkdir -p build
cd build

# Generate makefiles
echo ""
echo "Generating makefiles..."
cmake ..
if [ $? -ne 0 ]; then
    echo "ERROR: CMake configuration failed"
    cd ..
    exit 1
fi

# Build the project
echo ""
echo "Building project..."
make -j$(nproc)
if [ $? -ne 0 ]; then
    echo "ERROR: Build failed"
    cd ..
    exit 1
fi

cd ..

echo ""
echo "================================"
echo "Build completed successfully!"
echo "================================"
echo ""
echo "Executable location: build/bin/GameEngine"
echo ""
echo "Run the game engine using: ./run.sh"
echo ""
