# Game Engine - Windows .NET 9 + C++20

A hybrid voxel game engine combining C++20 (core performance) with .NET 9 (editor and high-level systems).

## Architecture

### **C++ Core Engine** (GameEngine.Core)
- DirectX 11 rendering
- Voxel chunk management
- Camera system
- Performance-critical systems

### **C# Editor** (.NET 9 WPF)
- Dockable UI with AvalonDock
- Real-time editor mode
- Property inspector
- Scene hierarchy
- Console for commands

## System Requirements

### Required
- **Windows 10/11** (64-bit)
- **Visual Studio 2022** (v17.8 or later)
- **.NET 9.0 SDK**
- **Windows SDK 10.0** or later
- **DirectX 11** capable GPU

### Optional
- **Visual Studio Workloads**:
  - Desktop development with C++
  - .NET desktop development

## Quick Start

### 1. Install Prerequisites

**Install .NET 9 SDK:**
```powershell
winget install Microsoft.DotNet.SDK.9
```

**Install Visual Studio 2022:**
- Download from: https://visualstudio.microsoft.com/
- Select workloads:
  - "Desktop development with C++"
  - ".NET desktop development"

### 2. Clone Repository

```powershell
git clone --recursive https://github.com/shifty81/GameEngine.git
cd GameEngine
```

### 3. Build and Run

**Option A: Visual Studio (Recommended)**
1. Open `GameEngine.sln` in Visual Studio 2022
2. Set build configuration to `Debug` or `Release` and `x64`
3. Right-click solution → "Build Solution" (F7)
4. Set `GameEngine.Editor` as startup project
5. Press F5 to run

**Option B: Command Line**
```powershell
# Build C++ Core
msbuild src\GameEngine.Core\GameEngine.Core.vcxproj /p:Configuration=Release /p:Platform=x64

# Build C# Editor
dotnet build src\GameEngine.Editor\GameEngine.Editor.csproj -c Release

# Run
dotnet run --project src\GameEngine.Editor\GameEngine.Editor.csproj -c Release
```

**Option C: PowerShell Script**
```powershell
.\build.ps1
.\run.ps1
```

## Project Structure

```
GameEngine/
├── GameEngine.sln              # Visual Studio solution
├── src/
│   ├── GameEngine.Core/        # C++ DLL (core engine)
│   │   ├── EngineCore.cpp/.h   # Main API exports
│   │   ├── VoxelEngine.cpp/.h  # Voxel management
│   │   ├── VoxelChunk.cpp/.h   # Chunk system
│   │   ├── Renderer.cpp/.h     # DirectX 11 renderer
│   │   └── Camera.cpp/.h       # Camera system
│   └── GameEngine.Editor/      # C# WPF Editor
│       ├── MainWindow.xaml     # Main editor UI
│       ├── EngineInterop.cs    # C++/C# interop
│       └── App.xaml            # Application entry
├── bin/                        # Build output
└── README_WINDOWS.md          # This file
```

## Editor Features

### Dockable Panels
- **Viewport**: 3D rendering view
- **Hierarchy**: Scene object tree
- **Properties**: Object inspector
- **Console**: Command interface

### Editor Modes
- **Editor Mode**: Free camera, no physics
- **Play Mode**: Game simulation active

### Controls

**Camera Movement:**
- `W/A/S/D` - Move forward/left/back/right
- `Space` - Move up
- `Left Shift` - Move down
- `Mouse` - Look around (in play mode)

**Editor Commands:**
- `F1` - Help
- `Ctrl+N` - New scene
- `Ctrl+O` - Open scene
- `Ctrl+S` - Save scene

### Console Commands

```
help                    - Show all commands
clear                   - Clear console
terrain <seed>          - Generate new terrain
setcam <x> <y> <z>     - Set camera position
editor                  - Toggle editor mode
```

## Development

### Building Debug Version

```powershell
msbuild GameEngine.sln /p:Configuration=Debug /p:Platform=x64
```

### Building Release Version

