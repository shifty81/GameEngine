# One-Click Build System

## Overview

The **OneClickBuild.bat** script provides a fully automated, one-click solution to build the GameEngine on Windows. It handles all prerequisite checking, installation guidance, and builds the project automatically.

## Features

- ✅ **Fully Automated** - One double-click to build everything
- 🔍 **Prerequisite Detection** - Automatically checks for all required tools
- 🛠️ **Smart Installation Help** - Guides you through installing missing tools
- 🔧 **Compiler Validation** - Verifies Visual Studio C++ compiler is properly installed
- 📦 **Dependency Management** - Automatically initializes git submodules
- 🎯 **Error Handling** - Clear error messages with actionable solutions
- ⚡ **Fast Rebuilds** - Incremental compilation for quick iterations

## Quick Start

### For First-Time Users

1. **Download** the GameEngine repository
2. **Double-click** `OneClickBuild.bat` in the root directory
3. **Follow** any on-screen prompts if prerequisites are missing
4. **Wait** for the build to complete (10-15 minutes first time)
5. **Run** the engine using `run.bat` or `build\bin\Release\GameEngine.exe`

That's it! The script handles everything else automatically.

## What It Does

The OneClickBuild.bat script performs these steps automatically:

### 1. **System Check**
- Detects if running with administrator privileges
- Checks for required build tools

### 2. **Git Verification**
- Verifies Git is installed
- Provides download link if missing

### 3. **CMake Detection**
- Checks if CMake is installed and in PATH
- Offers to help download portable CMake if missing

### 4. **Visual Studio Validation**
- Detects Visual Studio 2022 or 2019 installation
- Verifies C++ compiler (MSVC) is installed
- Runs advanced diagnostics using `check-vs2022-compiler.bat`
- Offers automatic fix via Visual Studio Installer

### 5. **Dependency Setup**
- Automatically runs `git submodule update --init --recursive`
- Downloads GLFW, GLM, Assimp libraries

### 6. **Build Configuration**
- Creates `build` directory
- Runs CMake to generate Visual Studio project files
- Configures for Release build

### 7. **Compilation**
- Builds the entire project using CMake
- Compiles all dependencies and source files
- Creates executable in `build\bin\Release\GameEngine.exe`

### 8. **Success Report**
- Reports build status
- Shows executable location
- Provides instructions for running the engine

## Prerequisites

The script will check for and help you install:

### Required Tools

1. **Git for Windows**
   - Download: https://git-scm.com/download/win
   - Used for repository management and submodules

2. **CMake 3.20 or higher**
   - Download: https://cmake.org/download/
   - Used to generate build files
   - Select "Add CMake to system PATH" during installation

3. **Visual Studio 2022 or 2019**
   - Download: https://visualstudio.microsoft.com/downloads/
   - Community Edition is free
   - Must include "Desktop development with C++" workload

## Usage

### Basic Usage

Simply double-click the file:
```
OneClickBuild.bat
```

Or run from Command Prompt:
```bash
cd GameEngine
OneClickBuild.bat
```

### After First Build

After the initial build, you can use faster methods for rebuilds:

**Option 1: Use build.bat (faster incremental builds)**
```bash
build.bat
```

**Option 2: Use OneClickBuild.bat again**
```bash
OneClickBuild.bat
```

**Option 3: Build from Visual Studio**
- Open `build\GameEngine.sln`
- Press F7 or Build → Build Solution

## Interactive Prompts

The script may ask for confirmation in certain situations:

### Missing CMake
```
Would you like to download a portable version of CMake? (Y/N)
```
- **Y**: Opens browser to CMake download page
- **N**: Exits and asks you to install manually

### Compiler Issues Detected
```
Would you like to run the diagnostic tool to fix this? (Y/N)
```
- **Y**: Runs detailed compiler diagnostics
- **N**: Continues or exits

### Auto-Fix Compiler
```
Would you like to automatically fix the issues? (Y/N)
```
- **Y**: Launches Visual Studio Installer to fix C++ tools
- **N**: Provides manual instructions

## Build Times

### First Build
- **Time**: 10-15 minutes
- **Why**: Compiles all dependencies (Assimp ~3000 files, GLFW, etc.)
- **Note**: This is normal and only happens once

### Subsequent Builds
- **Time**: 10-30 seconds
- **Why**: Only recompiles changed files (incremental compilation)
- **Tip**: Use `build.bat` for even faster incremental builds

## Output Locations

After a successful build:

```
GameEngine/
├── build/
│   ├── bin/
│   │   └── Release/
│   │       └── GameEngine.exe    ← Main executable here
│   └── GameEngine.sln             ← Visual Studio solution
```

## Running the Engine

After building, run the engine using any of these methods:

**Method 1: Use run.bat**
```bash
run.bat
```

**Method 2: Direct execution**
```bash
build\bin\Release\GameEngine.exe
```

