# Compiler Detection and Fix Tools

This directory contains tools to automatically detect and help fix C++ compiler issues across all platforms (Windows, Linux, macOS).

## Files

### check-compiler.sh (Linux/macOS)
Cross-platform shell script that performs comprehensive detection of C++ compiler and required development libraries.

**Features:**
- Detects operating system (Linux or macOS)
- Verifies C++ compiler (g++/clang++) is installed
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

**Supported Systems:**
- Linux: Debian/Ubuntu (apt), RHEL/CentOS/Fedora (yum/dnf), Arch (pacman)
- macOS: Xcode Command Line Tools

---

### check-vs2022-compiler.ps1 (Windows)
PowerShell script that performs comprehensive detection of Visual Studio 2022 and C++ compiler components.

**Features:**
- Detects Visual Studio 2022 installation
- Verifies C++ compiler (MSVC) is installed
- Checks for required components (C++ build tools, Windows SDK)
- Validates cl.exe compiler executable
- Provides detailed error messages with fix instructions
- Can automatically open Visual Studio Installer with `-AutoFix` flag

**Usage:**
```powershell
# Basic detection (shows detailed output)
powershell -ExecutionPolicy Bypass -File tools\check-vs2022-compiler.ps1

# Quiet mode (minimal output, useful for scripts)
powershell -ExecutionPolicy Bypass -File tools\check-vs2022-compiler.ps1 -Quiet

# Automatic fix (opens Visual Studio Installer if issues found)
powershell -ExecutionPolicy Bypass -File tools\check-vs2022-compiler.ps1 -AutoFix

# Combine flags
powershell -ExecutionPolicy Bypass -File tools\check-vs2022-compiler.ps1 -Quiet -AutoFix
```

**Exit Codes:**
- `0`: Success - Compiler is properly installed
- `1`: Error - Compiler or components are missing

### check-vs2022-compiler.bat (Windows)
Batch file wrapper for the PowerShell script, making it easier to call from other batch files.

**Usage:**
```batch
call tools\check-vs2022-compiler.bat
call tools\check-vs2022-compiler.bat -quiet
call tools\check-vs2022-compiler.bat -autofix
call tools\check-vs2022-compiler.bat -quiet -autofix
```

## Integration with Build Scripts

### setup.sh Integration (Linux/macOS)
The `setup.sh` script calls the compiler detection tool during Step 3 of the setup process:
- Runs compiler detection
- If issues are found, offers to automatically install missing packages
- Provides option to continue anyway or abort setup

### build.sh Integration (Linux/macOS)
The `build.sh` script checks the compiler before attempting to build:
- Runs quick compiler check in quiet mode
- If issues are found, offers detailed diagnostics
- Provides option to auto-fix or continue anyway
- Helps prevent confusing CMake errors by catching compiler issues early

### setup.bat Integration (Windows)
The `setup.bat` script now calls the compiler detection tool during Step 3 of the setup process:
- Runs compiler detection
- If issues are found, offers to automatically open Visual Studio Installer
- Provides option to continue anyway or abort setup

### build.bat Integration (Windows)
The `build.bat` script now checks the compiler before attempting to build:
- Runs quick compiler check in quiet mode
- If issues are found, offers detailed diagnostics
- Provides option to auto-fix or continue anyway
- Helps prevent confusing CMake errors by catching compiler issues early

## What the Detection Scripts Check

### Windows (check-vs2022-compiler.ps1/.bat)
1. **vswhere.exe** - Visual Studio installer tools presence
2. **Visual Studio 2022** - VS 2022 installation (version 17.x)
3. **C++ Components**:
   - MSVC v143 C++ build tools
   - Windows SDK (10 or 11)
4. **MSVC Compiler** - cl.exe executable presence and version
5. **Windows SDK** - SDK installation and configuration

### Linux/macOS (check-compiler.sh)
1. **CMake** - Build system presence and version
2. **C++ Compiler**:
   - Linux: g++ (GCC) and make
   - macOS: clang++ and Xcode Command Line Tools
3. **OpenGL Libraries** (Linux):
   - libGL.so (OpenGL)
   - libGLU.so (OpenGL Utility Library)
4. **X11 Libraries** (Linux, required for GLFW):
   - libX11.so (X Window System)
   - libXrandr.so (X11 RandR extension)
   - libXinerama.so (X11 Xinerama extension)
   - libXcursor.so (X11 cursor management)
   - libXi.so (X11 Input extension)

## Common Issues Detected

### Windows

#### Issue: Visual Studio not installed
**Detection:** vswhere.exe not found or no VS 2022 installation
**Fix:** Script opens Visual Studio download page with `-AutoFix`

#### Issue: C++ workload not installed
**Detection:** MSVC component missing, cl.exe not found
**Fix:** Script opens Visual Studio Installer to modify installation

