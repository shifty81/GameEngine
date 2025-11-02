# 3D Game Engine

A feature-rich 3D game engine built with C++ and OpenGL, featuring procedural world generation, cell shading, and advanced environmental systems.

## Features

### Core Features
- ✨ **Cell Shading** - Toon/cartoon-style rendering with customizable shading levels
- 🌍 **Procedural World Generation** - Infinite terrain generation using Perlin noise
- 🏔️ **Cave Systems** - Underground cavern generation for exploration
- 💧 **Flowing Water System** - Dynamic water with flow direction and animation
- 🌬️ **Wind System** - Realistic wind simulation for windmills and environmental effects
- 🎨 **Procedural Texture Generation** - Auto-generated textures that can be replaced
- 📦 **Asset Loading System** - Complete pipeline for 3D models and textures
- 🌳 **Stylized Nature Kit Support** - Ready for integration with asset packs

### Technical Features
- Modern OpenGL 3.3+ rendering
- Camera system with free movement
- Multi-octave Perlin noise for realistic terrain
- Real-time lighting and shading
- Transparent water rendering
- **Assimp** integration for loading .obj, .fbx, .gltf, and more
- **stb_image** for texture loading (png, jpg, tga, etc.)
- Asset Manager with automatic caching
- Mesh/Material system for complex models
- Vertex animation support for wind effects
- Cross-platform support (Windows, Linux, macOS)

## Building the Engine

### Prerequisites

#### Windows
- CMake 3.15 or higher
- Visual Studio 2019 or newer (with C++ desktop development)
- Git

#### Linux
```bash
sudo apt-get install cmake build-essential libgl1-mesa-dev libglu1-mesa-dev
```

#### macOS
```bash
brew install cmake
```

### Build Instructions

#### Windows (Visual Studio)

##### Prerequisites Verification

Before building, ensure you have the following installed:

1. **Git for Windows**
   - Download from: https://git-scm.com/download/win
   - Verify installation: Open Command Prompt and run `git --version`
   - You should see something like `git version 2.x.x`

2. **CMake 3.15 or higher**
   - Download from: https://cmake.org/download/ (Windows x64 Installer)
   - **Important**: During installation, select "Add CMake to system PATH for all users"
   - Verify installation: Run `cmake --version` in Command Prompt
   - You should see `cmake version 3.15.0` or higher

3. **Visual Studio 2019 or newer**
   - Download Community Edition (free) from: https://visualstudio.microsoft.com/downloads/
   - During installation, select the **"Desktop development with C++"** workload
   - This installs the C++ compiler (MSVC), Windows SDK, and build tools

##### Step 1: Clone the Repository

Open Command Prompt or PowerShell and run:

```bash
# Clone with --recursive flag to download all submodules (GLFW, GLM, Assimp)
git clone --recursive https://github.com/shifty81/GameEngine.git

# Navigate into the project directory
cd GameEngine
```

**Note**: The `--recursive` flag is essential as it downloads all required third-party libraries (GLFW, GLM, Assimp) from their Git submodules. Without it, the build will fail.

**If you already cloned without `--recursive`**, run:
```bash
git submodule update --init --recursive
```

##### Step 2: Create Build Directory

CMake uses out-of-source builds to keep generated files separate from source code:

```bash
# Create a build directory
mkdir build

# Navigate into it
cd build
```

**Why separate build directory?** This keeps all build artifacts (object files, executables, project files) separate from source code, making it easy to clean and rebuild.

##### Step 3: Generate Visual Studio Project Files

Run CMake to generate Visual Studio solution and project files:

```bash
# Generate Visual Studio project files
cmake ..
```

**What this does**:
- Analyzes `CMakeLists.txt` in the parent directory (`..`)
- Detects your Visual Studio version automatically
- Configures all dependencies (OpenGL, GLFW, GLM, Assimp)
- Creates `GameEngine.sln` (Visual Studio solution file)
- Creates `.vcxproj` files for each target

**Expected output**:
```
-- The C compiler identification is MSVC ...
-- The CXX compiler identification is MSVC ...
-- Configuring done
-- Generating done
-- Build files have been written to: .../GameEngine/build
```