```powershell
msbuild GameEngine.sln /p:Configuration=Release /p:Platform=x64
```

### Hot Reload
The editor supports hot reload for C# code changes:
1. Make changes to C# files
2. Save
3. Visual Studio automatically rebuilds and reloads

### Debugging

**C++ Debugging:**
1. Set breakpoints in C++ code
2. Attach Visual Studio debugger to running process
3. Or set GameEngine.Core as startup project

**C# Debugging:**
1. Set breakpoints in C# code
2. Press F5 in Visual Studio
3. Debugger will hit breakpoints automatically

## API Reference

### C++ Engine Core API

```cpp
// Initialize engine with window handle
bool InitializeEngine(void* hwnd, int width, int height);

// Shutdown engine
void ShutdownEngine();

// Update and render
void UpdateEngine(float deltaTime);
void RenderEngine();

// Camera controls
void SetCameraPosition(float x, float y, float z);
void MoveCameraForward(float distance);

// Voxel operations
void SetVoxel(int x, int y, int z, uint8_t blockType);
uint8_t GetVoxel(int x, int y, int z);

// Terrain generation
void GenerateTerrain(int seed);

// Editor mode
void SetEditorMode(bool enabled);
```

### C# Interop Example

```csharp
using GameEngine.Editor;

// Initialize engine
EngineInterop.InitializeEngine(hwnd, 1280, 720);

// Generate terrain
EngineInterop.GenerateTerrain(12345);

// Set camera position
EngineInterop.SetCameraPosition(50, 30, 50);

// Update loop
EngineInterop.UpdateEngine(deltaTime);
EngineInterop.RenderEngine();
```

## Roadmap

### Phase 1: Core Foundation ✅
- [x] Visual Studio solution structure
- [x] C++20 core engine with DirectX 11
- [x] .NET 9 WPF editor
- [x] C++/C# interop layer
- [x] Basic voxel rendering
- [x] Dockable UI with AvalonDock

### Phase 2: Voxel Features (In Progress)
- [ ] Advanced chunk LOD system
- [ ] Texture atlas for blocks
- [ ] Frustum and occlusion culling
- [ ] Multi-threaded chunk generation
- [ ] Save/load world data

### Phase 3: Procedural Generation
- [ ] L-Systems for vegetation
- [ ] Cellular Automata for caves
- [ ] Biome system
- [ ] Noise-based terrain

### Phase 4: Gameplay Systems
- [ ] Block placement/destruction
- [ ] Physics engine
- [ ] Inventory and crafting
- [ ] Entity system

### Phase 5: Advanced Features
- [ ] Multiplayer networking
- [ ] Mod support via C# scripting
- [ ] Dynamic lighting and shadows
- [ ] Particle effects
- [ ] Weather system
- [ ] Day/night cycle

### Phase 6: Polish
- [ ] Performance profiling
- [ ] Memory optimization
- [ ] Advanced editor tools
- [ ] Asset pipeline
- [ ] Documentation

## Troubleshooting

### "Failed to initialize engine"
- Ensure DirectX 11 is installed
- Update GPU drivers
- Check Windows SDK installation

### "DLL not found" error
- Rebuild GameEngine.Core project
- Check that DLL is in same directory as .exe
- Verify bin/Debug or bin/Release folder structure

### Build Errors
- Ensure .NET 9 SDK is installed: `dotnet --version`
- Ensure Visual Studio 2022 v17.8+
- Clean solution and rebuild: `msbuild /t:Clean,Build`

### Performance Issues
- Switch to Release build
- Update GPU drivers
- Reduce chunk render distance
- Enable frustum culling

## Contributing

Contributions welcome! Please:
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## License

MIT License - See LICENSE file for details

## Credits

- **DirectX 11** - Microsoft
- **AvalonDock** - Dirkster99
- **.NET 9** - Microsoft
- **C++20** - ISO C++ Committee

## Support

For issues and questions:
- GitHub Issues: https://github.com/shifty81/GameEngine/issues
- Discussions: https://github.com/shifty81/GameEngine/discussions
