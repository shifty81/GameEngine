#!/bin/bash

echo "================================"
echo "3D Game Engine - Build Script"
echo "================================"
echo ""

# ============================================================================
# Step 0: Check for C++ Compiler
# ============================================================================
echo "Checking for C++ compiler..."
echo ""

if [ -f "tools/check-compiler.sh" ]; then
    bash tools/check-compiler.sh -quiet
    
    if [ $? -ne 0 ]; then
        echo ""
        echo "========================================================================"
        echo "WARNING: C++ compiler issues detected!"
        echo "========================================================================"
        echo ""
        echo "The compiler detection found problems with your development setup."
        echo ""
        
        read -p "Run detailed diagnostics and get fix instructions? (y/n) " -n 1 -r
        echo ""
        
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            echo ""
            bash tools/check-compiler.sh
            echo ""
            echo "========================================================================"
            echo ""
            
            read -p "Would you like to automatically fix the issues? (y/n) " -n 1 -r
            echo ""
            
            if [[ $REPLY =~ ^[Yy]$ ]]; then
                bash tools/check-compiler.sh -autofix
                echo ""
                echo "Please fix the issues and run build.sh again."
                exit 1
            fi
        fi
        
        echo ""
        read -p "Continue with build anyway (may fail)? (y/n) " -n 1 -r
        echo ""
        if [[ ! $REPLY =~ ^[Yy]$ ]]; then
            exit 1
        fi
        echo ""
    fi
else
    echo "Compiler detection script not found, skipping compiler check..."
    echo "(Run setup.sh first to enable compiler detection)"
    echo ""
fi

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
cmake -DCMAKE_BUILD_TYPE=Release ..
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
