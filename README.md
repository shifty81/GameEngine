# Game Engine - Hybrid C++20/.NET 9 Voxel Engine

A professional-grade voxel game engine combining C++20 (performance core) with .NET 9 (editor and high-level systems).

> **🎯 Windows Build**: This project has been migrated to a Windows-focused hybrid architecture with C++20 and .NET 9.  
> **📖 See [README_WINDOWS.md](README_WINDOWS.md)** for Windows build instructions  
> **🚀 See [QUICKSTART.md](QUICKSTART.md)** for quick start guide  
> **🗺️ See [ROADMAP.md](ROADMAP.md)** for comprehensive feature roadmap

---

## Platform Status

### ✅ Windows (Primary - Current)
- **Architecture**: C++20 (Core) + .NET 9 (Editor)
- **Graphics**: DirectX 11
- **Build System**: MSBuild / Visual Studio 2022
- **Status**: ✅ **FULLY FUNCTIONAL** - See [README_WINDOWS.md](README_WINDOWS.md)

### ⚠️ Linux (Legacy - Ubuntu 24.04)
- **Architecture**: C++ + OpenGL
- **Build System**: CMake + GCC
- **Status**: ⚠️ Legacy version (below)
- **Note**: Windows version is the active development branch

---

## Quick Start (Windows)

### Prerequisites
- Windows 10/11 (64-bit)
- Visual Studio 2022 with C++ and .NET workloads
- .NET 9 SDK
- DirectX 11 capable GPU

### Build & Run
```powershell
# Clone
git clone --recursive https://github.com/shifty81/GameEngine.git
cd GameEngine

# Build
.\build.ps1

# Run
.\run.ps1
```

Or open `GameEngine.sln` in Visual Studio 2022 and press F5.

**See [QUICKSTART.md](QUICKSTART.md) for detailed instructions.**

---

## Architecture

### Hybrid Design
- **C++20 Core**: Voxel engine, rendering, physics (performance-critical)
- **.NET 9 Editor**: WPF UI, editor tools, game logic (rapid development)

### Key Features
- ✨ Voxel-based terrain with chunking
- 🎨 DirectX 11 rendering
- 🖼️ Dockable editor UI (AvalonDock)
- 📐 Real-time editor mode
- 🎬 Free-flight camera
- 📊 Console for commands
- 🔧 C++/C# interop layer

**See [ARCHITECTURE.md](ARCHITECTURE.md) for detailed architecture overview.**

---

## Documentation

- **[README_WINDOWS.md](README_WINDOWS.md)** - Windows build guide & features
- **[QUICKSTART.md](QUICKSTART.md)** - Fast setup and troubleshooting
- **[ROADMAP.md](ROADMAP.md)** - Comprehensive 14-phase feature plan
- **[ARCHITECTURE.md](ARCHITECTURE.md)** - System architecture & design
- **[UBUNTU_24_04.md](UBUNTU_24_04.md)** - Legacy Linux documentation

---

## Editor Features

### Dockable Panels
- **Viewport**: 3D rendering view with DirectX
- **Hierarchy**: Scene object tree
- **Properties**: Object inspector and editor
- **Console**: Command interface and logs

### Controls
- `W/A/S/D` - Move camera
- `Space/Shift` - Up/Down
- `F1` - Help
- `Ctrl+N/O/S` - New/Open/Save scene

### Console Commands
```
help                    - Show all commands
terrain <seed>          - Generate new terrain  
setcam <x> <y> <z>     - Set camera position
editor                  - Toggle editor mode
```

---

## Development Roadmap

### ✅ Phase 1: Foundation (Complete)
- [x] Visual Studio 2022 solution
- [x] C++20 core with DirectX 11
- [x] .NET 9 WPF editor
- [x] C++/C# interop
- [x] Basic voxel system
- [x] Dockable UI

### 🚧 Phase 2: Voxel Features (Next)
- [ ] LOD system
- [ ] Texture atlas
- [ ] Frustum/occlusion culling
- [ ] Multi-threaded generation
- [ ] Save/load