#### Issue: Partial installation
**Detection:** Some required components missing
**Fix:** Detailed list of missing components with installation instructions

---

### Linux

#### Issue: Compiler not installed
**Detection:** g++ or make not found
**Fix:** Script installs build-essential package with `-autofix`

#### Issue: OpenGL libraries missing
**Detection:** libGL.so or libGLU.so not found
**Fix:** Script installs mesa-libGL-dev packages with `-autofix`

#### Issue: X11 libraries missing
**Detection:** X11 development libraries not found
**Fix:** Script installs required X11 development packages with `-autofix`

---

### macOS

#### Issue: Xcode Command Line Tools not installed
**Detection:** clang++ not found or xcode-select fails
**Fix:** Script launches Xcode Command Line Tools installer with `-autofix`

#### Issue: Corrupted Xcode installation
**Detection:** xcode-select exists but compiler doesn't work
**Fix:** Instructions provided to reinstall Command Line Tools

## Manual Usage Examples

### Windows
If you suspect compiler issues, run the detection manually:

```batch
cd C:\path\to\GameEngine
tools\check-vs2022-compiler.bat
```

This will show you detailed information about your Visual Studio installation and any problems detected.

### Linux/macOS
If you suspect compiler issues, run the detection manually:

```bash
cd /path/to/GameEngine
./tools/check-compiler.sh
```

This will show you detailed information about your compiler setup and any problems detected.

## Requirements

### Windows
- Windows 10/11
- PowerShell 5.0 or higher (built into Windows)
- Visual Studio 2022 (Community, Professional, or Enterprise)
- Desktop development with C++ workload

### Linux
- Modern Linux distribution (Debian/Ubuntu, RHEL/CentOS/Fedora, Arch, etc.)
- Bash shell
- sudo privileges (for autofix)
- Internet connection (for package installation)

### macOS
- macOS 10.9 or later
- Bash or Zsh shell
- Internet connection (for Xcode Command Line Tools download)

## Troubleshooting

### Windows

#### "Running scripts is disabled on this system"
If you get a PowerShell execution policy error, you have two options:

1. Use the batch wrapper (recommended):
   ```batch
   tools\check-vs2022-compiler.bat
   ```

2. Or temporarily bypass the policy:
   ```powershell
   powershell -ExecutionPolicy Bypass -File tools\check-vs2022-compiler.ps1
   ```

#### Script shows wrong information
Try running as Administrator:
```batch
# Right-click Command Prompt -> "Run as administrator"
cd C:\path\to\GameEngine
tools\check-vs2022-compiler.bat
```

#### AutoFix doesn't work
If `-AutoFix` doesn't launch the installer:
1. Manually open "Visual Studio Installer" from Start Menu
2. Click "Modify" on Visual Studio 2022
3. Select "Desktop development with C++" workload
4. Click "Modify" button

---

### Linux/macOS

#### Permission denied
Make sure the script is executable:
```bash
chmod +x tools/check-compiler.sh
```

#### AutoFix requires sudo password
The autofix feature needs sudo privileges to install packages. You'll be prompted for your password.

#### Package installation fails
If autofix fails to install packages:
1. Check your internet connection
2. Try updating package lists manually:
   ```bash
   sudo apt-get update  # Debian/Ubuntu
   sudo yum update      # RHEL/CentOS
   ```
3. Install packages manually using the commands shown in the error output

## Benefits

- **Cross-Platform:** Works on Windows, Linux, and macOS
- **Early Error Detection**: Catch compiler issues before CMake configuration
- **Clear Error Messages**: Understand exactly what's missing
- **Automated Fixes**: Quick path to fixing common issues
- **Better User Experience**: Less confusion for developers setting up the project
- **Comprehensive Validation**: Checks multiple components, not just basic tools
- **Integration Ready**: Seamlessly integrates with setup and build scripts

## Technical Details

### Windows Implementation
The detection script uses:
- `vswhere.exe` to query Visual Studio installations
- File system checks for compiler binaries
- Component ID validation against installed packages
- Exit codes for script integration
- ANSI color codes for readable output

### Linux/macOS Implementation
The detection script uses:
- `command -v` to check for compiler presence
- `ldconfig -p` (Linux) to verify library installation
- Package manager detection (apt/yum/dnf/pacman/brew)
- Exit codes for script integration
- ANSI color codes for readable output

## See Also

- [TROUBLESHOOTING.md](../TROUBLESHOOTING.md) - Comprehensive troubleshooting guide for all platforms
- [WINDOWS_TROUBLESHOOTING.md](../WINDOWS_TROUBLESHOOTING.md) - Windows-specific troubleshooting
- [README.md](../README.md) - Main project documentation
- [setup.bat](../setup.bat) / [setup.sh](../setup.sh) - Initial setup scripts
- [build.bat](../build.bat) / [build.sh](../build.sh) - Build scripts
