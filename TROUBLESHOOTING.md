# GameEngine Build Troubleshooting Guide

This guide helps you resolve common build issues across all platforms (Windows, Linux, macOS).

## Table of Contents
- [Windows Issues](#windows-issues)
- [Linux Issues](#linux-issues)
- [macOS Issues](#macos-issues)
- [Common Issues (All Platforms)](#common-issues-all-platforms)
- [Automated Fix Tools](#automated-fix-tools)

---

## Windows Issues

### Error: "No CMAKE_CXX_COMPILER could be found" or "The CXX compiler identification is unknown"

**Symptoms:**
```
-- Building for: Visual Studio 17 2022
-- The CXX compiler identification is unknown
CMake Error at CMakeLists.txt:26 (project):
  No CMAKE_CXX_COMPILER could be found.
```

**Cause:** Visual Studio is installed without the C++ compiler tools.

**Quick Fix (Automated):**
```batch
tools\check-vs2022-compiler.bat -autofix
```

**Manual Fix:**
1. Open **Visual Studio Installer** (search in Start Menu)
2. Click **"Modify"** on your Visual Studio 2022 installation
3. Select the **"Desktop development with C++"** workload
4. Ensure these components are checked:
   - ✅ MSVC v143 - VS 2022 C++ x64/x86 build tools
   - ✅ Windows 11 SDK (or Windows 10 SDK)
   - ✅ C++ CMake tools for Windows
5. Click **"Modify"** to install (takes 10-20 minutes)
6. Delete the `build` folder
7. Run `build.bat` again

**Alternative:** If you don't have Visual Studio:
1. Download [Visual Studio 2022 Community](https://visualstudio.microsoft.com/downloads/) (free)
2. During installation, select **"Desktop development with C++"** workload
3. Complete installation
4. Run `build.bat`

---

### Error: "CMake is not recognized as an internal or external command"

**Cause:** CMake is not in your system PATH.

**Quick Fix (Automated):**
```batch
setup.bat
```
This will download a portable CMake if needed.

**Manual Fix - Option 1 (Reinstall with PATH):**
1. Download CMake from https://cmake.org/download/
2. During installation, select **"Add CMake to the system PATH for all users"**
3. Restart Command Prompt
4. Run `cmake --version` to verify

**Manual Fix - Option 2 (Add to PATH manually):**
1. Press **Windows + R**, type `sysdm.cpl`, press Enter
2. Click **"Environment Variables"**
3. Under **"System variables"**, select **"Path"**, click **"Edit"**
4. Click **"New"** and add your CMake bin directory (e.g., `C:\Program Files\CMake\bin`)
5. Click **"OK"** on all dialogs
6. Restart Command Prompt
7. Verify with `cmake --version`

---

### Error: "LINK : fatal error LNK1104: cannot open file 'opengl32.lib'"

**Cause:** Windows SDK is missing.

**Fix:**
1. Open Visual Studio Installer
2. Click "Modify" on Visual Studio 2022
3. Go to **Individual components** tab
4. Search for "Windows SDK"
5. Check the latest Windows 10 or Windows 11 SDK
6. Click "Modify" to install

---

## Linux Issues

### Error: "No CMAKE_CXX_COMPILER could be found"

**Symptoms:**
```
-- The CXX compiler identification is unknown
CMake Error at CMakeLists.txt:26 (project):
  No CMAKE_CXX_COMPILER could be found.
```

**Cause:** C++ compiler (g++) is not installed.

**Quick Fix (Automated):**
```bash
./tools/check-compiler.sh -autofix
```

**Manual Fix:**

**Debian/Ubuntu:**
```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake libgl1-mesa-dev libglu1-mesa-dev \
    libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
```

**RHEL/CentOS/Fedora:**
```bash
sudo yum groupinstall 'Development Tools'
sudo yum install -y cmake mesa-libGL-devel mesa-libGLU-devel \
    libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel libXi-devel
```

Or with dnf (Fedora):
```bash
sudo dnf groupinstall 'Development Tools'
sudo dnf install -y cmake mesa-libGL-devel mesa-libGLU-devel \
    libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel libXi-devel
```

**Arch Linux:**
```bash
sudo pacman -S base-devel cmake mesa glu libx11 libxrandr libxinerama libxcursor libxi
```

---

### Error: "Could NOT find OpenGL"

**Cause:** OpenGL development libraries are not installed.

**Fix:**

**Debian/Ubuntu:**
```bash
sudo apt-get install -y libgl1-mesa-dev libglu1-mesa-dev
```

**RHEL/CentOS/Fedora:**
```bash
sudo yum install -y mesa-libGL-devel mesa-libGLU-devel
```

---

### Error: "Could NOT find X11"

**Cause:** X11 development libraries required by GLFW are not installed.

**Fix:**

**Debian/Ubuntu:**
```bash
sudo apt-get install -y libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
```

**RHEL/CentOS/Fedora:**
```bash
sudo yum install -y libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel libXi-devel
```

---

## macOS Issues

### Error: "No CMAKE_CXX_COMPILER could be found"

**Symptoms:**
```
-- The CXX compiler identification is unknown
CMake Error at CMakeLists.txt:26 (project):
  No CMAKE_CXX_COMPILER could be found.
```

**Cause:** Xcode Command Line Tools are not installed.

**Quick Fix (Automated):**
```bash
./tools/check-compiler.sh -autofix
```

**Manual Fix:**
```bash
xcode-select --install
```

This opens a dialog to install the command line tools. Follow the prompts.

**Verify installation:**
```bash
clang++ --version
```

---

### Error: "xcrun: error: invalid active developer path"

**Cause:** Xcode Command Line Tools are not installed or corrupted.

**Fix:**
```bash
xcode-select --install
```

If already installed but corrupted:
```bash
sudo rm -rf /Library/Developer/CommandLineTools
xcode-select --install
```

---

### Error: "Could NOT find OpenGL"

**Cause:** This is unusual on macOS as OpenGL is included. May indicate outdated macOS.

**Fix:**
1. Update macOS to the latest version
2. Reinstall Xcode Command Line Tools:
   ```bash
   sudo rm -rf /Library/Developer/CommandLineTools
   xcode-select --install
   ```

---

## Common Issues (All Platforms)

### Error: "Could not find GLFW" or "Could not find GLM" or "Could not find Assimp"

**Cause:** Git submodules were not initialized.

**Fix:**
```bash
# Any platform
git submodule update --init --recursive
```

Then delete the `build` directory and rebuild:

**Windows:**
```batch
rmdir /s /q build
build.bat
```

**Linux/macOS:**
```bash
rm -rf build
./build.sh
```

---

### Error: Long build time (>30 minutes on first build)

**This is normal!** The first build compiles Assimp (~3000 files) which takes 10-20 minutes.

**Speed up:**

**Windows:**
```batch
cmake --build build --config Release -j 8
```

**Linux/macOS:**
```bash
make -j$(nproc)
```

Subsequent rebuilds only take 10-30 seconds.

---

### Error: Black screen or window doesn't open

**Cause:** GPU doesn't support OpenGL 3.3+.

**Fix:**
1. Update graphics drivers:
   - **NVIDIA:** https://www.nvidia.com/Download/index.aspx
   - **AMD:** https://www.amd.com/en/support
   - **Intel:** https://www.intel.com/content/www/us/en/download-center/home.html
2. Check if your GPU supports OpenGL 3.3+:
   ```bash
   # Linux
   glxinfo | grep "OpenGL version"
   
   # macOS
   # OpenGL 3.3+ is standard on macOS 10.9+
   ```

---

### Error: "fatal: not a git repository"

**Cause:** Not running commands from the GameEngine directory.

**Fix:**
```bash
cd /path/to/GameEngine
# Then run build commands
```

---

### Error: Build fails with hundreds of errors in external libraries

**Cause:** Corrupted submodules or interrupted download.

**Fix:**
```bash
# Clean submodules
git submodule deinit --all -f
git submodule update --init --recursive

# Clean build
rm -rf build  # or rmdir /s /q build on Windows
./build.sh    # or build.bat on Windows
```

---

## Automated Fix Tools

We provide automated tools to detect and fix compiler issues:

### Windows
```batch
# Check for issues
tools\check-vs2022-compiler.bat

# Check and auto-fix
tools\check-vs2022-compiler.bat -autofix

# Run full setup (includes checks)
setup.bat
```

### Linux/macOS
```bash
# Check for issues
./tools/check-compiler.sh

# Check and auto-fix
./tools/check-compiler.sh -autofix

# Run full setup (includes checks)
./setup.sh
```

---

## Getting More Help

If you're still experiencing issues:

1. **Check the error message carefully** - it often contains the solution
2. **Run the automated detection tools** - they provide specific fix instructions
3. **Check existing documentation:**
   - [README.md](README.md) - Main documentation
   - [WINDOWS_TROUBLESHOOTING.md](WINDOWS_TROUBLESHOOTING.md) - Windows-specific issues
   - [tools/README.md](tools/README.md) - Tool documentation
4. **Clean build:** Delete `build` folder and try again
5. **Update dependencies:** Run `git submodule update --init --recursive`
6. **Open an issue:** Include your OS, error message, and what you've tried

---

## Success Indicators

You'll know the build is working when you see:

**Windows:**
```
-- The CXX compiler identification is MSVC 19.xx.xxxxx
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Configuring done
-- Generating done
-- Build files have been written to: .../GameEngine/build
```

**Linux/macOS:**
```
-- The CXX compiler identification is GNU 11.x.x
-- The CXX compiler identification is AppleClang 14.x.x
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Configuring done
-- Generating done
-- Build files have been written to: .../GameEngine/build
```

Then the build should complete successfully!
