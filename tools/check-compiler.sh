#!/bin/bash
# ============================================================================
# Cross-Platform C++ Compiler Detection and Fix Script
# ============================================================================
# This script detects C++ compiler installation and provides automated fixes
# when the compiler is not found.
# Supports: Linux (GCC/Clang), macOS (Xcode Command Line Tools)
# ============================================================================

set -e

# ANSI color codes for better output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
RESET='\033[0m'

# Command line flags
AUTO_FIX=false
QUIET=false

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -autofix|--autofix)
            AUTO_FIX=true
            shift
            ;;
        -quiet|--quiet)
            QUIET=true
            shift
            ;;
        *)
            echo "Unknown option: $1"
            echo "Usage: $0 [-autofix] [-quiet]"
            exit 1
            ;;
    esac
done

# Helper functions
print_header() {
    if [ "$QUIET" = false ]; then
        echo ""
        echo -e "${CYAN}============================================================================${RESET}"
        echo -e "${CYAN} $1${RESET}"
        echo -e "${CYAN}============================================================================${RESET}"
    fi
}

print_success() {
    if [ "$QUIET" = false ]; then
        echo -e "${GREEN}✓ $1${RESET}"
    fi
}

print_error() {
    echo -e "${RED}✗ $1${RESET}" >&2
}

print_warning() {
    if [ "$QUIET" = false ]; then
        echo -e "${YELLOW}⚠ $1${RESET}"
    fi
}

print_info() {
    if [ "$QUIET" = false ]; then
        echo -e "${BLUE}ℹ $1${RESET}"
    fi
}

# Detect operating system
detect_os() {
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        echo "linux"
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        echo "macos"
    else
        echo "unknown"
    fi
}

# ============================================================================
# Main Detection Logic
# ============================================================================

print_header "C++ Compiler Detection"

OS=$(detect_os)
print_info "Operating System: $OS"

if [ "$OS" = "unknown" ]; then
    print_error "Unsupported operating system: $OSTYPE"
    exit 1
fi

# ============================================================================
# Check for CMake
# ============================================================================
print_header "Step 1: Checking for CMake"

if command -v cmake &> /dev/null; then
    CMAKE_VERSION=$(cmake --version | head -n1)
    print_success "CMake found: $CMAKE_VERSION"
else
    print_error "CMake not found"
    
    if [ "$OS" = "linux" ]; then
        print_info "Install CMake with: sudo apt-get install cmake"
        print_info "Or: sudo yum install cmake"
    elif [ "$OS" = "macos" ]; then
        print_info "Install CMake with: brew install cmake"
    fi
    
    exit 1
fi

