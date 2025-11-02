# Quick Start Guide - 3D Game Engine

This guide will help you get the 3D Game Engine running on your Windows machine in minutes.

## 🚀 Choose Your Path

### Path 1: Pre-built Binaries (Easiest - No Compilation!)

**Don't want to install Visual Studio or compile code?**

👉 **[Download Pre-built Binaries](DOWNLOAD_BINARIES.md)**

- ✅ Download ready-to-use executables
- ✅ No Visual Studio needed
- ✅ No build tools required
- ✅ Just download, extract, and run!

**Continue below only if you want to build from source.**

---

### Path 2: Build from Source (For Developers)

Follow the steps below to set up your development environment and build the engine yourself.

**🆕 NEW: Build Without Visual Studio!**

You now have two options for building on Windows:

**Option A: MinGW (Recommended - No Visual Studio!)**
- Just double-click `build-mingw.bat`
- Downloads MinGW automatically
- No 10+ GB Visual Studio installation
- See [BUILD_WITHOUT_VS.md](BUILD_WITHOUT_VS.md) for details

**Option B: Visual Studio (Traditional Method)**
- Requires Visual Studio installation
- Continue with Step 1 below

## Step 1: Prerequisites

**⚠️ Having installation issues?** Check [TROUBLESHOOTING.md](TROUBLESHOOTING.md) for solutions to common problems.

### Option A: MinGW Build (No Visual Studio Required!)

**Simple: Just run `build-mingw.bat`!**

The script will automatically handle everything. You only need:

1. **Git for Windows**
   - Download from: https://git-scm.com/download/win
   - Install with default options

2. **CMake** (optional - script can download portable version)
   - Download from: https://cmake.org/download/
   - Choose "Windows x64 Installer"
   - During installation, select "Add CMake to system PATH"

Then simply double-click `build-mingw.bat` and it will:
- Download MinGW-w64 automatically
- Configure the build
- Compile the engine
- Create the executable

**For details, see [BUILD_WITHOUT_VS.md](BUILD_WITHOUT_VS.md)**

### Option B: Visual Studio Build (Traditional Method)

**Install Required Software:**

1. **Git for Windows**
   - Download from: https://git-scm.com/download/win
   - Install with default options

2. **CMake**
   - Download from: https://cmake.org/download/
   - Choose "Windows x64 Installer"
   - During installation, select "Add CMake to system PATH"
   - **💡 TIP:** Run `setup.bat` instead to automatically download portable CMake if needed

3. **Visual Studio 2022 or 2019** (Community Edition is free)
   - **Recommended:** Visual Studio Community 2022 (version 17.8 or newer, including 17.14.x)
   - **Tested versions:** VS 2022 v17.8 through v17.14 (all fully supported)
   - **Alternative:** Visual Studio Community 2019 (version 16.11 or newer)
   - Download from: https://visualstudio.microsoft.com/downloads/
   - During installation, select "Desktop development with C++"
   - **📖 For detailed installation with visual instructions, see [VISUAL_STUDIO_SETUP.md](VISUAL_STUDIO_SETUP.md)**
   - **⚠️ Installation problems?** See [WINDOWS_TROUBLESHOOTING.md](WINDOWS_TROUBLESHOOTING.md) for help

## Step 2: Clone and Build

**⚠️ Before proceeding:** If you encounter any errors, refer to [TROUBLESHOOTING.md](TROUBLESHOOTING.md) for solutions.

### For MinGW Build (Option A):

1. Open Command Prompt or PowerShell

2. Clone the repository:
   ```
   git clone --recursive https://github.com/shifty81/GameEngine.git
   cd GameEngine
   ```

3. Build the project:
   ```
   build-mingw.bat
   ```
   This will:
   - Download MinGW-w64 automatically (if not present)
   - Download dependencies (GLFW, GLM, Assimp)
   - Configure the project with MinGW
   - Compile the engine
   - Create the executable at `build-mingw\bin\GameEngine.exe`
   
   **⚠️ Build failed?** See [BUILD_WITHOUT_VS.md](BUILD_WITHOUT_VS.md) troubleshooting section

### For Visual Studio Build (Option B):

1. Open Command Prompt or PowerShell

2. Clone the repository:
   ```
   git clone --recursive https://github.com/shifty81/GameEngine.git
   cd GameEngine
   ```

