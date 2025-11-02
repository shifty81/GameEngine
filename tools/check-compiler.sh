#!/bin/bash
# ============================================================================
# Ubuntu 24.04 C++ Compiler Detection and Fix Script
# ============================================================================
# This script detects C++ compiler installation and provides automated fixes
# when the compiler is not found.
# Optimized for: Ubuntu 24.04 LTS (Noble Numbat)
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
        echo -e "${CYAN}════════════════════════════════════════════════════════════════════════${RESET}"
        echo -e "${CYAN} $1${RESET}"
        echo -e "${CYAN}════════════════════════════════════════════════════════════════════════${RESET}"
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
        echo -e "${BLUE}  • $1${RESET}"
    fi
}

# Track issues
MISSING_PACKAGES=()
HAS_ERRORS=false

# ============================================================================
# Check CMake
# ============================================================================
print_header "Step 1: Checking for CMake"

if command -v cmake &> /dev/null; then
    CMAKE_VERSION=$(cmake --version | head -n1 | awk '{print $3}')
    print_success "CMake is installed: version $CMAKE_VERSION"
else
    print_error "CMake is not installed"
    MISSING_PACKAGES+=("cmake")
    HAS_ERRORS=true
    
    if [ "$QUIET" = false ]; then
        echo ""
        print_info "Install CMake with:"
        echo "  sudo apt-get update"
        echo "  sudo apt-get install -y cmake"
    fi
fi

# ============================================================================
# Check C++ Compiler
# ============================================================================
print_header "Step 2: Checking for C++ Compiler (GCC)"

if command -v g++ &> /dev/null; then
    GCC_VERSION=$(g++ --version | head -n1 | awk '{print $3}')
    print_success "GCC is installed: version $GCC_VERSION"
    
    # Check if it's GCC 13.3+ for best C++20 support
    MAJOR=$(echo $GCC_VERSION | cut -d. -f1)
    if [ "$MAJOR" -ge 13 ]; then
        print_success "GCC version is 13+ (excellent C++20 support)"
    elif [ "$MAJOR" -ge 10 ]; then
        print_warning "GCC version is $MAJOR (good C++20 support, but 13+ recommended for Ubuntu 24.04)"
    else
        print_warning "GCC version is $MAJOR (may have limited C++20 support, recommend upgrading)"
    fi
else
    print_error "GCC (g++) is not installed"
    MISSING_PACKAGES+=("build-essential")
    HAS_ERRORS=true
    
    if [ "$QUIET" = false ]; then
        echo ""
        print_info "Install GCC with:"
        echo "  sudo apt-get update"
        echo "  sudo apt-get install -y build-essential"
    fi
fi

# Check for make
if command -v make &> /dev/null; then
    print_success "make is installed"
else
    print_error "make is not installed"
    if [[ ! " ${MISSING_PACKAGES[@]} " =~ " build-essential " ]]; then
        MISSING_PACKAGES+=("build-essential")
    fi
    HAS_ERRORS=true
fi

# ============================================================================
# Check OpenGL Libraries
# ============================================================================
print_header "Step 3: Checking for OpenGL Libraries"

# Check for libGL
if ldconfig -p | grep -q libGL.so; then
    print_success "libGL (OpenGL) is installed"
else
    print_error "libGL (OpenGL) is not installed"
    MISSING_PACKAGES+=("libgl1-mesa-dev")
    HAS_ERRORS=true
fi

# Check for libGLU
if ldconfig -p | grep -q libGLU.so; then
    print_success "libGLU (OpenGL Utility) is installed"
else
    print_error "libGLU (OpenGL Utility) is not installed"
    MISSING_PACKAGES+=("libglu1-mesa-dev")
    HAS_ERRORS=true
fi

# ============================================================================
# Check X11 Libraries (Required for GLFW)
# ============================================================================
print_header "Step 4: Checking for X11 Libraries"

X11_LIBS=(
    "libX11.so:libx11-dev:X11 Core"
    "libXrandr.so:libxrandr-dev:X11 RandR"
    "libXinerama.so:libxinerama-dev:X11 Xinerama"
    "libXcursor.so:libxcursor-dev:X11 Cursor"
    "libXi.so:libxi-dev:X11 Input"
)

for lib_info in "${X11_LIBS[@]}"; do
    IFS=':' read -r lib_file pkg_name lib_name <<< "$lib_info"
    
    if ldconfig -p | grep -q "$lib_file"; then
        print_success "$lib_name is installed"
    else
        print_error "$lib_name is not installed"
        MISSING_PACKAGES+=("$pkg_name")
        HAS_ERRORS=true
    fi
done

# ============================================================================
# Summary and Fix
# ============================================================================
echo ""

if [ "$HAS_ERRORS" = false ]; then
    print_header "✓ SUCCESS: C++ compiler and all required libraries are installed!"
    echo ""
    exit 0
fi

# There are errors - show summary
print_header "✗ MISSING COMPONENTS DETECTED"
echo ""

if [ ${#MISSING_PACKAGES[@]} -gt 0 ]; then
    echo "The following packages are required but not installed:"
    echo ""
    
    # Remove duplicates and print
    UNIQUE_PACKAGES=($(printf "%s\n" "${MISSING_PACKAGES[@]}" | sort -u))
    for pkg in "${UNIQUE_PACKAGES[@]}"; do
        print_info "$pkg"
    done
    
    echo ""
    echo ""
    echo "Option 1: Install All Required Packages (Recommended)"
    echo "  For Ubuntu 24.04 LTS / Ubuntu 22.04 / Debian, run:"
    echo ""
    echo "  sudo apt-get update"
    echo "  sudo apt-get install -y build-essential cmake libgl1-mesa-dev libglu1-mesa-dev libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev"
    echo ""
    echo "  Note: Ubuntu 24.04 LTS comes with GCC 13.3 which has excellent C++20 support!"
    echo ""
    
    if [ "$AUTO_FIX" = false ]; then
        echo "Or run this script with -autofix flag to install automatically:"
        echo "  ./tools/check-compiler.sh -autofix"
        echo ""
    fi
fi

# Auto-fix if requested
if [ "$AUTO_FIX" = true ]; then
    echo ""
    print_header "AUTO-FIX: Installing missing packages"
    echo ""
    
    echo "Running: sudo apt-get update"
    sudo apt-get update
    
    echo ""
    echo "Running: sudo apt-get install -y build-essential cmake libgl1-mesa-dev libglu1-mesa-dev libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev"
    sudo apt-get install -y build-essential cmake libgl1-mesa-dev libglu1-mesa-dev libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
    
    echo ""
    print_success "All missing packages have been installed!"
    echo ""
    echo "You can now build the engine with: ./build.sh"
    echo ""
    exit 0
fi

exit 1
