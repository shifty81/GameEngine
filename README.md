# 3D Game Engine

A professional-grade 3D game engine built with C++ and OpenGL, featuring procedural world generation, advanced rendering systems, and comprehensive game development tools.

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

### 🎯 Platform Integration
- 🎮 **itch.io Integration** - Seamless user authentication, purchase verification, and profile access
- 🔑 **OAuth Support** - User authentication for standalone and web builds
- ☁️ **Cloud Save Ready** - User ID-based save system for cross-device sync
- 🏆 **Achievement Framework** - Backend-ready achievement tracking system

### ⚠️ Integration-Ready Systems
- ⚛️ **Physics Framework** - AABB collision, rigid bodies (integrate Bullet/PhysX)
- 🔊 **Audio Framework** - 3D spatial audio system (integrate OpenAL/FMOD)
- 📜 **Scripting-Ready** - Architecture supports Lua/ChaiScript integration

### Technical Capabilities
- Modern OpenGL 3.3+ rendering pipeline
- PBR (Physically Based Rendering) materials
- HDR rendering with tone mapping
- Multi-light support (unlimited lights)
- Real-time shader compilation
- Cross-platform support (Windows, Linux, macOS)
- Assetimport for 10+ 3D formats
- Event-driven architecture
- Component-based entity system

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

**Problem**: "No CMAKE_C_COMPILER could be found" or "No CMAKE_CXX_COMPILER could be found"
- **Cause**: Visual Studio is installed but the C++ compiler (MSVC) is not installed
- **Solution**: 
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
│   ├── ItchIO.h                  # ✨ NEW - itch.io platform integration
│   ├── ItchIOExample.cpp         # ✨ NEW - itch.io usage examples
│   ├── InputManager.h            # ✨ NEW - Keyboard/mouse/gamepad input
│   ├── EventSystem.h             # ✨ NEW - Event bus system
│   ├── LightSystem.h             # ✨ NEW - Advanced lighting (directional/point/spot)
│   ├── MaterialSystem.h          # ✨ NEW - PBR material system
│   ├── PostProcessing.h          # ✨ NEW - Post-processing effects (FXAA, tone mapping)
│   ├── PhysicsSystem.h           # ✨ NEW - Physics framework (collision, rigid bodies)
│   ├── AudioSystem.h             # ✨ NEW - 3D audio framework
│   ├── UISystem.h                # ✨ NEW - In-game UI system
│   ├── SceneGraph.h              # ✨ NEW - Scene/Entity management
│   └── DebugTools.h              # ✨ NEW - Profiling and debug rendering
├── assets/
│   ├── models/                   # 3D model files (.obj, .fbx, .gltf)
│   └── textures/                 # Texture files (.png, .jpg, .tga)
├── external/
│   ├── glfw/                     # Window and input handling
│   ├── glm/                      # Mathematics library
│   ├── glad/                     # OpenGL loader
│   ├── assimp/                   # 3D model loading library
│   └── stb/                      # Image loading library
├── CMakeLists.txt                # Build configuration
├── README.md                     # This file
├── ENGINE_SYSTEMS.md             # ✨ NEW - Complete systems documentation
├── ASSET_PIPELINE.md             # Asset integration guide
├── ITCHIO_INTEGRATION.md         # ✨ NEW - itch.io platform integration guide
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
- **[ENGINE_SYSTEMS.md](ENGINE_SYSTEMS.md)** - Complete engine systems documentation
- **[ITCHIO_INTEGRATION.md](ITCHIO_INTEGRATION.md)** - itch.io platform integration guide
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

### Integrating with itch.io

Seamlessly integrate your game with the itch.io platform:

```cpp
#include "ItchIO.h"

int main() {
    // Initialize itch.io integration
    ItchIO itchio;
    
    if (itchio.IsAvailable() && itchio.Initialize()) {
        // Get user profile
        auto profile = itchio.GetUserProfile();
        std::cout << "Welcome, " << profile->username << "!" << std::endl;
        
        // Verify purchase
        if (itchio.VerifyPurchase("your-game-id")) {
            enablePremiumFeatures();
        }
        
        // Use profile->id for cloud saves
        loadCloudSave(profile->id);
    }
    
    // Your game code...
}
```

See **ITCHIO_INTEGRATION.md** for complete guide including:
- User authentication
- Purchase verification
- Cloud save integration
- Achievement systems
- Publishing to itch.io

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