### 📋 Future Phases (Planned)
- L-Systems for vegetation
- Cellular Automata for caves
- Inventory & crafting
- Physics engine
- Multiplayer
- Modding support
- VR support

**See [ROADMAP.md](ROADMAP.md) for complete 14-phase roadmap.**

---

# Legacy: Ubuntu 24.04 LTS Build

> **⚠️ Note**: The Ubuntu build below is the legacy version. Active development has moved to the Windows hybrid architecture above.

## System Requirements

- **Ubuntu 24.04 LTS (Noble Numbat)**
- **GCC 13.3+** with C++20 support
- **CMake 3.28+**
- **OpenGL 3.3+** capable GPU

## Quick Start

### One-Command Setup and Build

The following command will install dependencies, clone the repository, build, and run the engine.
**Note:** If any step fails, run the individual commands below for better error feedback.

```bash
sudo apt-get update && sudo apt-get install -y build-essential cmake git \
    libgl1-mesa-dev libglu1-mesa-dev \
    libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev && \
git clone --recursive https://github.com/shifty81/GameEngine.git && \
cd GameEngine && ./build.sh && ./run.sh
```

### Step-by-Step Setup

**1. Install Dependencies**
```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake git \
    libgl1-mesa-dev libglu1-mesa-dev \
    libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
```

**2. Clone Repository**
```bash
git clone --recursive https://github.com/shifty81/GameEngine.git
cd GameEngine
```

**3. Build**
```bash
./build.sh
```

**4. Run**
```bash
./run.sh
```

**For Headless/Server Environments:**
```bash
# Install Xvfb for virtual display
sudo apt-get install xvfb

# Run with the headless script
./run-headless.sh
```

## Engine Features

### Graphics and Rendering
- ✨ Cell Shading - Toon/cartoon-style rendering
- 🎨 Advanced Lighting System - Directional, point, and spot lights with PBR
- 🖼️ Material System - PBR materials with full texture support
- 🌟 Post-Processing - FXAA, tone mapping, bloom
- 🎬 Camera System - Free-flight 3D camera with mouse look
- 📐 Shader Management - GLSL shader compilation
- 📦 Model Loading - Assimp support (.obj, .fbx, .gltf)
- 🖼️ Texture Management - PNG, JPG, TGA support

### World Generation
- 🏔️ Procedural Terrain - Multi-octave Perlin noise
- ⛰️ Cave Systems - Underground cavern generation
- 💧 Water System - Animated flowing water
- 🌬️ Wind System - Dynamic wind simulation
- 🎨 Procedural Textures - Runtime texture generation

### Input & Events
- ⌨️ Input Manager - Keyboard, mouse, and gamepad handling
- 📡 Event System - Type-safe event bus
- 🎯 Action Mapping - Configurable input bindings
- 🎮 Gamepad Support - Up to 16 controllers

### Core Systems
- 🗂️ Scene Graph / ECS - Hierarchical entity system
- 📂 Resource Management - Asset caching and memory management
- ⏱️ Game Loop - Fixed timestep with delta time
- 🧮 Math Library - GLM integration