# ============================================================================
# Check for C++ Compiler (Linux)
# ============================================================================
if [ "$OS" = "linux" ]; then
    print_header "Step 2: Checking for C++ Compiler (Linux)"
    
    COMPILER_FOUND=false
    MISSING_PACKAGES=()
    
    # Check for g++
    if command -v g++ &> /dev/null; then
        GCC_VERSION=$(g++ --version | head -n1)
        print_success "g++ found: $GCC_VERSION"
        COMPILER_FOUND=true
    else
        print_error "g++ not found"
        MISSING_PACKAGES+=("g++")
    fi
    
    # Check for make
    if command -v make &> /dev/null; then
        MAKE_VERSION=$(make --version | head -n1)
        print_success "make found: $MAKE_VERSION"
    else
        print_error "make not found"
        MISSING_PACKAGES+=("make")
    fi
    
    # Check for OpenGL development libraries
    print_header "Step 3: Checking for Required Development Libraries"
    
    # Try ldconfig first, fall back to pkg-config if not available
    if command -v ldconfig &> /dev/null; then
        if ldconfig -p 2>/dev/null | grep -q libGL.so; then
            print_success "OpenGL development libraries found"
        else
            print_warning "OpenGL development libraries not found"
            MISSING_PACKAGES+=("libgl1-mesa-dev")
        fi
        
        if ldconfig -p 2>/dev/null | grep -q libGLU.so; then
            print_success "GLU development libraries found"
        else
            print_warning "GLU development libraries not found"
            MISSING_PACKAGES+=("libglu1-mesa-dev")
        fi
        
        if ldconfig -p 2>/dev/null | grep -q libX11.so; then
            print_success "X11 development libraries found"
        else
            print_warning "X11 development libraries not found"
            MISSING_PACKAGES+=("libx11-dev" "libxrandr-dev" "libxinerama-dev" "libxcursor-dev" "libxi-dev")
        fi
    elif command -v pkg-config &> /dev/null; then
        # Fallback to pkg-config for systems without ldconfig
        if pkg-config --exists gl 2>/dev/null; then
            print_success "OpenGL development libraries found (via pkg-config)"
        else
            print_warning "OpenGL development libraries not found"
            MISSING_PACKAGES+=("libgl1-mesa-dev")
        fi
        
        if pkg-config --exists glu 2>/dev/null; then
            print_success "GLU development libraries found (via pkg-config)"
        else
            print_warning "GLU development libraries not found"
            MISSING_PACKAGES+=("libglu1-mesa-dev")
        fi
        
        if pkg-config --exists x11 2>/dev/null; then
            print_success "X11 development libraries found (via pkg-config)"
        else
            print_warning "X11 development libraries not found"
            MISSING_PACKAGES+=("libx11-dev" "libxrandr-dev" "libxinerama-dev" "libxcursor-dev" "libxi-dev")
        fi
    else
        print_warning "Cannot verify libraries (ldconfig and pkg-config not available)"
        print_info "Libraries will be checked during CMake configuration"
    fi
    
    # Summary
    if [ "$COMPILER_FOUND" = true ] && [ ${#MISSING_PACKAGES[@]} -eq 0 ]; then
        print_header "Detection Summary"
        echo ""
        echo -e "${GREEN}════════════════════════════════════════════════════════════════════════${RESET}"
        echo -e "${GREEN}✓ SUCCESS: C++ compiler and all required libraries are installed!${RESET}"
        echo -e "${GREEN}════════════════════════════════════════════════════════════════════════${RESET}"
        echo ""
        print_info "You can now build the project using build.sh or CMake"
        exit 0
    else
        print_header "Detection Summary"
        echo ""
        echo -e "${RED}════════════════════════════════════════════════════════════════════════${RESET}"
        echo -e "${RED}✗ MISSING COMPONENTS DETECTED${RESET}"
        echo -e "${RED}════════════════════════════════════════════════════════════════════════${RESET}"
        echo ""
        
        if [ ${#MISSING_PACKAGES[@]} -gt 0 ]; then
            echo -e "${YELLOW}The following packages are required but not installed:${RESET}"
            echo ""
            for pkg in "${MISSING_PACKAGES[@]}"; do
                echo "  • $pkg"
            done
            echo ""
        fi
        
        print_header "How to Fix"
        echo ""
        
        # Detect package manager
        if command -v apt-get &> /dev/null; then
            PKG_MANAGER="apt-get"
            INSTALL_CMD="sudo apt-get update && sudo apt-get install -y"
        elif command -v yum &> /dev/null; then
            PKG_MANAGER="yum"
            INSTALL_CMD="sudo yum install -y"
        elif command -v dnf &> /dev/null; then
            PKG_MANAGER="dnf"
            INSTALL_CMD="sudo dnf install -y"
        elif command -v pacman &> /dev/null; then
            PKG_MANAGER="pacman"
            INSTALL_CMD="sudo pacman -S"
        else
            PKG_MANAGER="unknown"
        fi
        
        if [ "$PKG_MANAGER" = "apt-get" ]; then
            echo -e "${CYAN}Option 1: Install All Required Packages (Recommended)${RESET}"
            echo "  Run this command:"
            echo ""
            echo "  sudo apt-get update"
            echo "  sudo apt-get install -y build-essential cmake libgl1-mesa-dev libglu1-mesa-dev libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev"
            echo ""
            
            if [ "$AUTO_FIX" = true ]; then
                echo ""
                echo -e "${CYAN}AutoFix Mode: Installing packages...${RESET}"
                echo ""
                echo -e "${YELLOW}The following command will be executed:${RESET}"
                echo "  sudo apt-get update"
                echo "  sudo apt-get install -y build-essential cmake libgl1-mesa-dev libglu1-mesa-dev libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev"
                echo ""
                read -p "Continue with installation? (y/n) " -n 1 -r
                echo ""
                if [[ ! $REPLY =~ ^[Yy]$ ]]; then
                    print_info "Installation cancelled"
                    exit 1
                fi
                
                sudo apt-get update
                sudo apt-get install -y build-essential cmake libgl1-mesa-dev libglu1-mesa-dev libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
                
                if [ $? -eq 0 ]; then
                    echo ""
                    print_success "Packages installed successfully!"
                    print_info "You can now build the project"
                    exit 0
                else
                    print_error "Failed to install packages"
                    exit 1
                fi
            fi
        elif [ "$PKG_MANAGER" = "yum" ] || [ "$PKG_MANAGER" = "dnf" ]; then
            echo -e "${CYAN}Option 1: Install All Required Packages (Recommended)${RESET}"
            echo "  Run this command:"
            echo ""
            echo "  $INSTALL_CMD gcc-c++ cmake mesa-libGL-devel mesa-libGLU-devel libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel libXi-devel"
            echo ""
            
            if [ "$AUTO_FIX" = true ]; then
                echo ""
                echo -e "${CYAN}AutoFix Mode: Installing packages...${RESET}"
                echo ""
                echo -e "${YELLOW}The following command will be executed:${RESET}"
                echo "  $INSTALL_CMD gcc-c++ cmake mesa-libGL-devel mesa-libGLU-devel libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel libXi-devel"
                echo ""
                read -p "Continue with installation? (y/n) " -n 1 -r
                echo ""
                if [[ ! $REPLY =~ ^[Yy]$ ]]; then
                    print_info "Installation cancelled"
                    exit 1
                fi
                
                $INSTALL_CMD gcc-c++ cmake mesa-libGL-devel mesa-libGLU-devel libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel libXi-devel
                
                if [ $? -eq 0 ]; then
                    echo ""
                    print_success "Packages installed successfully!"
                    print_info "You can now build the project"
                    exit 0
                else
                    print_error "Failed to install packages"
                    exit 1
                fi
            fi
        else
            echo -e "${CYAN}Install Required Packages${RESET}"
            echo "  Please install the following packages using your system's package manager:"
            echo "  - build-essential or gcc-c++"
            echo "  - cmake"
            echo "  - OpenGL development libraries"
            echo "  - X11 development libraries"
            echo ""
        fi
        
        echo -e "${CYAN}Or run this script with -autofix flag to install automatically:${RESET}"
        echo "  ./tools/check-compiler.sh -autofix"
        echo ""
        
        exit 1
    fi

# ============================================================================
# Check for C++ Compiler (macOS)
# ============================================================================
elif [ "$OS" = "macos" ]; then
    print_header "Step 2: Checking for C++ Compiler (macOS)"
    
    COMPILER_FOUND=false
    
    # Check for clang
    if command -v clang++ &> /dev/null; then
        CLANG_VERSION=$(clang++ --version | head -n1)
        print_success "clang++ found: $CLANG_VERSION"
        COMPILER_FOUND=true
    else
        print_error "clang++ not found"
    fi
    
    # Check for Xcode Command Line Tools
    if xcode-select -p &> /dev/null; then
        XCODE_PATH=$(xcode-select -p)
        print_success "Xcode Command Line Tools found: $XCODE_PATH"
        COMPILER_FOUND=true
    else
        print_error "Xcode Command Line Tools not found"
    fi
    
    # Summary
    if [ "$COMPILER_FOUND" = true ]; then
        print_header "Detection Summary"
        echo ""
        echo -e "${GREEN}════════════════════════════════════════════════════════════════════════${RESET}"
        echo -e "${GREEN}✓ SUCCESS: C++ compiler is properly installed!${RESET}"
        echo -e "${GREEN}════════════════════════════════════════════════════════════════════════${RESET}"
        echo ""
        print_info "You can now build the project using build.sh or CMake"
        exit 0
    else
        print_header "Detection Summary"
        echo ""
        echo -e "${RED}════════════════════════════════════════════════════════════════════════${RESET}"
        echo -e "${RED}✗ MISSING COMPONENTS DETECTED${RESET}"
        echo -e "${RED}════════════════════════════════════════════════════════════════════════${RESET}"
        echo ""
        echo -e "${YELLOW}Xcode Command Line Tools are required but not installed.${RESET}"
        echo ""
        
        print_header "How to Fix"
        echo ""
        echo -e "${CYAN}Option 1: Install Xcode Command Line Tools (Recommended)${RESET}"
        echo "  Run this command:"
        echo ""
        echo "  xcode-select --install"
        echo ""
        echo "  This will open a dialog to install the command line tools."
        echo "  Follow the prompts to complete the installation."
        echo ""
        
        if [ "$AUTO_FIX" = true ]; then
            echo ""
            echo -e "${CYAN}AutoFix Mode: Launching Xcode Command Line Tools installer...${RESET}"
            echo ""
            xcode-select --install 2>&1
            
            EXIT_CODE=$?
            if [ $EXIT_CODE -eq 0 ]; then
                echo ""
                print_info "Please complete the installation in the dialog that appeared."
                print_info "After installation is complete, run this script again to verify."
            elif [ $EXIT_CODE -eq 1 ]; then
                # Exit code 1 usually means already installed
                print_warning "Command Line Tools may already be installed but corrupted"
                print_info "Try reinstalling with these steps:"
                print_info "  1. sudo rm -rf /Library/Developer/CommandLineTools"
                print_info "  2. xcode-select --install"
            else
                print_error "Failed to launch installer (exit code: $EXIT_CODE)"
            fi
            exit 1
        fi
        
        echo -e "${CYAN}Option 2: Install Full Xcode (Alternative)${RESET}"
        echo "  1. Open the App Store"
        echo "  2. Search for 'Xcode'"
        echo "  3. Install Xcode"
        echo "  4. Run: sudo xcodebuild -license accept"
        echo ""
        
        echo -e "${CYAN}Or run this script with -autofix flag to launch the installer:${RESET}"
        echo "  ./tools/check-compiler.sh -autofix"
        echo ""
        
        exit 1
    fi
fi
