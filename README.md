# Game Engine - Hybrid C++20/.NET 9 Voxel Engine

A professional-grade voxel game engine combining C++20 (performance core) with .NET 9 (editor and high-level systems).

> **🎯 Windows-Only**: This project is built for Windows with a hybrid architecture using C++20 and .NET 9.  
> **📖 See [README_WINDOWS.md](README_WINDOWS.md)** for Windows build instructions  
> **🚀 See [QUICKSTART.md](QUICKSTART.md)** for quick start guide  
> **🗺️ See [ROADMAP.md](ROADMAP.md)** for comprehensive feature roadmap

---

## Platform

### ✅ Windows 10/11 (64-bit)
- **Architecture**: C++20 (Core) + .NET 9 (Editor)
- **Graphics**: DirectX 11
- **Build System**: MSBuild / Visual Studio 2022
- **Status**: ✅ **FULLY FUNCTIONAL** - See [README_WINDOWS.md](README_WINDOWS.md)

---

## Quick Start

### Prerequisites
- Windows 10/11 (64-bit)
- Visual Studio 2022 with C++ and .NET workloads
- .NET 9 SDK
- DirectX 11 capable GPU

### Build & Run
```powershell
# Clone
git clone https://github.com/shifty81/GameEngine.git
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

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Contributing

Contributions are welcome! Please submit pull requests or open issues for bugs and feature requests.
