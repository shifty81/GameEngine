# Compiler Detection Tool for Ubuntu 24.04

This directory contains tools to automatically detect and help fix C++ compiler issues on Ubuntu 24.04 LTS.

## Files

### check-compiler.sh

Shell script that performs comprehensive detection of C++ compiler and required development libraries for Ubuntu 24.04 LTS.

**Features:**
- Verifies GCC 13.3+ is installed
- Checks for required development libraries (OpenGL, X11, etc.)
- Provides detailed error messages with fix instructions
- Can automatically install missing packages with `-autofix` flag (requires sudo)

**Usage:**
```bash
# Basic detection (shows detailed output)
./tools/check-compiler.sh

# Quiet mode (minimal output, useful for scripts)
./tools/check-compiler.sh -quiet

# Automatic fix (installs missing packages)
./tools/check-compiler.sh -autofix

# Combine flags
./tools/check-compiler.sh -quiet -autofix
```

**Exit Codes:**
- `0`: Success - Compiler and libraries are properly installed
- `1`: Error - Compiler or libraries are missing

## Integration with Build Scripts

### setup.sh Integration
The `setup.sh` script calls the compiler detection tool during setup:
- Runs compiler detection
- If issues are found, offers to automatically install missing packages
- Provides option to continue anyway or abort setup

### build.sh Integration
The `build.sh` script checks the compiler before attempting to build:
- Runs quick compiler check in quiet mode
- If issues are found, offers detailed diagnostics
- Provides option to auto-fix or continue anyway
- Helps prevent confusing CMake errors by catching compiler issues early

## What the Detection Script Checks

1. **CMake** - Build system presence and version (3.28+)
2. **C++ Compiler** - GCC 13.3+ with C++20 support
3. **Build Tools** - make and other essential tools
4. **OpenGL Libraries**:
   - libGL.so (OpenGL)
   - libGLU.so (OpenGL Utility Library)
5. **X11 Libraries** (required for GLFW):
   - libX11.so (X Window System)
   - libXrandr.so (X11 RandR extension)
   - libXinerama.so (X11 Xinerama extension)
   - libXcursor.so (X11 cursor management)
   - libXi.so (X11 Input extension)

## Common Issues Detected

### Issue: Compiler not installed
**Detection:** g++ or make not found
**Fix:** Script installs build-essential package with `-autofix`

### Issue: OpenGL libraries missing
**Detection:** libGL.so or libGLU.so not found
**Fix:** Script installs mesa-libGL-dev packages with `-autofix`

### Issue: X11 libraries missing
**Detection:** X11 development libraries not found
**Fix:** Script installs required X11 development packages with `-autofix`

## Manual Usage

If you suspect compiler issues, run the detection manually:

```bash
cd /path/to/GameEngine
./tools/check-compiler.sh
```

This will show you detailed information about your compiler setup and any problems detected.

## Requirements

- Ubuntu 24.04 LTS (Noble Numbat)
- Bash shell
- sudo privileges (for autofix)
- Internet connection (for package installation)

## Troubleshooting

### Permission denied
Make sure the script is executable:
```bash
chmod +x tools/check-compiler.sh
```

### AutoFix requires sudo password
The autofix feature needs sudo privileges to install packages. You'll be prompted for your password.

### Package installation fails
If autofix fails to install packages:
1. Check your internet connection
2. Try updating package lists manually:
   ```bash
   sudo apt-get update
   ```
3. Install packages manually:
   ```bash
   sudo apt-get install -y build-essential cmake git \
       libgl1-mesa-dev libglu1-mesa-dev \
       libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
   ```

## Benefits

- **Early Error Detection**: Catch compiler issues before CMake configuration
- **Clear Error Messages**: Understand exactly what's missing
- **Automated Fixes**: Quick path to fixing common issues
- **Better User Experience**: Less confusion for developers setting up the project
- **Comprehensive Validation**: Checks multiple components, not just basic tools
- **Integration Ready**: Seamlessly integrates with setup and build scripts

## Technical Details

The detection script uses:
- `command -v` to check for compiler presence
- `ldconfig -p` to verify library installation
- `apt` package manager for installation
- Exit codes for script integration
- ANSI color codes for readable output

## See Also

- [UBUNTU_24_04.md](../UBUNTU_24_04.md) - Complete Ubuntu 24.04 LTS development guide
- [README.md](../README.md) - Main project documentation
- [setup.sh](../setup.sh) - Initial setup script
- [build.sh](../build.sh) - Build script
