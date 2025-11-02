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

3. **Visual Studio 2019 or newer** (Community Edition is free)
   - Download from: https://visualstudio.microsoft.com/downloads/
   - During installation, select "Desktop development with C++"

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

### "CMake is not installed"
- Make sure CMake is installed and added to PATH
- Restart your command prompt after installation

### "Visual Studio not found"
- Install Visual Studio with C++ desktop development workload
- Or use MinGW: `cmake -G "MinGW Makefiles" ..` in the build directory

### "GameEngine.exe not found"
- Make sure build.bat completed successfully
- Check for error messages in the build output

### Performance Issues
- The engine is optimized, but large terrains may need a good GPU
- Reduce terrain size in src/main.cpp (change 100, 100 to 50, 50)

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