**If CMake cannot find Visual Studio**, you can specify the generator explicitly:
```bash
# For Visual Studio 2022:
cmake -G "Visual Studio 17 2022" ..

# For Visual Studio 2019:
cmake -G "Visual Studio 16 2019" ..

# For Visual Studio 2017:
cmake -G "Visual Studio 15 2017" -A x64 ..
```

##### Step 4: Build the Project

You have two options:

**Option A: Build from Command Line** (Recommended for automation)

```bash
# Build Release configuration (optimized, fast)
cmake --build . --config Release

# This will:
# - Compile all .cpp files
# - Link libraries (OpenGL, GLFW, GLM, Assimp)
# - Create GameEngine.exe in build/bin/Release/
```

**Build time**: First build takes 5-15 minutes (compiles Assimp and GLFW). Subsequent builds are much faster.

For **Debug** builds (with debugging symbols, slower runtime):
```bash
cmake --build . --config Debug
```

**Option B: Build in Visual Studio IDE** (Better for development and debugging)

1. Open `GameEngine.sln` in the `build` directory
2. In Visual Studio, select **Release** configuration from the dropdown at the top (or Debug if you need debugging)
3. Right-click the **GameEngine** project in Solution Explorer → **Set as Startup Project**
4. Press **Ctrl+Shift+B** or go to **Build → Build Solution** (or press F7)
5. Wait for compilation to complete (status bar shows progress)

**Advantages of building in Visual Studio**:
- Better error visualization and IntelliSense
- Built-in debugger (F5 to run with debugging)
- Easy navigation through code
- Integrated profiling tools

##### Step 5: Run the Executable

After a successful build:

**From Command Line**:
```bash
# Run Release build
.\bin\Release\GameEngine.exe

# Or run Debug build
.\bin\Debug\GameEngine.exe
```

**From Visual Studio**:
- Press **F5** (run with debugger) or **Ctrl+F5** (run without debugger)

**From Windows Explorer**:
- Navigate to `GameEngine\build\bin\Release\`
- Double-click `GameEngine.exe`

The game engine window should open displaying a procedurally generated 3D world with terrain and water!

##### Quick Build (Alternative Method)

For convenience, use the provided batch script:

```bash
# From GameEngine root directory
build.bat
```

This script automatically:
1. Updates git submodules
2. Creates build directory
3. Runs CMake configuration
4. Builds Release configuration
5. Reports success/failure

To run the engine:
```bash
run.bat
```

##### Troubleshooting

**Problem**: "CMake is not recognized as an internal or external command"
- **Solution**: CMake is not in your system PATH. Reinstall CMake and select "Add to PATH" option, or add manually:
  - Default location: `C:\Program Files\CMake\bin`
  - Add to System Environment Variables → Path

**Problem**: "Could not find Visual Studio"
- **Solution**: Install Visual Studio with "Desktop development with C++" workload, or specify generator with `-G` flag

**Problem**: "LINK : fatal error LNK1104: cannot open file 'opengl32.lib'"
- **Solution**: Windows SDK is missing. Reinstall Visual Studio and ensure Windows SDK is selected in the installer

**Problem**: Build fails with "Cannot find GLFW" or "Cannot find GLM"
- **Solution**: Submodules were not initialized. Run:
  ```bash
  git submodule update --init --recursive
  ```
  Then delete `build` directory and start over from Step 2

**Problem**: "GameEngine.exe - System Error: The code execution cannot proceed because VCRUNTIME140.dll was not found"
- **Solution**: Install Microsoft Visual C++ Redistributable:
  - Download from: https://aka.ms/vs/17/release/vc_redist.x64.exe
  - Run installer and restart

**Problem**: Long build time (>20 minutes)
- **Expected**: First build compiles Assimp (~3000 files) and takes 10-15 minutes
- **Speed up**: Use `cmake --build . --config Release -j 8` to build with 8 parallel jobs
- **Alternative**: Subsequent rebuilds only take 10-30 seconds

**Problem**: Black screen or window doesn't open
- **Solution**: Your GPU may not support OpenGL 3.3+. Update graphics drivers:
  - NVIDIA: https://www.nvidia.com/Download/index.aspx
  - AMD: https://www.amd.com/en/support
  - Intel: https://www.intel.com/content/www/us/en/download-center/home.html

##### Clean Build

To start fresh (useful if you encounter strange errors):

```bash
# Delete build directory
cd ..
rmdir /s /q build