3. Build the project:
   ```
   build.bat
   ```
   This will:
   - Download dependencies (GLFW, GLM, Assimp)
   - Configure the project with Visual Studio
   - Compile the engine
   - Create the executable at `build\bin\Release\GameEngine.exe`
   
   **⚠️ Build failed?** Check the error message and refer to:
   - [TROUBLESHOOTING.md](TROUBLESHOOTING.md) for general build issues
   - [WINDOWS_TROUBLESHOOTING.md](WINDOWS_TROUBLESHOOTING.md) for Windows-specific problems
   - The [Troubleshooting](#troubleshooting) section below for quick fixes

## Step 3: Run the Engine

**For MinGW Build:**
```
build-mingw\bin\GameEngine.exe
```

**For Visual Studio Build:**
Simply double-click `run.bat` or run from command line:
```
run.bat
```

The game engine window will open with a procedurally generated 3D world!

## What You'll See

- **Procedural Terrain**: A landscape generated using Perlin noise
- **Cell Shading**: Cartoon-style rendering (toggle with C key)
- **Flowing Water**: Animated water surface
- **Dynamic Sky**: Beautiful sky blue background

## Controls

- **WASD** - Move around
- **Mouse** - Look around
- **Space** - Fly up
- **Left Shift** - Fly down
- **F** - Toggle wireframe mode
- **C** - Toggle cell shading
- **ESC** - Exit

## Features Included

✓ Cell shading (toon rendering)
✓ Procedural world generation
✓ Cave systems
✓ Flowing water system
✓ Wind system (ready for windmills)
✓ Procedural texture generation

## Troubleshooting

**⚠️ For comprehensive troubleshooting**, see:
- **[TROUBLESHOOTING.md](TROUBLESHOOTING.md)** - Complete troubleshooting guide for all platforms
- **[WINDOWS_TROUBLESHOOTING.md](WINDOWS_TROUBLESHOOTING.md)** - Windows-specific detailed solutions
- **[README.md](README.md)** - Full documentation with advanced troubleshooting

### "No CMAKE_C_COMPILER could be found" or "No CMAKE_CXX_COMPILER could be found"

This is the most common error! It means Visual Studio is installed but the C++ compiler is not.

**How to fix:**
1. Open **Visual Studio Installer** (search in Windows Start menu)
2. Click **"Modify"** button on your Visual Studio installation
3. Check the box for **"Desktop development with C++"** workload
4. Click **"Modify"** to install (takes 10-15 minutes)
5. Delete your `GameEngine\build` folder
6. Run `build.bat` again

**Don't have Visual Studio?**
- Download Visual Studio Community (free): https://visualstudio.microsoft.com/downloads/
- During installation, select **"Desktop development with C++"**

**Why this happens:** CMake found Visual Studio but the actual C++ compiler tools (cl.exe, MSBuild, Windows SDK) aren't installed. The "Desktop development with C++" workload includes everything needed.

**💡 For step-by-step instructions with screenshots**, see [WINDOWS_TROUBLESHOOTING.md](WINDOWS_TROUBLESHOOTING.md#error-no-cmake_cxx_compiler-could-be-found-or-the-cxx-compiler-identification-is-unknown).

### "CMake is not installed"
- Make sure CMake is installed and added to PATH
- Restart your command prompt after installation
- Download from: https://cmake.org/download/
- **OR** run `setup.bat` to automatically download portable CMake
- **💡 More help:** See [TROUBLESHOOTING.md](TROUBLESHOOTING.md#error-cmake-is-not-recognized-as-an-internal-or-external-command)

### "Visual Studio not found"
- Install Visual Studio with C++ desktop development workload
- Or use MinGW: `cmake -G "MinGW Makefiles" ..` in the build directory
- **💡 Detailed fix:** See [WINDOWS_TROUBLESHOOTING.md](WINDOWS_TROUBLESHOOTING.md#error-no-cmake_cxx_compiler-could-be-found-or-the-cxx-compiler-identification-is-unknown)

### "GameEngine.exe not found"
- Make sure build.bat completed successfully
- Check for error messages in the build output
- Check README.md Troubleshooting section for detailed solutions
- **💡 Complete guide:** See [TROUBLESHOOTING.md](TROUBLESHOOTING.md) for all build-related issues

### Performance Issues
- The engine is optimized, but large terrains may need a good GPU
- Reduce terrain size in src/main.cpp (change 100, 100 to 50, 50)

### Build takes forever (>20 minutes)
- First build compiles Assimp library (~3000 files) and takes 10-15 minutes
- This is normal! Subsequent builds are much faster (10-30 seconds)
- Speed up with: `cmake --build . --config Release -j 8` (uses 8 cores)
- **💡 More optimization tips:** See [TROUBLESHOOTING.md](TROUBLESHOOTING.md#error-long-build-time-30-minutes-on-first-build)

## Customization

### Change Terrain Size
Edit `src/main.cpp`, line ~187:
```cpp
Terrain terrain(50, 50, 15.0f, 12345);  // Smaller terrain
```

### Change Water Level
Edit `src/main.cpp`, line ~190:
```cpp
Water water(10.0f, 50, 50);  // Higher water level
```

### Modify Textures
Generated textures are saved in `textures/` folder:
- Replace these PPM files with your own
- Restart the engine to see changes

## Next Steps

1. Explore the procedurally generated world
2. Check out the source code in `src/` directory
3. Read the full README.md for more details
4. Experiment with different settings

## Need Help?

- Check the full README.md file
- Review the source code comments
- Open an issue on GitHub

Enjoy building with the 3D Game Engine!