**Method 3: Windows Explorer**
- Navigate to `build\bin\Release\`
- Double-click `GameEngine.exe`

## Troubleshooting

### Error: Git not found

**Solution:**
1. Download Git from https://git-scm.com/download/win
2. Install it (select default options)
3. Restart Command Prompt
4. Run OneClickBuild.bat again

### Error: CMake not found

**Solution:**
1. Download CMake from https://cmake.org/download/
2. Run installer and select "Add CMake to system PATH"
3. Restart Command Prompt
4. Run OneClickBuild.bat again

### Error: Visual Studio C++ compiler not found

**Solution:**
1. The script will offer to run diagnostics
2. Choose "Y" to run auto-fix
3. This will launch Visual Studio Installer
4. Install the "Desktop development with C++" workload
5. Wait for installation to complete
6. Run OneClickBuild.bat again

**Manual Solution:**
1. Download Visual Studio Community from https://visualstudio.microsoft.com/downloads/
2. Run installer
3. Select "Desktop development with C++" workload
4. Click Install
5. Wait for installation (10-15 minutes)
6. Run OneClickBuild.bat again

### Error: Failed to initialize git submodules

**Solution:**
- Check internet connection
- Verify Git is properly installed
- Try manually: `git submodule update --init --recursive`
- Run OneClickBuild.bat again

### Error: CMake configuration failed

**Common causes:**
- Visual Studio C++ compiler not installed
- CMake version too old (need 3.20+)
- Missing Visual Studio components

**Solution:**
1. Check error messages for specific issue
2. Run: `tools\check-vs2022-compiler.bat` for diagnostics
3. Install missing components
4. Delete `build` folder
5. Run OneClickBuild.bat again

### Error: Build failed

**Solution:**
1. Review error messages in console
2. Check TROUBLESHOOTING.md for specific errors
3. Try clean build:
   ```bash
   rmdir /s /q build
   OneClickBuild.bat
   ```

## Comparison with Manual Build

### OneClickBuild.bat
- ✅ Fully automated
- ✅ Prerequisite checking
- ✅ Interactive help
- ✅ Error guidance
- ⚠️ Longer initial setup time

### Manual Build (using build.bat)
- ✅ Faster for rebuilds
- ✅ More control
- ✅ Better for development
- ⚠️ Requires manual setup
- ⚠️ Must handle errors yourself

### Recommendation
- **First time?** Use `OneClickBuild.bat`
- **Development?** Use `build.bat` for faster incremental builds
- **Having issues?** Use `OneClickBuild.bat` for diagnostics

## Advanced Options

### Silent Mode (Future Enhancement)

Currently not implemented, but future versions may support:
```bash
OneClickBuild.bat /silent
```

### Clean Build

To force a complete rebuild:
```bash
rmdir /s /q build
OneClickBuild.bat
```

### Configuration Options

The script uses these defaults:
- **Build Type**: Release (optimized)
- **Target Platform**: x64
- **CMake Generator**: Auto-detected (Visual Studio)

## Integration with Existing Tools

OneClickBuild.bat complements existing tools:

### Works with:
- `build.bat` - Use for faster incremental builds
- `run.bat` - Run the built executable
- `setup.bat` - Additional setup and diagnostics
- Visual Studio - Open generated solution for development

### Calls internally:
- `tools\check-vs2022-compiler.bat` - For compiler validation
- `git` - For submodule management
- `cmake` - For build generation

## System Requirements

### Minimum Requirements:
- **OS**: Windows 10 or later
- **Disk Space**: 5 GB free
- **RAM**: 4 GB minimum, 8 GB recommended
- **Internet**: Required for first-time dependency download

### Supported Configurations:
- Visual Studio 2022 (v17.8 through v17.14+)
- Visual Studio 2019 (v16.11+)
- CMake 3.20 or higher
- Git 2.0 or higher

## For Developers

### Modifying the Script

The script is well-commented and easy to modify:

```batch
REM Add custom build steps here
echo [Custom] Running custom build step...
```

### Adding Checks

Add custom prerequisite checks:

```batch
REM Check for custom tool
where mytool >nul 2>&1
if %errorLevel% neq 0 (
    echo [✗] MyTool not found
    exit /b 1
)
```

### Integration with CI/CD

For automated builds, use:
```bash
OneClickBuild.bat
if %errorLevel% neq 0 exit /b 1
```

## Related Documentation

- [TROUBLESHOOTING.md](TROUBLESHOOTING.md) - Detailed troubleshooting guide
- [WINDOWS_TROUBLESHOOTING.md](WINDOWS_TROUBLESHOOTING.md) - Windows-specific issues
- [VISUAL_STUDIO_SETUP.md](VISUAL_STUDIO_SETUP.md) - Visual Studio installation guide
- [README.md](README.md) - General build instructions
- [SCRIPT_COMPILER.md](SCRIPT_COMPILER.md) - Script system documentation

## Feedback

If you encounter issues not covered here:
1. Check [TROUBLESHOOTING.md](TROUBLESHOOTING.md)
2. Review error messages carefully
3. Open an issue on GitHub with error details

## Updates

### Version 1.0 (Current)
- Initial release
- Full prerequisite checking
- Visual Studio compiler validation
- Automatic submodule initialization
- Interactive error handling
- Integration with existing tools

### Future Improvements
- Silent/unattended mode
- Custom configuration options
- Progress indicators
- Parallel compilation option
- Build cache management
