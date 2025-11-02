# Quick Start Guide - 3D Game Engine

This guide will help you get the 3D Game Engine running on your Windows machine in minutes.

## Step 1: Prerequisites

### Install Required Software

1. **Git for Windows**
   - Download from: https://git-scm.com/download/win
   - Install with default options

2. **CMake**
   - Download from: https://cmake.org/download/
   - Choose "Windows x64 Installer"
   - During installation, select "Add CMake to system PATH"

3. **Visual Studio 2022 or 2019** (Community Edition is free)
   - **Recommended:** Visual Studio Community 2022 (version 17.8 or newer, preferably 17.11+)
   - **Alternative:** Visual Studio Community 2019 (version 16.11 or newer)
   - Download from: https://visualstudio.microsoft.com/downloads/
   - During installation, select "Desktop development with C++"
   - **📖 For detailed installation with visual instructions, see [VISUAL_STUDIO_SETUP.md](VISUAL_STUDIO_SETUP.md)**

## Step 2: Clone and Build

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
   - Download dependencies (GLFW, GLM)
   - Configure the project
   - Compile the engine
   - Create the executable

## Step 3: Run the Engine

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

### "CMake is not installed"
- Make sure CMake is installed and added to PATH
- Restart your command prompt after installation
- Download from: https://cmake.org/download/

### "Visual Studio not found"
- Install Visual Studio with C++ desktop development workload
- Or use MinGW: `cmake -G "MinGW Makefiles" ..` in the build directory

### "GameEngine.exe not found"
- Make sure build.bat completed successfully
- Check for error messages in the build output
- Check README.md Troubleshooting section for detailed solutions

### Performance Issues
- The engine is optimized, but large terrains may need a good GPU
- Reduce terrain size in src/main.cpp (change 100, 100 to 50, 50)

### Build takes forever (>20 minutes)
- First build compiles Assimp library (~3000 files) and takes 10-15 minutes
- This is normal! Subsequent builds are much faster (10-30 seconds)
- Speed up with: `cmake --build . --config Release -j 8` (uses 8 cores)

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
