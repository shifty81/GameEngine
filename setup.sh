#!/bin/bash
# ============================================================================
# GameEngine Setup Script for Ubuntu 24.04 LTS
# ============================================================================
# This script automatically sets up your development environment including:
# - CMake (if not found)
# - C++ compiler and build tools (GCC 13.3)
# - Git submodules
# - Required development libraries (OpenGL, X11)
#
# If you encounter issues, see UBUNTU_24_04.md for detailed solutions.
# ============================================================================

set -e

echo "========================================"
echo "GameEngine - Ubuntu 24.04 Setup"
echo "========================================"
echo ""

# Verify we're on Ubuntu
if [ ! -f /etc/os-release ]; then
    echo "ERROR: Cannot detect OS. This script is designed for Ubuntu 24.04 LTS."
    exit 1
fi

source /etc/os-release
if [[ "$ID" != "ubuntu" ]]; then
    echo "WARNING: This script is optimized for Ubuntu 24.04 LTS."
    echo "Detected: $NAME $VERSION"
    echo "Proceeding anyway..."
    echo ""
fi

# ============================================================================
# Step 1: Check for Git
# ============================================================================
echo "[1/4] Checking for Git..."
if ! command -v git &> /dev/null; then
    echo "ERROR: Git is not installed"
    echo ""
    echo "Install Git with:"
    echo "  sudo apt-get update"
    echo "  sudo apt-get install -y git"
    echo ""
    exit 1
fi
echo "✓ Git is installed: $(git --version)"
echo ""

# ============================================================================
# Step 2: Check for CMake
# ============================================================================
echo "[2/4] Checking for CMake..."
if ! command -v cmake &> /dev/null; then
    echo "WARNING: CMake is not installed"
    echo ""
    echo "Install CMake with:"
    echo "  sudo apt-get update"
    echo "  sudo apt-get install -y cmake"
    echo ""
    
    read -p "Would you like to install CMake now? (y/n) " -n 1 -r
    echo ""
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        sudo apt-get update
        sudo apt-get install -y cmake
        echo "✓ CMake installed successfully"
    else
        echo "ERROR: CMake is required to build the engine"
        exit 1
    fi
else
    CMAKE_VERSION=$(cmake --version | head -n1)
    echo "✓ CMake is installed: $CMAKE_VERSION"
fi
echo ""

# ============================================================================
# Step 3: Check for C++ Compiler and Libraries
# ============================================================================
echo "[3/4] Checking for C++ compiler and libraries..."
if [ -f "./tools/check-compiler.sh" ]; then
    ./tools/check-compiler.sh -quiet
    COMPILER_CHECK=$?
    
    if [ $COMPILER_CHECK -ne 0 ]; then
        echo ""
        echo "Compiler or required libraries are missing."
        echo ""
        read -p "Would you like to install them now? (y/n) " -n 1 -r
        echo ""
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            ./tools/check-compiler.sh -autofix
            echo ""
            echo "✓ Development tools installed successfully"
        else
            echo "WARNING: Build may fail without required dependencies"
            echo ""
            read -p "Continue anyway? (y/n) " -n 1 -r
            echo ""
            if [[ ! $REPLY =~ ^[Yy]$ ]]; then
                echo "Setup aborted."
                exit 1
            fi
        fi
    else
        echo "✓ C++ compiler and all required libraries are installed"
    fi
else
    echo "WARNING: Compiler check script not found"
    echo "Make sure you have GCC 13.3+ and development libraries installed:"
    echo "  sudo apt-get install -y build-essential cmake git \\"
    echo "      libgl1-mesa-dev libglu1-mesa-dev \\"
    echo "      libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev"
fi
echo ""

# ============================================================================
# Step 4: Initialize Git Submodules
# ============================================================================
echo "[4/4] Initializing git submodules..."
if git submodule status | grep -q '^-'; then
    echo "Downloading external dependencies (GLFW, GLM, Assimp)..."
    git submodule update --init --recursive
    echo "✓ Submodules initialized successfully"
else
    echo "✓ Submodules already initialized"
fi
echo ""

# ============================================================================
# Setup Complete
# ============================================================================
echo "========================================"
echo "✓ Setup Complete!"
echo "========================================"
echo ""
echo "Next steps:"
echo "  1. Build the engine:  ./build.sh"
echo "  2. Run the engine:    ./run.sh"
echo ""
echo "For more information, see:"
echo "  - README.md - Quick start guide"
echo "  - UBUNTU_24_04.md - Complete Ubuntu 24.04 development guide"
echo ""
