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
```bash
# Clone the repository with submodules
git clone --recursive https://github.com/shifty81/GameEngine.git
cd GameEngine

# Create build directory
mkdir build
cd build

# Generate Visual Studio project
cmake ..

# Open the .sln file in Visual Studio and build
# Or build from command line:
cmake --build . --config Release

# Run the executable
.\bin\Release\GameEngine.exe
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
