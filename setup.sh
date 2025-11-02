#!/bin/bash
# ============================================================================
# GameEngine Setup Script - Automated Dependency Installation
# ============================================================================
# This script automatically sets up your development environment including:
# - CMake (if not found)
# - C++ compiler and build tools
# - Git submodules
# - Required development libraries
#
# If you encounter issues, see TROUBLESHOOTING.md for detailed solutions.
# ============================================================================

set -e

echo "========================================"
echo "GameEngine - Automated Setup"
echo "========================================"
echo ""
echo "⚠️  Having issues? See TROUBLESHOOTING.md for help"
echo ""

# Detect OS
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    OS="linux"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    OS="macos"
else
    echo "ERROR: Unsupported operating system: $OSTYPE"
    exit 1
fi

echo "Operating System: $OS"
echo ""

# ============================================================================
# Step 1: Check for Git
# ============================================================================
echo "[1/4] Checking for Git..."
if ! command -v git &> /dev/null; then
    echo "ERROR: Git is not installed"
    echo ""
    
    if [ "$OS" = "linux" ]; then
        echo "Install Git with:"
        echo "  sudo apt-get install git    (Ubuntu 24.04 LTS / Ubuntu 22.04 / Debian)"
        echo "  sudo yum install git        (RHEL/CentOS)"
        echo "  sudo dnf install git        (Fedora)"
    elif [ "$OS" = "macos" ]; then
        echo "Install Git with:"
        echo "  brew install git"
        echo "Or install Xcode Command Line Tools:"
        echo "  xcode-select --install"
    fi
    
    exit 1
fi

echo "   Git found: $(git --version)"
echo ""

# ============================================================================
# Step 2: Check for CMake
# ============================================================================
echo "[2/4] Checking for CMake..."
if ! command -v cmake &> /dev/null; then
    echo "CMake not found"
    echo ""
    
    if [ "$OS" = "linux" ]; then
        echo "Install CMake with:"
        echo "  sudo apt-get install cmake    (Ubuntu 24.04 LTS / Ubuntu 22.04 / Debian)"
        echo "  sudo yum install cmake        (RHEL/CentOS)"
        echo "  sudo dnf install cmake        (Fedora)"
        
        read -p "Would you like to install CMake now? (y/n) " -n 1 -r
        echo ""
        
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            if command -v apt-get &> /dev/null; then
                sudo apt-get update
                sudo apt-get install -y cmake
            elif command -v yum &> /dev/null; then
                sudo yum install -y cmake
            elif command -v dnf &> /dev/null; then
                sudo dnf install -y cmake
            else
                echo "ERROR: Could not determine package manager"
                exit 1
            fi
        else
            echo "Please install CMake and run this script again"
            exit 1
        fi
    elif [ "$OS" = "macos" ]; then
        echo "Install CMake with:"
        echo "  brew install cmake"
        echo ""
        
        read -p "Would you like to install CMake now using Homebrew? (y/n) " -n 1 -r
        echo ""
        
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            if ! command -v brew &> /dev/null; then
                echo "Homebrew not found. Installing Homebrew first..."
                echo ""
                echo "NOTE: This will download and execute the official Homebrew installer from:"
                echo "  https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh"
                echo ""
                read -p "Continue? (y/n) " -n 1 -r
                echo ""
                if [[ ! $REPLY =~ ^[Yy]$ ]]; then
                    echo "Installation cancelled. Please install Homebrew manually:"
                    echo "  Visit: https://brew.sh"
                    exit 1
                fi
                /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
            fi
            brew install cmake
        else
            echo "Please install CMake and run this script again"
            exit 1
        fi
    fi
fi

echo "   CMake found: $(cmake --version | head -n1)"
echo ""

# ============================================================================
# Step 3: Check for C++ Compiler and Development Tools
# ============================================================================
echo "[3/4] Checking for C++ compiler and build tools..."
echo ""

# Use the comprehensive compiler detection script
if [ -f "tools/check-compiler.sh" ]; then
    bash tools/check-compiler.sh
    
    if [ $? -ne 0 ]; then
        echo ""
        echo "The compiler detection script found issues."
        echo ""
        
        read -p "Would you like to automatically fix these issues? (y/n) " -n 1 -r
        echo ""
        
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            echo ""
            echo "Running compiler fix with autofix..."
            bash tools/check-compiler.sh -autofix
            
            if [ $? -ne 0 ]; then
                echo ""
                echo "After fixing the issues, please run this setup script again."
                exit 1
            fi
        else
            read -p "Continue setup anyway? (y/n) " -n 1 -r
            echo ""
            if [[ ! $REPLY =~ ^[Yy]$ ]]; then
                exit 1
            fi
        fi
    fi
else
    # Fallback to basic checks if detection script is missing
    echo "   Compiler detection script not found, performing basic checks..."
    
    if [ "$OS" = "linux" ]; then
        if ! command -v g++ &> /dev/null; then
            echo "   ERROR: g++ not found"
            echo ""
            echo "   Install build tools with:"
            echo "     sudo apt-get install build-essential    (Ubuntu 24.04 LTS / Ubuntu 22.04 / Debian)"
            echo "     sudo yum groupinstall 'Development Tools'    (RHEL/CentOS)"
            
            read -p "Continue anyway? (y/n) " -n 1 -r
            echo ""
            if [[ ! $REPLY =~ ^[Yy]$ ]]; then
                exit 1
            fi
        else
            echo "   g++ found: $(g++ --version | head -n1)"
        fi
    elif [ "$OS" = "macos" ]; then
        if ! command -v clang++ &> /dev/null; then
            echo "   ERROR: clang++ not found"
            echo ""
            echo "   Install Xcode Command Line Tools:"
            echo "     xcode-select --install"
            
            read -p "Continue anyway? (y/n) " -n 1 -r
            echo ""
            if [[ ! $REPLY =~ ^[Yy]$ ]]; then
                exit 1
            fi
        else
            echo "   clang++ found: $(clang++ --version | head -n1)"
        fi
    fi
fi

echo ""

# ============================================================================
# Step 4: Initialize Git Submodules
# ============================================================================
echo "[4/4] Initializing git submodules..."
git submodule update --init --recursive

if [ $? -ne 0 ]; then
    echo "ERROR: Failed to update git submodules"
    exit 1
fi

echo "   Submodules initialized successfully"
echo ""

# ============================================================================
# Setup Complete
# ============================================================================
echo "========================================"
echo "Setup Complete!"
echo "========================================"
echo ""
echo "Your development environment is ready."
echo ""
echo "Next steps:"
echo "  1. Run: ./build.sh (to compile the engine)"
echo "  2. Run: ./run.sh (to start the game engine)"
echo ""
echo "⚠️  If you encounter build issues, see TROUBLESHOOTING.md for solutions."
echo ""