### Development Tools
- 🐛 Debug Renderer - Debug visualization
- 📊 Performance Profiler - FPS and frame timing
- 🧠 Memory Tracker - GPU memory monitoring
- 🔍 OpenGL Debugger - Error detection and logging
- 🎮 Debug Console - Runtime command console (press ` key)
- ✨ Script Compiler - Runtime script execution system

## Controls

- **WASD** - Move camera
- **Space** - Move up
- **Left Shift** - Move down
- **Mouse** - Look around
- **F** - Toggle wireframe mode
- **C** - Toggle cell shading
- **`** (Grave/Tilde) - Toggle debug console
- **ESC** - Exit

## Debug Console Commands

- `help` - List all commands
- `clear` - Clear console
- `wireframe` - Toggle wireframe
- `cellshading [on|off]` - Toggle cell shading
- `setcam <x> <y> <z>` - Set camera position
- `getcam` - Display camera position
- `script <command>` - Execute script command
- `loadscript <filename>` - Load and execute script file

## Project Structure

```
GameEngine/
├── src/                      # Source code
│   ├── main.cpp             # Application entry
│   ├── Camera.h             # Camera system
│   ├── Shader.h             # Shader management
│   ├── Terrain.h            # Terrain generation
│   ├── Water.h              # Water system
│   ├── Wind.h               # Wind simulation
│   └── ...                  # Additional systems
├── assets/
│   ├── models/              # 3D models
│   └── textures/            # Texture files
├── scripts/                 # Script files
├── external/                # Third-party libraries
│   ├── glfw/               # Window management
│   ├── glm/                # Math library
│   ├── glad/               # OpenGL loader
│   ├── assimp/             # Model loading
│   └── stb/                # Image loading
├── build.sh                # Build script
├── run.sh                  # Run script
├── setup.sh                # Setup script
└── CMakeLists.txt          # Build configuration
```

## Development

### Building for Debug

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)
```

### Building for Release

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
```

### Recommended IDEs

- **Visual Studio Code** - Lightweight with good C++ support
- **CLion** - Full-featured C++ IDE
- **Qt Creator** - Excellent CMake integration

## Performance Optimization

### GPU Driver Recommendations

**NVIDIA GPUs:**
```bash
sudo ubuntu-drivers autoinstall
sudo reboot
```

**AMD GPUs:**
```bash
sudo add-apt-repository ppa:kisak/kisak-mesa
sudo apt-get update && sudo apt-get upgrade
```

**Intel GPUs:**
```bash
sudo apt-get install mesa-vulkan-drivers intel-media-va-driver
```

### Verify OpenGL Support

```bash
sudo apt-get install mesa-utils
glxinfo | grep "OpenGL version"
# Should show OpenGL 3.3 or higher
```

## Troubleshooting

### "Failed to create GLFW window" Error

This error occurs when the application cannot create an OpenGL window. Common solutions:

**1. No Display Available (Headless/Server/SSH):**
```bash
# Install and use Xvfb virtual framebuffer
sudo apt-get install xvfb
./run-headless.sh
```

**2. Missing Display Variable:**
```bash
# Check if DISPLAY is set
echo $DISPLAY

# If empty, set it (for X11 systems)
export DISPLAY=:0
```

**3. Graphics Driver Issues:**
```bash
# Check OpenGL version
glxinfo | grep "OpenGL version"

# Update NVIDIA drivers
sudo ubuntu-drivers autoinstall

# Update Mesa (AMD/Intel)
sudo apt-get update && sudo apt-get upgrade
```

**4. OpenGL Not Supported:**
- Ensure your GPU supports OpenGL 3.3 or higher
- Update graphics drivers
- For virtual machines, ensure 3D acceleration is enabled

For detailed troubleshooting, see [UBUNTU_24_04.md](UBUNTU_24_04.md).

## Documentation

See **[UBUNTU_24_04.md](UBUNTU_24_04.md)** for comprehensive Ubuntu 24.04 LTS development guide including:
- Detailed setup instructions
- IDE configuration
- Debugging tools
- Graphics optimization
- Performance tuning
- Troubleshooting

## Technical Details

- **Language**: C++20
- **Graphics API**: OpenGL 3.3+
- **Build System**: CMake 3.28+
- **Compiler**: GCC 13.3+
- **Window Management**: GLFW 3.4
- **Mathematics**: GLM 0.9.9.8
- **Model Loading**: Assimp 6.0.2
- **Image Loading**: stb_image

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Credits

- **OpenGL** - Graphics API
- **GLFW** - Window and input management
- **GLM** - Mathematics library
- **GLAD** - OpenGL loader
- **Assimp** - 3D model loading
- **stb_image** - Image loading

## Contributing

Contributions are welcome! Please submit pull requests or open issues for bugs and feature requests.