# Start over from Step 2
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

#### Windows (MinGW)
```bash
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
.\bin\GameEngine.exe
```

#### Linux/macOS
```bash
mkdir build
cd build
cmake ..
make -j4
./bin/GameEngine
```

## Controls

- **WASD** - Move camera forward/backward/left/right
- **Space** - Move camera up
- **Left Shift** - Move camera down
- **Mouse** - Look around
- **F** - Toggle wireframe mode
- **C** - Toggle cell shading on/off
- **ESC** - Exit application

## Procedural Generation

### Terrain Generation
The terrain uses multi-octave Perlin noise to create realistic landscapes with:
- Hills and valleys
- Variable height maps
- Smooth transitions
- Configurable scale and detail

### Cave Systems
Caves are generated using 3D Perlin noise with dual thresholds:
- Natural-looking underground caverns
- Multiple cave layers at different depths
- Procedurally connected systems

### Texture Generation
Procedural textures are generated at runtime and saved to the `textures/` directory:
- `grass.ppm` - Green grass texture
- `stone.ppm` - Gray stone texture
- `sand.ppm` - Sandy texture
- `water.ppm` - Blue water texture

**Note**: These textures can be replaced with custom textures in PPM format.

## Water System

The water system includes:
- Animated surface with wave effects
- Flow direction calculation
- Semi-transparent rendering
- Real-time updates

## Wind System

The wind system simulates:
- Dynamic wind direction and strength
- Realistic gusts using sine wave patterns
- Time-based variations
- Windmill rotation calculations (for future windmill objects)

## Project Structure

```
GameEngine/
├── src/
│   ├── main.cpp              # Main application entry point
│   ├── Camera.h              # Camera controller
│   ├── Shader.h              # Shader compilation and management
│   ├── Terrain.h             # Procedural terrain generation
│   ├── Water.h               # Water system
│   ├── Wind.h                # Wind simulation
│   ├── TextureGenerator.h    # Procedural texture generation
│   ├── Model.h               # 3D model loading and rendering
│   ├── TextureLoader.h       # Texture loading with stb_image
│   └── AssetManager.h        # Asset management system
├── assets/
│   ├── models/               # 3D model files (.obj, .fbx, .gltf)
│   └── textures/             # Texture files (.png, .jpg, .tga)
├── external/
│   ├── glfw/                 # Window and input handling
│   ├── glm/                  # Mathematics library
│   ├── glad/                 # OpenGL loader
│   ├── assimp/               # 3D model loading library
│   └── stb/                  # Image loading library
├── CMakeLists.txt            # Build configuration
├── README.md                 # This file
└── ASSET_PIPELINE.md         # Asset integration guide
```

## Extending the Engine

### Loading 3D Models (Stylized Nature Kit)

```cpp
#include "AssetManager.h"

// Get asset manager instance
AssetManager* assetMgr = AssetManager::GetInstance();

// Load a tree model from your nature kit
auto treeModel = assetMgr->LoadModel("assets/models/nature/tree_01.fbx", "oak_tree");

// Load a rock model
auto rockModel = assetMgr->LoadModel("assets/models/nature/rock_01.obj", "granite_rock");

// Render in your game loop
shader.use();
shader.setMat4("model", transform);
treeModel->Draw(shader);
```

See **ASSET_PIPELINE.md** for complete integration guide.

### Adding New Features
The engine is designed to be easily extensible:

1. **New Objects**: Create classes similar to `Terrain` or `Water`
2. **Custom Shaders**: Add new shader pairs in `main.cpp`
3. **New Systems**: Follow the pattern of `Wind.h` for new systems
4. **Custom Textures**: Replace files in `textures/` directory or use Asset Manager
5. **3D Models**: Place in `assets/` and load with Asset Manager

### Windmill Implementation (Future)
The wind system is already prepared for windmills:
```cpp
float rotation = wind.getWindmillRotation();
// Apply rotation to windmill model
```

## Performance

- Optimized mesh generation
- Efficient OpenGL buffer usage
- Configurable terrain resolution
- Level of detail system ready for implementation

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Credits

- **OpenGL** - Graphics API
- **GLFW** - Window and input management
- **GLM** - Mathematics library
- **GLAD** - OpenGL loader
- **Assimp** - 3D model loading library
- **stb_image** - Image loading library

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.
