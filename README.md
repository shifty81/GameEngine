# 3D Game Engine

A professional-grade 3D game engine built with C++ and OpenGL, featuring procedural world generation, advanced rendering systems, and comprehensive game development tools.

## 🚀 Quick Start Options

### Option 1: Download Pre-built Binaries (No Compilation Required!)

**Just want to run the engine?** Download ready-to-use executables:

👉 **[Download Pre-built Binaries](DOWNLOAD_BINARIES.md)** - Windows & Linux available

- ✅ No Visual Studio or compiler needed
- ✅ No build process required
- ✅ Just download, extract, and run!
- ✅ Perfect for trying out the engine

### Option 2: Build from Source (For Developers)

Want to modify the code or contribute? See [Building the Engine](#building-the-engine) section below.

---

## 🎮 Complete Engine Systems

This engine now includes **all essential game engine systems** for production-ready game development!

### ✅ Graphics and Rendering
- ✨ **Cell Shading** - Toon/cartoon-style rendering with customizable shading levels
- 🎨 **Advanced Lighting System** - Directional, point, and spot lights with full PBR support
- 🖼️ **Material System** - PBR materials with albedo, metallic, roughness, and texture maps
- 🌟 **Post-Processing** - FXAA anti-aliasing, tone mapping (Reinhard/ACES), bloom framework
- 🎬 **Camera System** - Full 3D free-flight with mouse look and WASD controls
- 📐 **Shader Management** - Complete GLSL shader compilation and uniform management
- 📦 **Model Loading** - Assimp integration for .obj, .fbx, .gltf, and more
- 🖼️ **Texture Management** - stb_image support for PNG, JPG, TGA, etc.

### 🌍 World Generation
- 🏔️ **Procedural Terrain** - Multi-octave Perlin noise for realistic landscapes
- ⛰️ **Cave Systems** - Underground cavern generation for exploration
- 💧 **Water System** - Animated flowing water with transparency
- 🌬️ **Wind System** - Dynamic wind simulation for environmental effects
- 🎨 **Procedural Textures** - Runtime texture generation system

### 🎮 Input & Events
- ⌨️ **Input Manager** - Unified keyboard, mouse, and gamepad input handling
- 📡 **Event System** - Type-safe event bus for decoupled system communication
- 🎯 **Action Mapping** - Map game actions to input keys
- 🎮 **Gamepad Support** - Full gamepad/controller support (up to 16 devices)

### 🏗️ Core Engine
- 🗂️ **Scene Graph / ECS** - Hierarchical entity system with transform hierarchy
- 📂 **Resource Management** - Automatic asset caching and memory management
- ⏱️ **Game Loop** - Fixed timestep with delta time for frame-independent movement
- 🧮 **Math Library** - Complete GLM integration for vectors, matrices, quaternions

### 🎨 User Interface
- 🖼️ **UI System** - In-game UI with panels, buttons, and labels
- 🖱️ **Mouse Interaction** - Click and hover detection
- 📐 **Screen-space Rendering** - Proper UI rendering pipeline

### 🔧 Development Tools
- 🐛 **Debug Renderer** - Draw debug lines, boxes, and spheres
- 📊 **Performance Profiler** - FPS counter, frame timing, section profiling
- 🧠 **Memory Tracker** - GPU memory usage tracking
- 🔍 **OpenGL Debugger** - Automatic OpenGL error detection and logging
- ⏱️ **High-Precision Timers** - Performance measurement tools
- 🛠️ **Visual Studio Compiler Detector** - Automated detection and fix for VC++ 2022 compiler issues
  - Comprehensive VS 2022 installation validation
  - C++ workload component verification
  - Auto-fix option to launch Visual Studio Installer
  - Detailed error messages with actionable solutions
- 🎮 **Debug Console** - Runtime command console with enable/disable toggle (press ` key)
  - Command history with up/down arrow navigation
  - Variable get/set system
  - Extensible command registration
  - Built-in commands (help, clear, set, get, echo)
  - Game control commands (wireframe, cellshading, camera control)

### 🎯 Asset Integration
- 🎨 **itch.io Asset Support** - Easy integration of downloaded assets from itch.io marketplace
- 📦 **Multiple Format Support** - FBX, OBJ, GLTF, and more from any asset store
- 🗂️ **Smart Asset Organization** - Automatic caching and management
- 🔄 **Quick Drop-in** - Simple 3-step process to use marketplace assets

### ⚠️ Integration-Ready Systems
- ⚛️ **Physics Framework** - AABB collision, rigid bodies (integrate Bullet/PhysX)
- 🔊 **Audio Framework** - 3D spatial audio system (integrate OpenAL/FMOD)
- 📜 **Scripting-Ready** - Architecture supports Lua/ChaiScript integration

### Technical Capabilities
- Modern OpenGL 3.3+ rendering pipeline
- C++20 standard with modern features
- PBR (Physically Based Rendering) materials
- HDR rendering with tone mapping
- Multi-light support (unlimited lights)
- Real-time shader compilation
- Cross-platform support (Windows, Linux, macOS)
- Assimp 6.0.2 for 10+ 3D formats
- GLFW 3.4 for window management
- GLM 0.9.9.8 for mathematics
- Event-driven architecture
- Component-based entity system
- Runtime debug console system

## Building the Engine

### Prerequisites

#### Windows
- CMake 3.20 or higher
- **Visual Studio Community 2022 (version 17.8 or newer)** or Visual Studio 2019 (version 16.11 or newer)
  - **Specific versions recommended:** Visual Studio 2022 v17.11+ (preferred) or Visual Studio 2019 v16.11+
  - Must include **"Desktop development with C++"** workload for C++20 support
  - See [VISUAL_STUDIO_SETUP.md](VISUAL_STUDIO_SETUP.md) for detailed installation guide with visual instructions
- Git

#### Linux
```bash
sudo apt-get install cmake build-essential libgl1-mesa-dev libglu1-mesa-dev \
    libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
```
Note: Requires GCC 10+ or Clang 11+ for C++20 support

**Automated Setup (Recommended):**
```bash
./setup.sh
```
This script will automatically check and install all required dependencies.

#### macOS
```bash
# Install Xcode Command Line Tools (includes clang++)
xcode-select --install

# Install CMake
brew install cmake
```
Note: Requires Xcode 12+ for C++20 support

**Automated Setup (Recommended):**
```bash
./setup.sh
```
This script will automatically check and install all required dependencies.

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

3. **Visual Studio 2022 (version 17.8+) or Visual Studio 2019 (version 16.11+)**
   - **Recommended:** Visual Studio Community 2022 version 17.8 or newer (Latest: 17.11+)
   - **Alternative:** Visual Studio Community 2019 version 16.11 or newer
   - Download Community Edition (free) from: https://visualstudio.microsoft.com/downloads/
   - During installation, select the **"Desktop development with C++"** workload
   - This installs the C++ compiler (MSVC v143/v142), Windows SDK, and build tools
   - **📖 See [VISUAL_STUDIO_SETUP.md](VISUAL_STUDIO_SETUP.md) for step-by-step installation guide with visual diagrams**

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

##### Automated Setup (Recommended for First-Time Users)

For the easiest setup experience, use the automated setup script:

```bash
# From GameEngine root directory
setup.bat
```

This script automatically:
1. Checks for Git installation
2. Checks for CMake (offers to download portable version if missing)
3. **Detects Visual Studio 2022 and C++ compiler installation**
4. **Offers to automatically fix compiler issues**
5. Initializes git submodules
6. Provides clear next steps

**Features**:
- Automatic compiler detection using advanced diagnostics
- One-click fix for missing C++ components
- Downloads portable CMake if needed (no PATH configuration required)
- Comprehensive error messages with actionable solutions

After setup completes, simply run:
```bash
build.bat
```

##### Quick Build (Alternative Method)

For convenience, use the provided batch script:

```bash
# From GameEngine root directory
build.bat
```

This script automatically:
1. **Checks for Visual Studio C++ compiler issues (with optional auto-fix)**
2. Updates git submodules
3. Creates build directory
4. Runs CMake configuration
5. Builds Release configuration
6. Reports success/failure

To run the engine:
```bash
run.bat
```

##### Troubleshooting

**Problem**: "No CMAKE_CXX_COMPILER could be found" or "The CXX compiler identification is unknown"
- **Cause**: Visual Studio is installed but the C++ compiler (MSVC) is not installed
- **Quick Fix**: Run the automated compiler detection tool:
  ```batch
  tools\check-vs2022-compiler.bat
  ```
  This will diagnose the issue and provide specific fix instructions. Use the `-autofix` flag to automatically open Visual Studio Installer:
  ```batch
  tools\check-vs2022-compiler.bat -autofix
  ```
- **Manual Solution**: 
  1. Open **Visual Studio Installer** (search in Start menu)
  2. Click **"Modify"** on your Visual Studio installation
  3. Select the **"Desktop development with C++"** workload (checkbox on the left)
  4. Click **"Modify"** button at bottom right to install
  5. Wait for installation to complete (may take 10-15 minutes)
  6. Delete the `build` folder in your GameEngine directory
  7. Run `build.bat` again
- **Alternative**: If you don't have Visual Studio, download Community Edition (free):
  - https://visualstudio.microsoft.com/downloads/
  - During installation, select **"Desktop development with C++"** workload
- **Note**: This error means CMake found Visual Studio but the actual C++ compiler tools aren't installed. The "Desktop development with C++" workload includes cl.exe (C++ compiler), MSBuild, and Windows SDK.
- **See Also**: For comprehensive troubleshooting, see [WINDOWS_TROUBLESHOOTING.md](WINDOWS_TROUBLESHOOTING.md)

**Problem**: "CMake is not recognized as an internal or external command"
- **Cause**: CMake is not in your system PATH, or is installed in a custom location
- **Solution Option 1 - Reinstall CMake with PATH**:
  1. Reinstall CMake from https://cmake.org/download/
  2. During installation, select **"Add CMake to the system PATH for all users"** or **"Add CMake to the system PATH for the current user"**
  3. Restart Command Prompt and try again
- **Solution Option 2 - Manually Add CMake to PATH** (for custom installations like `C:\make\`):
  1. Press **Windows + R**, type `sysdm.cpl`, press Enter
  2. Click **"Environment Variables"** button (bottom right)
  3. Under **"System variables"** (or "User variables"), find and select **"Path"**
  4. Click **"Edit"**
  5. Click **"New"** and add your CMake bin directory:
     - If CMake is at `C:\make\`, add: `C:\make\bin`
     - If CMake is at `C:\Program Files\CMake\`, add: `C:\Program Files\CMake\bin`
     - Make sure to add the `bin` subdirectory where `cmake.exe` is located
  6. Click **"OK"** on all dialogs
  7. **Restart your Command Prompt or PowerShell** (environment changes require a new session)
  8. Verify: Run `cmake --version` - you should see your CMake version (e.g., 4.2.0 or 3.20+)
- **Solution Option 3 - Use Full Path**:
  - Instead of running `cmake ..`, use the full path: `C:\make\bin\cmake.exe ..`
- **Note**: The PATH variable tells Windows where to find executable files. Without it, you must use the full path to cmake.exe every time

**Problem**: "Could not find Visual Studio" or Visual Studio compiler detection issues
- **Quick Fix**: Run the compiler detection diagnostic tool:
  ```batch
  tools\check-vs2022-compiler.bat
  ```
  This comprehensive tool will:
  - Detect Visual Studio 2022 installation
  - Verify C++ compiler (MSVC) is installed
  - Check for required components (C++ build tools, Windows SDK)
  - Provide specific fix instructions with component names
  - Optionally auto-launch Visual Studio Installer to fix issues
- **Manual Solution**: Install Visual Studio with "Desktop development with C++" workload, or specify generator with `-G` flag

**For more details on the compiler detection tool**, see [tools/README.md](tools/README.md)

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

**Quick Build (Recommended):**
```bash
# Automated setup (first time only)
./setup.sh

# Build the project
./build.sh

# Run the engine
./run.sh
```

**Manual Build:**
```bash
# Create and enter build directory
mkdir build
cd build

# Generate makefiles
cmake ..

# Build (use all CPU cores for faster compilation)
make -j$(nproc)  # Linux
make -j$(sysctl -n hw.ncpu)  # macOS

# Run
./bin/GameEngine
```

**Troubleshooting:**
If you encounter compiler errors, run the diagnostic tool:
```bash
./tools/check-compiler.sh
```

Or use autofix to automatically install missing dependencies:
```bash
./tools/check-compiler.sh -autofix
```

See [TROUBLESHOOTING.md](TROUBLESHOOTING.md) for comprehensive troubleshooting guidance.

## Controls

- **WASD** - Move camera forward/backward/left/right
- **Space** - Move camera up
- **Left Shift** - Move camera down
- **Mouse** - Look around
- **F** - Toggle wireframe mode
- **C** - Toggle cell shading on/off
- **` (Grave/Tilde)** - Toggle debug console
- **ESC** - Exit application

### Debug Console Commands
When the debug console is open (press ` key):
- **help** - List all available commands
- **clear** - Clear console output
- **wireframe** - Toggle wireframe rendering
- **cellshading [on|off]** - Toggle or set cell shading
- **setcam <x> <y> <z>** - Set camera position
- **getcam** - Display current camera position
- **set <name> <value>** - Set a console variable
- **get <name>** - Get a console variable value
- **listvars** - List all console variables
- **echo <text>** - Print text to console
- **exit/quit** - Close console
- **Up/Down Arrow** - Navigate command history

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
│   ├── main.cpp                  # Main application entry point
│   ├── Camera.h                  # Camera controller
│   ├── Shader.h                  # Shader compilation and management
│   ├── Terrain.h                 # Procedural terrain generation
│   ├── Water.h                   # Water system
│   ├── Wind.h                    # Wind simulation
│   ├── TextureGenerator.h        # Procedural texture generation
│   ├── Model.h                   # 3D model loading and rendering
│   ├── TextureLoader.h           # Texture loading with stb_image
│   ├── AssetManager.h            # Asset management system
│   ├── InputManager.h            # ✨ NEW - Keyboard/mouse/gamepad input
│   ├── EventSystem.h             # ✨ NEW - Event bus system
│   ├── LightSystem.h             # ✨ NEW - Advanced lighting (directional/point/spot)
│   ├── MaterialSystem.h          # ✨ NEW - PBR material system
│   ├── PostProcessing.h          # ✨ NEW - Post-processing effects (FXAA, tone mapping)
│   ├── PhysicsSystem.h           # ✨ NEW - Physics framework (collision, rigid bodies)
│   ├── AudioSystem.h             # ✨ NEW - 3D audio framework
│   ├── UISystem.h                # ✨ NEW - In-game UI system
│   ├── SceneGraph.h              # ✨ NEW - Scene/Entity management
│   ├── DebugTools.h              # ✨ NEW - Profiling and debug rendering
│   └── DebugConsole.h            # ✨ NEW - Runtime debug console system
├── assets/
│   ├── models/                   # 3D model files (.obj, .fbx, .gltf)
│   └── textures/                 # Texture files (.png, .jpg, .tga)
├── external/
│   ├── glfw/                     # Window and input handling (v3.4)
│   ├── glm/                      # Mathematics library (v0.9.9.8)
│   ├── glad/                     # OpenGL loader (3.3 Core)
│   ├── assimp/                   # 3D model loading library (v6.0.2)
│   └── stb/                      # Image loading library
├── CMakeLists.txt                # Build configuration
├── README.md                     # This file
├── ENGINE_SYSTEMS.md             # ✨ NEW - Complete systems documentation
├── VERSION_UPDATES.md            # ✨ NEW - Architecture updates and modernization
├── ASSET_PIPELINE.md             # Asset integration guide
├── ITCHIO_ASSETS.md              # ✨ NEW - itch.io asset integration guide
└── QUICKSTART.md                 # Quick start guide
```

## 📚 Complete Documentation

### Engine Systems Documentation
See **[ENGINE_SYSTEMS.md](ENGINE_SYSTEMS.md)** for comprehensive documentation on all engine systems:
- Detailed API reference for each system
- Usage examples and code snippets
- Integration guides
- Best practices
- Complete game object implementation examples

### Quick References
- **[README.md](README.md)** - This file - Overview and build instructions
- **[DOWNLOAD_BINARIES.md](DOWNLOAD_BINARIES.md)** - 📦 Download pre-built executables (no compilation needed!)
- **[VISUAL_STUDIO_SETUP.md](VISUAL_STUDIO_SETUP.md)** - 📖 Detailed Visual Studio installation guide with specific versions and visual instructions
- **[ENGINE_SYSTEMS.md](ENGINE_SYSTEMS.md)** - Complete engine systems documentation
- **[DEBUG_CONSOLE_GUIDE.md](DEBUG_CONSOLE_GUIDE.md)** - Debug console usage and command reference
- **[TROUBLESHOOTING.md](TROUBLESHOOTING.md)** - Comprehensive cross-platform troubleshooting guide
- **[WINDOWS_TROUBLESHOOTING.md](WINDOWS_TROUBLESHOOTING.md)** - Windows-specific troubleshooting
- **[VERSION_UPDATES.md](VERSION_UPDATES.md)** - Architecture modernization and dependency updates
- **[ITCHIO_ASSETS.md](ITCHIO_ASSETS.md)** - Easy itch.io asset integration guide
- **[QUICKSTART.md](QUICKSTART.md)** - Quick start guide for beginners
- **[ASSET_PIPELINE.md](ASSET_PIPELINE.md)** - 3D asset integration guide
- **[FEATURES.md](FEATURES.md)** - Detailed feature list

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

### Integrating itch.io Assets

Easily drop in assets downloaded from itch.io:

**Step 1:** Download asset pack from [itch.io](https://itch.io/game-assets)

**Step 2:** Extract and copy to your project:
```bash
# Copy models to assets folder
cp downloaded_pack/models/* GameEngine/assets/models/

# Copy textures
cp downloaded_pack/textures/* GameEngine/assets/textures/
```

**Step 3:** Load and use in your game:
```cpp
// Load the itch.io assets
auto model = AssetManager::GetInstance()->LoadModel(
    "assets/models/tree.fbx", 
    "tree"
);

// Render in game loop
shader.use();
shader.setMat4("model", transform);
model->Draw(shader);
```

See **ITCHIO_ASSETS.md** for complete guide including:
- Organizing large asset collections
- Fixing coordinate system mismatches
- Handling scale issues
- Performance optimization
- Recommended asset packs

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
